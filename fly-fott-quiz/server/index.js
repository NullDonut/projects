import http from 'http';
import path from 'path';
import { fileURLToPath } from 'url';
import express from 'express';
import cors from 'cors';
import { Server } from 'socket.io';
import { QUESTIONS, POINT_LADDERS } from './questions.js';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const PORT = process.env.PORT || 3001;

const app = express();
app.use(cors());
app.use(express.json());

const server = http.createServer(app);
const io = new Server(server, {
  cors: { origin: '*' },
});

const sessions = new Map();

const ROOM_PREFIX = 'room:';
const PLAYERS_PREFIX = 'players:';
const genCode = () => {
  const chars = 'ABCDEFGHJKMNPQRSTUVWXYZ23456789';
  let code;
  do {
    code = Array.from({ length: 4 }, () => chars[Math.floor(Math.random() * chars.length)]).join('');
  } while (sessions.has(code));
  return code;
};

function createSession() {
  const code = genCode();
  const session = {
    code,
    status: 'lobby', // lobby | question | revealed | finished
    round: null,
    currentQuestionIndex: -1,
    questions: [],
    players: new Map(), // key: playerId
    playersByName: new Map(), // name -> playerId  (reconnect by name)
    answers: new Map(), // playerId -> { choice, timestampMs, correct }
    roundStartTime: 0,
    timeLimitSec: 0,
    answersLocked: true,
    lockedBy: null,
  };
  sessions.set(code, session);
  return session;
}

function resetRoundState(session, round) {
  session.round = round;
  session.questions = QUESTIONS[round];
  session.currentQuestionIndex = -1;
  session.status = 'lobby';
  session.answers = new Map();
  session.lockedBy = null;
  return {
    total: session.questions.length,
    points: round === 'individual' ? POINT_LADDERS.individual : POINT_LADDERS.rapidfire,
  };
}

function startQuestion(session) {
  session.currentQuestionIndex += 1;
  const q = session.questions[session.currentQuestionIndex];
  if (!q) {
    session.status = 'finished';
    return null;
  }
  session.status = 'question';
  session.answers = new Map();
  session.answersLocked = false;
  session.timeLimitSec = 20;
  session.roundStartTime = Date.now();
  return q;
}

function reveal(session, lockTimeMs) {
  if (session.timer) clearTimeout(session.timer);
  const q = session.questions[session.currentQuestionIndex];
  if (session.round === 'rapidfire') {
    // award points to first correct answer by server receive time
    let winner = null;
    for (const [pid, a] of session.answers) {
      if (a.correct && (!winner || a.receivedMs < winner.receivedMs)) winner = { pid, ...a };
    }
    if (winner) {
      const p = session.players.get(winner.pid);
      if (p) {
        p.score += q.points;
        p.correctCount = (p.correctCount || 0) + 1;
      }
    }
    session.winnerId = winner ? winner.pid : null;
  } else {
    for (const [pid, a] of session.answers) {
      if (a.correct && a.receivedMs <= lockTimeMs) {
        const p = session.players.get(pid);
        if (p) {
          p.score += q.points;
          p.correctCount = (p.correctCount || 0) + 1;
        }
      }
    }
  }
  session.status = 'revealed';
  session.answersLocked = true;
  return {
    correctIndex: q.correctIndex,
    correctAnswer: q.options[q.correctIndex],
    points: q.points,
  };
}

function lobbySnapshot(session, forJudge = true) {
  const players = [...session.players.values()].map((p) => ({
    id: p.id,
    name: p.name,
    score: p.score,
    connected: p.connected,
    correctCount: p.correctCount || 0,
  }));
  return {
    code: session.code,
    status: session.status,
    round: session.round,
    currentQuestionIndex: session.currentQuestionIndex,
    players,
  };
}

function emitPlayerList(session) {
  io.to(`${ROOM_PREFIX}${session.code}`).emit('server:playerList', lobbySnapshot(session));
}

function playerRoomName(session) {
  return `${ROOM_PREFIX}${session.code}`;
}

function playersRoomName(session) {
  return `${PLAYERS_PREFIX}${session.code}`;
}

// ---------- SOCKET LOGIC ----------

io.on('connection', (socket) => {
  // Judge: create session
  socket.on('host:createSession', () => {
    const session = createSession();
    socket.join(playerRoomName(session));
    socket.data.sessionCode = session.code;
    socket.data.role = 'judge';
    socket.emit('host:sessionCreated', { roomCode: session.code });
    emitPlayerList(session);
  });

  // Judge: rejoin existing session
  socket.on('host:joinSession', (payload = {}) => {
    const { roomCode } = payload;
    const session = sessions.get(roomCode?.toUpperCase());
    if (!session) return socket.emit('host:error', { message: 'Room not found' });
    socket.join(playerRoomName(session));
    socket.data.sessionCode = session.code;
    socket.data.role = 'judge';
    socket.emit('host:sessionReady', lobbySnapshot(session));
    emitPlayerList(session);
    // judge reloaded mid-question: resend current question with correctIndex
    if (session.status === 'question' && session.currentQuestionIndex >= 0) {
      const q = session.questions[session.currentQuestionIndex];
      socket.emit('server:showQuestion', {
        text: q.text,
        options: q.options,
        points: q.points,
        timeLimitSec: session.timeLimitSec,
        questionIndex: session.currentQuestionIndex,
        totalQuestions: session.questions.length,
        round: session.round,
        correctIndex: q.correctIndex,
      });
    } else if (session.status === 'revealed' && session.currentQuestionIndex >= 0) {
      const q = session.questions[session.currentQuestionIndex];
      socket.emit('server:revealResult', {
        correctIndex: q.correctIndex,
        correctAnswer: q.options[q.correctIndex],
        points: q.points,
        scores: [...session.players.values()].map((p) => ({ id: p.id, name: p.name, score: p.score })),
        round: session.round,
        winnerId: session.winnerId,
      });
    }
  });

  // Player: join
  socket.on('player:join', (payload = {}) => {
    const { roomCode, name } = payload;
    const session = sessions.get(roomCode?.toUpperCase());
    if (!session) return socket.emit('player:error', { message: 'Room not found' });
    const cleanName = (name || '').toString().trim().slice(0, 18);
    if (!cleanName) return socket.emit('player:error', { message: 'Enter your name first' });

    socket.join(playerRoomName(session));
    socket.join(playersRoomName(session));
    socket.data.sessionCode = session.code;
    socket.data.role = 'player';

    let player = session.players.get(session.playersByName.get(cleanName));
    if (!player) {
      player = {
        id: `p${Date.now().toString(36)}${Math.floor(Math.random() * 1e6).toString(36)}`,
        name: cleanName,
        score: 0,
        connected: true,
        socketId: socket.id,
        correctCount: 0,
      };
      session.players.set(player.id, player);
      session.playersByName.set(cleanName, player.id);
    } else {
      // reconnect: keep score
      player.socketId = socket.id;
      player.connected = true;
      player.reconnectedCount = (player.reconnectedCount || 0) + 1;
    }

    socket.emit('player:joined', { playerId: player.id, name: player.name, snapshot: lobbySnapshot(session, false) });
    emitPlayerList(session);

    // reconnected player mid-question: resend current question state
    if (session.status === 'question' && session.currentQuestionIndex >= 0) {
      const q = session.questions[session.currentQuestionIndex];
      socket.emit('server:showQuestion', {
        text: q.text,
        options: q.options,
        points: q.points,
        timeLimitSec: session.timeLimitSec,
        questionIndex: session.currentQuestionIndex,
        totalQuestions: session.questions.length,
        round: session.round,
      });
    } else if (session.status === 'revealed' && session.currentQuestionIndex >= 0) {
      const q = session.questions[session.currentQuestionIndex];
      socket.emit('server:revealResult', {
        correctIndex: q.correctIndex,
        correctAnswer: q.options[q.correctIndex],
        points: q.points,
        scores: [...session.players.values()].map((p) => ({ id: p.id, name: p.name, score: p.score })),
        round: session.round,
        winnerId: session.winnerId,
      });
    }
  });

  // Player: submit answer
  socket.on('player:submitAnswer', (payload = {}) => {
    const { choice } = payload;
    const session = sessions.get(socket.data.sessionCode);
    if (!session) return;
    const player = [...session.players.values()].find((p) => p.socketId === socket.id);
    if (!player) return;
    if (session.status !== 'question' || session.answersLocked) return;

    const answerKey = player.id;
    if (session.answers.has(answerKey)) return;

    const receivedMs = Date.now();
    const q = session.questions[session.currentQuestionIndex];
    if (!q) return;

    const answer = {
      playerId: player.id,
      choice,
      timestampMs: receivedMs,
      receivedMs,
      correct: choice === q.correctIndex,
    };
    session.answers.set(answerKey, answer);

    // inform judge of live progress
    io.to(playerRoomName(session)).emit('server:playerAnswered', {
      answeredCount: session.answers.size,
      totalCount: session.players.size,
      playerId: player.id,
      name: player.name,
    });

    // auto-reveal in rapid fire when first correct arrives
    if (session.round === 'rapidfire' && answer.correct) {
      const result = reveal(session, receivedMs);
      io.to(playerRoomName(session)).emit('server:revealResult', {
        ...result,
        scores: [...session.players.values()].map((p) => ({ id: p.id, name: p.name, score: p.score })),
        round: session.round,
        winnerId: session.winnerId,
      });
      emitPlayerList(session);
    }
  });

  // Judge: start round (first question)
  socket.on('host:startRound', (payload = {}) => {
    const { round } = payload;
    const session = sessions.get(socket.data.sessionCode);
    if (!session || socket.data.role !== 'judge') return;
    if (!['individual', 'rapidfire'].includes(round)) return;
    resetRoundState(session, round);
    sendNextQuestion(socket, session, round);
  });

  // Judge: next question (continue round or start rapidfire)
  socket.on('host:nextQuestion', (payload = {}) => {
    const { round } = payload;
    const session = sessions.get(socket.data.sessionCode);
    if (!session || socket.data.role !== 'judge') return;
    // rapid fire: restart question flow after a reveal
    if (session.status === 'revealed' && session.round === 'rapidfire') {
      sendNextQuestion(socket, session, session.round);
      return;
    }
    if (round === 'rapidfire') {
      resetRoundState(session, 'rapidfire');
      sendNextQuestion(socket, session, 'rapidfire');
      return;
    }
    sendNextQuestion(socket, session, session.round);
  });

  // Judge: reveal answer
  socket.on('host:revealAnswer', () => {
    const session = sessions.get(socket.data.sessionCode);
    if (!session || socket.data.role !== 'judge') return;
    if (session.status !== 'question') return;
    const result = reveal(session, Date.now());
    io.to(playerRoomName(session)).emit('server:revealResult', {
      ...result,
      scores: [...session.players.values()].map((p) => ({ id: p.id, name: p.name, score: p.score })),
      round: session.round,
      winnerId: session.winnerId,
    });
    emitPlayerList(session);
  });

  // Judge: end round -> final leaderboard
  socket.on('host:finishRound', () => {
    const session = sessions.get(socket.data.sessionCode);
    if (!session || socket.data.role !== 'judge') return;
    session.status = 'finished';
    const final = [...session.players.values()]
      .map((p) => ({ id: p.id, name: p.name, score: p.score, correctCount: p.correctCount || 0 }))
      .sort((a, b) => b.score - a.score);
    io.to(playerRoomName(session)).emit('server:finalScores', final);
    emitPlayerList(session);
  });

  // Judge: new session after finish
  socket.on('host:newSession', () => {
    const session = sessions.get(socket.data.sessionCode);
    if (session) {
      if (session.timer) clearTimeout(session.timer);
      io.to(playerRoomName(session)).emit('server:sessionClosed');
      sessions.delete(session.code);
      socket.leave(playerRoomName(session));
    }
    socket.data.sessionCode = null;
    const fresh = createSession();
    socket.join(playerRoomName(fresh));
    socket.data.sessionCode = fresh.code;
    socket.data.role = 'judge';
    socket.emit('host:sessionCreated', { roomCode: fresh.code });
    emitPlayerList(fresh);
  });

  socket.on('disconnect', () => {
    const session = sessions.get(socket.data.sessionCode);
    if (!session) return;
    if (socket.data.role === 'player') {
      const player = [...session.players.values()].find((p) => p.socketId === socket.id);
      if (player) {
        player.connected = false;
        player.socketId = null;
        emitPlayerList(session);
      }
    }
  });
});

function sendNextQuestion(judgeSocket, session, round) {
  if (session.timer) clearTimeout(session.timer);
  const q = startQuestion(session);
  if (!q) {
    io.to(playerRoomName(session)).emit('server:finalScores', finalSnapshot(session));
    emitPlayerList(session);
    return;
  }
  const payload = {
    text: q.text,
    options: q.options,
    points: q.points,
    timeLimitSec: session.timeLimitSec,
    questionIndex: session.currentQuestionIndex,
    totalQuestions: session.questions.length,
    round: session.round,
    correctIndex: q.correctIndex,
  };
  // send full payload (incl. correctIndex) to judge socket
  judgeSocket.emit('server:showQuestion', payload);
  // players get the same question but without the correctIndex
  const playerPayload = {
    text: q.text,
    options: q.options,
    points: q.points,
    timeLimitSec: session.timeLimitSec,
    questionIndex: session.currentQuestionIndex,
    totalQuestions: session.questions.length,
    round: session.round,
  };
  io.to(playersRoomName(session)).emit('server:showQuestion', playerPayload);

  // server-authoritative timer expiry -> lock answers
  session.timer = setTimeout(() => {
    if (session.status === 'question' && !session.answersLocked) {
      session.answersLocked = true;
      io.to(playerRoomName(session)).emit('server:timeUp', {
        answeredCount: session.answers.size,
        totalCount: session.players.size,
      });
      emitPlayerList(session);
    }
  }, session.timeLimitSec * 1000);
}

function finalSnapshot(session) {
  return [...session.players.values()]
    .map((p) => ({ id: p.id, name: p.name, score: p.score, correctCount: p.correctCount || 0 }))
    .sort((a, b) => b.score - a.score);
}

app.get('/health', (_req, res) => res.json({ ok: true, activeSessions: sessions.size }));

if (process.env.NODE_ENV === 'production') {
  const clientDist = path.join(__dirname, '..', 'client', 'dist');
  app.use(express.static(clientDist));
  app.get('*', (_req, res) => res.sendFile(path.join(clientDist, 'index.html')));
}

server.listen(PORT, () => {
  console.log(`Quiz server listening on :${PORT}`);
});
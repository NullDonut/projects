import React, { useEffect, useRef, useState } from 'react';
import { getSocket, resetSocket } from '../socket.js';

const LETTERS = ['A', 'B', 'C', 'D'];

function Judge() {
  const socket = useRef(null);
  const [roomCode, setRoomCode] = useState(null);
  const [players, setPlayers] = useState([]);
  const [question, setQuestion] = useState(null);
  const [status, setStatus] = useState('lobby'); // lobby | question | revealed | finished
  const [round, setRound] = useState(null);
  const [answered, setAnswered] = useState({ count: 0, total: 0 });
  const [revealed, setRevealed] = useState(null);
  const [scores, setScores] = useState([]);
  const [finals, setFinals] = useState(null);
  const [error, setError] = useState(null);
  const [joinCode, setJoinCode] = useState('');
  const [timeLeft, setTimeLeft] = useState(null);
  const [joining, setJoining] = useState(false);
  const timerRef = useRef(null);

  useEffect(() => {
    const s = getSocket();
    socket.current = s;

    s.on('connect', () => {
      if (roomCode) {
        s.emit('host:joinSession', { roomCode });
      }
    });

    s.on('host:sessionCreated', ({ roomCode: code }) => {
      setRoomCode(code);
      setStatus('lobby');
      setQuestion(null);
      setRevealed(null);
      setScores([]);
      setFinals(null);
      setAnswered({ count: 0, total: 0 });
    });

    s.on('host:sessionReady', (snap) => {
      setRoomCode(snap.code);
      setStatus(snap.status);
      setPlayers(snap.players);
      setRound(snap.round);
      if (snap.status === 'finished') setFinals(sortByScore(snap.players));
    });

    s.on('server:playerList', (snap) => {
      setPlayers(snap.players);
      if (snap.status === 'finished') setFinals(sortByScore(snap.players));
    });

    s.on('server:showQuestion', (q) => {
      setQuestion(q);
      setStatus('question');
      setRound(q.round);
      setAnswered({ count: 0, total: playersRef.current.length || q.totalQuestions });
      setRevealed(null);
      setTimeLeft(q.timeLimitSec);
      startCountdown(q.timeLimitSec);
    });

    s.on('server:playerAnswered', ({ answeredCount, totalCount }) => {
      setAnswered({ count: answeredCount, total: totalCount });
    });

    s.on('server:timeUp', ({ answeredCount, totalCount }) => {
      setAnswered({ count: answeredCount, total: totalCount });
    });

    s.on('server:revealResult', (res) => {
      setRevealed(res);
      setScores(res.scores);
      setStatus('revealed');
      clearCountdown();
      setAnswered({ count: 0, total: playersRef.current.length });
    });

    s.on('server:finalScores', (final) => {
      setFinals(sortByScore(final));
      setStatus('finished');
    });

    s.on('host:error', ({ message }) => setError(message));

    s.connect();
    return () => {
      clearCountdown();
      s.off();
      resetSocket();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const playersRef = useRef(players);
  playersRef.current = players;

  const startCountdown = (secs) => {
    clearCountdown();
    timerRef.current = setInterval(() => {
      setTimeLeft((t) => {
        if (t === null) return secs;
        const next = Math.round((t - 0.1) * 10) / 10;
        if (next <= 0) {
          clearInterval(timerRef.current);
          timerRef.current = null;
          return 0;
        }
        return next;
      });
    }, 100);
  };

  const clearCountdown = () => {
    if (timerRef.current) clearInterval(timerRef.current);
    timerRef.current = null;
  };

  const sortByScore = (list) => [...list].sort((a, b) => b.score - a.score);

  const createSession = () => {
    setError(null);
    socket.current.emit('host:createSession');
  };

  const joinExisting = () => {
    setError(null);
    setJoining(true);
    socket.current.emit('host:joinSession', { roomCode: joinCode.trim().toUpperCase() });
    setTimeout(() => setJoining(false), 600);
  };

  const startRound = (r) => {
    setError(null);
    socket.current.emit('host:startRound', { round: r });
  };

  const nextQuestion = () => {
    socket.current.emit('host:nextQuestion', { round });
  };

  const reveal = () => {
    socket.current.emit('host:revealAnswer');
  };

  const finishRound = () => {
    socket.current.emit('host:finishRound');
  };

  const newSession = () => {
    socket.current.emit('host:newSession');
  };

  const roomLink = roomCode ? `${window.location.origin}/play/${roomCode}` : '';

  return (
    <div className="app judge-app">
      {error && <div className="toast error">{error}</div>}

      {!roomCode && (
        <div className="auth-screen">
          <img className="logo-img" src="/logo.jpg" alt="Fylfot Quiz logo" />
          <h1>Fly &amp; Fott Quiz</h1>
          <p className="subtitle">Judge console — host the room, run the rounds, reveal the glory.</p>

          <div className="card">
            <button className="btn primary big" onClick={createSession}>
              🚀 Create new session
            </button>
            <div className="divider"><span>or rejoin a room</span></div>
            <div className="row">
              <input
                className="input code-input"
                placeholder="ROOM"
                value={joinCode}
                maxLength={4}
                onChange={(e) => setJoinCode(e.target.value.toUpperCase())}
                onKeyDown={(e) => e.key === 'Enter' && joinExisting()}
              />
              <button className="btn" onClick={joinExisting} disabled={joining || !joinCode.trim()}>
                Join
              </button>
            </div>
          </div>
        </div>
      )}

      {roomCode && status === 'lobby' && (
        <div className="lobby">
          <header className="topbar">
            <h1><img className="logo-img" src="/logo.jpg" alt="" /> Fly &amp; Fott Quiz</h1>
            <button className="btn ghost" onClick={newSession}>♻️ New room</button>
          </header>

          <div className="room-hero">
            <div className="room-code">{roomCode}</div>
            <p className="share-line">Players join at</p>
            <div className="share-box">{roomLink}</div>
            <button
              className="btn ghost small"
              onClick={() => navigator.clipboard?.writeText(roomLink)}
            >
              📋 Copy invite link
            </button>
          </div>

          <div className="panel">
            <div className="panel-title">👥 Players · {players.length}</div>
            {players.length === 0 && <p className="empty">Waiting for players to join…</p>}
            <div className="player-grid">
              {players.map((p) => (
                <div key={p.id} className={`player-chip ${p.connected ? '' : 'offline'}`}>
                  <span className="dot" /> {p.name}
                </div>
              ))}
            </div>
            <div className="hint">Start once 4+ players are in.</div>
          </div>

          <div className="round-picker">
            <h2>Pick a round</h2>
            <div className="round-cards">
              <button className="round-card individual" onClick={() => startRound('individual')}>
                <span className="big-emoji">💎</span>
                <strong>Individual</strong>
                <small>Millionaire-style · points climb per question</small>
              </button>
              <button className="round-card rapidfire" onClick={() => startRound('rapidfire')}>
                <span className="big-emoji">⚡</span>
                <strong>Rapid Fire</strong>
                <small>Buzzer style · first correct answer wins</small>
              </button>
            </div>
          </div>
        </div>
      )}

      {roomCode && (status === 'question' || status === 'revealed') && question && (
        <div className="live">
          <header className="topbar">
            <h1><img className="logo-img" src="/logo.jpg" alt="" /> {roomCode} <span className="round-tag">{round === 'rapidfire' ? '⚡ Rapid Fire' : '💎 Individual'}</span></h1>
            <button className="btn ghost" onClick={newSession}>♻️ New room</button>
          </header>

          <div className="question-status">
            <span className="q-count">Q{question.questionIndex + 1} / {question.totalQuestions}</span>
            {status === 'question' && timeLeft !== null && (
              <span className={`timer ${timeLeft <= 5 ? 'danger' : ''}`}>⏱ {timeLeft.toFixed(1)}s</span>
            )}
            <span className="points-tag">worth {question.points.toLocaleString()} pts</span>
          </div>

          <div className="question-card">
            <h2>{question.text}</h2>
            <div className="options-grid judge-options">
              {question.options.map((opt, i) => (
                <div
                  key={i}
                  className={`option ${revealed ? (i === revealed.correctIndex ? 'correct' : 'dim') : 'idle'}`}
                >
                  <span className="letter">{LETTERS[i]}</span>
                  {opt}
                </div>
              ))}
            </div>
          </div>

          <div className="control-bar">
            <div className="answered-tracker">
              {status === 'question'
                ? <span>✅ {answered.count} of {Math.max(answered.total, players.length)} answered</span>
                : <span>🔒 Answers locked</span>}
            </div>
            {status === 'question' && (
              <button className="btn primary big" onClick={reveal}>💡 Reveal answer</button>
            )}
            {status === 'revealed' && (
              <button className="btn primary big" onClick={nextQuestion}>
                {round === 'rapidfire' ? '⚡ Next question' : '➡️ Next question'}
              </button>
            )}
          </div>

          {revealed && round === 'rapidfire' && (
            <div className="winner-banner">
              🏆 {revealed.winnerId ? (players.find((p) => p.id === revealed.winnerId)?.name || 'Someone') : 'No one'} grabbed it — {revealed.points} pts
            </div>
          )}
          {revealed && round === 'individual' && (
            <div className="reveal-note">
              ✅ Correct answer: {LETTERS[revealed.correctIndex]}) {revealed.correctAnswer}
            </div>
          )}

          <Scoreboard scores={scores} players={players} />
          <div className="finish-row">
            <button className="btn danger" onClick={finishRound}>🏁 End round</button>
          </div>
        </div>
      )}

      {roomCode && status === 'finished' && finals && (
        <div className="finale">
          <header className="topbar">
            <h1><img className="logo-img" src="/logo.jpg" alt="" /> {roomCode}</h1>
            <button className="btn ghost" onClick={newSession}>♻️ New room</button>
          </header>
          <div className="confetti">🏆</div>
          <h1 className="finale-title">Final leaderboard</h1>
          <div className="final-list">
            {finals.map((p, i) => (
              <div key={p.id} className={`final-row ${i === 0 ? 'winner' : ''}`}>
                <span className="rank">{i === 0 ? '🥇' : i === 1 ? '🥈' : i === 2 ? '🥉' : `${i + 1}.`}</span>
                <span className="fname">{p.name}</span>
                <span className="fscore">{p.score.toLocaleString()}</span>
              </div>
            ))}
          </div>
          <button className="btn primary big" onClick={newSession}>♻️ Start a new session</button>
        </div>
      )}

      <div className="footer-credit">Made by <b>nullDonut</b> &amp; <b>Pranjal Bhatt</b> for the school quiz</div>
    </div>
  );
}

function Scoreboard({ scores, players }) {
  const sorted = [...scores].sort((a, b) => b.score - a.score);
  if (sorted.length === 0) return null;
  return (
    <div className="scoreboard">
      <div className="panel-title">📊 Live scores</div>
      <div className="score-rows">
        {sorted.map((p) => {
          const pj = players.find((x) => x.id === p.id);
          return (
            <div key={p.id} className="score-row">
              <span className="sname">{pj?.connected === false ? '⚠️' : ''} {p.name}</span>
              <span className="svalue">{p.score.toLocaleString()}</span>
            </div>
          );
        })}
      </div>
    </div>
  );
}

export default Judge;

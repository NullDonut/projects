import React, { useEffect, useRef, useState } from 'react';
import { useParams } from 'react-router-dom';
import { getSocket, resetSocket } from '../socket.js';

const LETTERS = ['A', 'B', 'C', 'D'];

function Player() {
  const { roomCode: paramCode } = useParams();
  const socket = useRef(null);
  const [roomCode] = useState(paramCode ? paramCode.toUpperCase() : '');
  const [name, setName] = useState('');
  const [playerId, setPlayerId] = useState(null);
  const [joined, setJoined] = useState(false);
  const [question, setQuestion] = useState(null);
  const [selected, setSelected] = useState(null);
  const [timeLeft, setTimeLeft] = useState(null);
  const [result, setResult] = useState(null);
  const [finals, setFinals] = useState(null);
  const [sessionClosed, setSessionClosed] = useState(false);
  const [error, setError] = useState(null);
  const [score, setScore] = useState(0);
  const [lastChange, setLastChange] = useState(null);
  const timerRef = useRef(null);
  const joinedRef = useRef(false);
  const nameRef = useRef('');
  nameRef.current = name;

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

  useEffect(() => {
    const s = getSocket();
    socket.current = s;

    s.on('connect', () => {
      if (joinedRef.current && nameRef.current) {
        s.emit('player:join', { roomCode, name: nameRef.current });
      }
    });

    s.on('player:joined', ({ playerId: id, name: n, snapshot }) => {
      joinedRef.current = true;
      setJoined(true);
      setPlayerId(id);
      setName(n);
      setScore(snapshot.players.find((p) => p.id === id)?.score || 0);
      if (snapshot.status === 'finished') setFinals(snapshot.players);
      if (snapshot.status === 'question') {
        // reconnected mid-question; judge will push fresh question on next
      }
    });

    s.on('server:showQuestion', (q) => {
      setQuestion(q);
      setSelected(null);
      setResult(null);
      setTimeLeft(q.timeLimitSec);
      startCountdown(q.timeLimitSec);
    });

    s.on('server:timeUp', () => {
      clearCountdown();
      setTimeLeft(0);
    });

    s.on('server:revealResult', (res) => {
      clearCountdown();
      setTimeLeft(0);
      setResult(res);
      const myRow = (res.scores || []).find((p) => p.id === playerIdRef.current);
      if (myRow) {
        if (myRow.score !== scoreRef.current) {
          setLastChange(myRow.score - scoreRef.current);
        }
        setScore(myRow.score);
      }
    });

    s.on('server:finalScores', (final) => {
      clearCountdown();
      setFinals(final);
      const my = final.find((p) => p.id === playerIdRef.current);
      if (my) setScore(my.score);
    });

    s.on('server:sessionClosed', () => {
      setSessionClosed(true);
      setJoined(false);
      setQuestion(null);
      setFinals(null);
    });

    s.on('player:error', ({ message }) => setError(message));

    s.connect();
    return () => {
      clearCountdown();
      s.off();
      resetSocket();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const playerIdRef = useRef(playerId);
  playerIdRef.current = playerId;
  const scoreRef = useRef(score);
  scoreRef.current = score;

  const join = () => {
    if (!name.trim()) return;
    setError(null);
    socket.current.emit('player:join', { roomCode, name: name.trim() });
  };

  const answer = (i) => {
    if (selected !== null || !question) return;
    setSelected(i);
    socket.current.emit('player:submitAnswer', { choice: i });
  };

  const isCorrect = () => {
    if (!result || selected === null) return false;
    return selected === result.correctIndex;
  };

  if (sessionClosed) {
    return (
      <div className="app player-app">
        <div className="auth-screen">
          <div className="logo">👋</div>
          <h1>Session ended</h1>
          <p>The host closed this room. Thanks for playing!</p>
        </div>
      </div>
    );
  }

  if (!joined) {
    return (
      <div className="app player-app">
        {error && <div className="toast error">{error}</div>}
        <div className="auth-screen">
          <div className="logo">🕹️</div>
          <h1>Join the quiz</h1>
          <p className="subtitle">Room <strong>{roomCode}</strong></p>
          <div className="card">
            <input
              className="input"
              placeholder="Your name"
              value={name}
              maxLength={18}
              autoFocus
              onChange={(e) => setName(e.target.value)}
              onKeyDown={(e) => e.key === 'Enter' && join()}
            />
            <button className="btn primary big" onClick={join} disabled={!name.trim()}>
              ▶️ Join game
            </button>
          </div>
        </div>
      </div>
    );
  }

  if (finals) {
    return (
      <div className="app player-app">
        <div className="finale">
          <div className="confetti">🏆</div>
          <h1 className="finale-title">Final leaderboard</h1>
          <div className="your-score">You scored <strong>{score.toLocaleString()}</strong></div>
          <div className="final-list">
            {finals.map((p, i) => (
              <div key={p.id} className={`final-row ${p.id === playerId ? 'me' : ''} ${i === 0 ? 'winner' : ''}`}>
                <span className="rank">{i === 0 ? '🥇' : i === 1 ? '🥈' : i === 2 ? '🥉' : `${i + 1}.`}</span>
                <span className="fname">{p.name}{p.id === playerId ? ' (you)' : ''}</span>
                <span className="fscore">{p.score.toLocaleString()}</span>
              </div>
            ))}
          </div>
        </div>
      </div>
    );
  }

  if (!question) {
    return (
      <div className="app player-app">
        <div className="waiting-screen">
          <div className="logo">⏳</div>
          <h1>Waiting for the quiz…</h1>
          <p>Hi <strong>{name}</strong> — you're in room {roomCode}. The host will start the first question soon.</p>
        </div>
      </div>
    );
  }

  return (
    <div className="app player-app">
      <div className="player-top">
        <span className="room-tag">📡 {roomCode}</span>
        <span className="player-score">
          Score <strong>{score.toLocaleString()}</strong>
          {lastChange !== null && lastChange !== 0 && <b className={lastChange > 0 ? 'gain' : ''}> {lastChange > 0 ? '+' : ''}{lastChange.toLocaleString()}</b>}
        </span>
      </div>

      {result ? (
        <div className="player-result">
          {isCorrect() ? (
            <div className="big-feedback correct-feedback">✅ Correct!<small>+{result.points.toLocaleString()} pts</small></div>
          ) : selected !== null ? (
            <div className="big-feedback wrong-feedback">❌ Not quite.<small>The answer was {LETTERS[result.correctIndex]}).</small></div>
          ) : (
            <div className="big-feedback timeout-feedback">⏰ Time's up!<small>The answer was {LETTERS[result.correctIndex]}).</small></div>
          )}
          <div className="waiting-hint">Waiting for the host…</div>
        </div>
      ) : (
        <div className="player-question">
          <div className="player-meta">
            <span className="q-count">Q{question.questionIndex + 1} / {question.totalQuestions}</span>
            {timeLeft !== null && (
              <span className={`timer ${timeLeft <= 5 ? 'danger' : ''}`}>⏱ {timeLeft.toFixed(1)}s</span>
            )}
          </div>
          <h2 className="q-text">{question.text}</h2>
          <div className="options-grid">
            {question.options.map((opt, i) => (
              <button
                key={i}
                className={`option playable ${selected === i ? 'selected' : ''} ${selected !== null && selected !== i ? 'locked' : ''}`}
                onClick={() => answer(i)}
                disabled={selected !== null}
              >
                <span className="letter">{LETTERS[i]}</span>
                {opt}
              </button>
            ))}
          </div>
          {selected !== null && <div className="locked-hint">✅ Answer locked in — sit tight!</div>}
        </div>
      )}
    </div>
  );
}

export default Player;

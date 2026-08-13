# Quiz App — PRD & Workflow

## 1. Concept

One **Judge/Host** device controls the game. **4+ Client** devices are players.
Two round types:
- **Individual Round** — Who Wants to Be a Millionaire style: one question at a time, shown to everyone, increasing point value per question, answer within a timer.
- **Rapid Fire Round** — buzzer-style: question shown, first player to answer (correctly) wins the point, others locked out.

Since players may **not** be on the same WiFi as the judge, devices can't talk to each other directly — everything routes through one central server on the internet. This makes it a true client-server app, not a LAN broadcast app.

## 2. Architecture

```
[Judge Browser] ─┐
[Player 1]       ├─ WebSocket ─→ [Node.js + Socket.IO server] ─→ holds game state
[Player 2]       │
[Player N]       ┘
```

- **Backend:** Node.js + Express + Socket.IO. Holds all game state in memory (single active session is enough for this use case). Deploy to Render/Railway/Fly.io (free tier is fine) so it has a public URL.
- **Frontend:** One React app, two views based on route — `/judge` and `/play/:roomCode`. Deploy to Vercel/Netlify.
- **Why Socket.IO over raw WebSocket:** auto-reconnect, room support (built-in room-code grouping), fallback transport — saves you reimplementing all of that.

Everything is event-driven. The judge never talks to clients directly; it emits an event to the server, the server broadcasts to the room.

## 3. Data Model

```
Session { code, status, currentRound, currentQuestionIndex }
Player  { id, name, socketId, score, connected: bool }
Question{ id, round, text, options[4], correctIndex, points, timeLimitSec }
Answer  { playerId, questionId, choice, timestampMs, correct }
```

Keep questions in a static JSON/array per round — no DB needed unless you want persistence across restarts.

## 4. Core Socket Events

| Event | Direction | Payload |
|---|---|---|
| `host:createSession` | Judge → Server | — returns `roomCode` |
| `player:join` | Player → Server | `{roomCode, name}` |
| `server:playerList` | Server → Judge | `[{id,name,connected}]` |
| `host:startQuestion` | Judge → Server | `{questionId}` |
| `server:showQuestion` | Server → All Players | `{text, options, timeLimitSec}` (no correct answer sent) |
| `player:submitAnswer` | Player → Server | `{choice, timestampMs}` |
| `host:revealAnswer` | Judge → Server | — triggers scoring |
| `server:revealResult` | Server → All | `{correctIndex, scores[]}` |
| `host:nextQuestion` | Judge → Server | — |

Rapid fire uses the same flow, except the server accepts only the **first correct** `player:submitAnswer` per question and ignores the rest (compare `timestampMs` server-side, not client-side — never trust client clocks for ordering, use server receive time).

## 5. Scoring Logic

- **Individual round:** fixed point ladder per question index (e.g. Q1=100 ... Q15=1,000,000-style scaling, or whatever ladder you define). Correct = add points. Wrong = round ends for that player (classic WWTBAM) or just no points (simpler variant — decide which).
- **Rapid fire:** flat points per question, awarded only to first correct responder (by server receive-timestamp).
- Server is the single source of truth for scores — never trust client-submitted correctness.

## 6. Event Workflow (a full game)

1. Judge opens `/judge` → clicks "Create Session" → gets a room code / shareable link.
2. Players open the link on any device (any network) → enter name → land in lobby, marked "waiting."
3. Judge's screen lists connected players live. Judge starts once 4+ are in.
4. Judge picks a round (Individual or Rapid Fire) and hits "Next Question."
5. Server pushes the question + timer to all players simultaneously.
6. Players submit answers; judge's screen shows live "X of N answered."
7. Judge hits "Reveal" → server locks further answers, computes correctness/scores, broadcasts result + updated leaderboard to everyone.
8. Repeat 4–7 until questions exhausted.
9. Final leaderboard screen on all devices.

## 7. Build Order (recommended)

1. Backend: Socket.IO server with session creation, join, and player list broadcast. Test with two browser tabs.
2. Add question push + answer submission + reveal/scoring logic (no UI polish yet — console.log states).
3. Judge frontend: create session, player list, question controls, reveal button, live scoreboard.
4. Player frontend: join screen, waiting screen, question screen with timer, result screen.
5. Add rapid-fire variant (mostly reuses the same components, different scoring rule).
6. Deploy backend + frontend, test across two separate networks (e.g. WiFi + mobile data) to confirm the "not same WiFi" requirement actually works.

## 8. Open Decisions (need your call before backend build starts)

- Individual round: does a wrong answer end that player's turn, or just cost them the point?
- Timer: same countdown for everyone, or does each player get their own clock from when they see the question?
- Reconnect handling: if a player's app crashes/reloads mid-question, should they rejoin with their score intact? (Socket.IO makes this easy if you key state by player name/id rather than socket id.)


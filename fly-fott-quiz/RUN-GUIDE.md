# Fly & Fott Quiz — Run Guide

One Judge/Host device runs the game. 4+ player devices join via a room code. Everything routes
through a central Node.js + Socket.IO server, so players do NOT need to be on the same WiFi as the judge.

## Project layout

```
fly-fott-quiz/
├── package.json          # root convenience scripts
├── prd.md                # original product requirements
├── server/               # Express + Socket.IO backend (all game state in memory)
│   ├── index.js          # socket events + scoring logic
│   └── questions.js      # static question pools + point ladders
└── client/               # Vite + React frontend
    ├── vite.config.js    # dev proxy /socket.io -> :3001
    └── src/
        ├── pages/Judge.jsx   # route /judge  (host console)
        ├── pages/Player.jsx  # route /play/:roomCode
        └── socket.js         # shared Socket.IO client (auto-reconnect)
```

## Quick start (local dev)

```bash
cd /data/data/com.termux/files/home/proj/fly-fott-quiz
npm --prefix server install && npm --prefix client install
```

```bash
# terminal 1 — backend
npm --prefix server run dev        # http://localhost:3001

# terminal 2 — frontend
npm --prefix client run dev        # http://localhost:5173  (proxies sockets to :3001)
```

## Gameplay

1. **Judge** opens `http://localhost:5173/judge` → **🚀 Create new session** → gets a room code
   like `ABCD` and an invite link `http://localhost:5173/play/ABCD`.
2. **Players** open that link on any device/network → enter a name → land in the lobby.
3. Judge waits for 4+ players, then picks a round:
   - **💎 Individual** — Millionaire-style, points climb per question (100 → 1,000,000).
     Correct = add points; wrong/skip = 0 points (simpler variant).
   - **⚡ Rapid Fire** — buzzer style, flat 500 pts. Only the first **correct** answer
     (by server receive time) scores; all others are locked out; auto-reveal on first correct.
4. Each question has a shared 20s countdown. Judge sees the correct answer + live
   "X of N answered". Judge hits **💡 Reveal answer** to lock scoring, then **➡️ Next question**.
5. Judge hits **🏁 End round** → final leaderboard on all devices. **♻️ New room** resets.

## Testing on one device

Open `/judge` in one tab and `/play/ABCD` in 2–4 other tabs. Same code path as real devices.

## Reconnect

- Players keyed by name: reload/crash → rejoin with same name, score intact, current
  question/result re-pushed automatically.
- Judge can reload and rejoin the room via the room code.

## Production / deploy

The backend can serve the built client itself (single deploy):

```bash
npm --prefix client run build      # builds client/dist
NODE_ENV=production npm --prefix server start   # serves API + built client on PORT (default 3001)
```

- **Backend:** Render / Railway / Fly.io — `npm --prefix server start`, set `PORT`.
- **Frontend (separate deploy, optional):** Vercel / Netlify from `client/`, set
  `VITE_SERVER_URL` to the backend URL. Otherwise keep the single-deploy backend approach.

## Editing questions

Edit `server/questions.js`:
- `baseIndividual` / `baseRapid` — the question pools (`text`, `options[4]`, `correctIndex`).
- `POINT_LADDERS.individual` — per-question points array.
- `POINT_LADDERS.rapidfire` — flat points per rapid-fire question.
- Time limit per question: `session.timeLimitSec = 20` in `server/index.js`.

## Decisions taken (from PRD open questions)

- Wrong answer in Individual round: **just no points** (player keeps playing).
- Timer: **one shared countdown** for everyone (server-authoritative).
- Reconnect: **by player name**, score retained.

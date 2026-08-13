# Fly & Fott Quiz — Project Context

> **Purpose of this file:** everything needed to resume development exactly where it left off.
> Read `RUN-GUIDE.md` for usage, `prd.md` for the original requirements.

## tl;dr — current state

- ✅ Backend + frontend fully built, wired to the PRD
- ✅ All PRD core socket events implemented, scoring verified via automated socket tests
- ✅ Production build works; backend can self-serve the built client (single deploy)
- ✅ Deployed-and-running on `github.com/NullDonut/projects` (branch `main`, commit `8ab976d`)
- ✅ Rebranded: Fylfot Public School, Dehradun palette (dark green `#006A4E`/`#114816` + golden yellow `#ffb601`), logo image, charcoal page background, author credits
- 🚧 Not yet deployed to a host (Render/Railway/Fly.io). Not yet tested across two real networks.
- 🧭 Two PRD "open decisions" were made at build time (see below) — free to revisit.

---

## What was built (session summary, Aug 2026)

**Backend — `server/` (Express + Socket.IO, ESM)**
- All game state in memory (single active session per room code). No DB.
- Room codes: 4 chars from `ABCDEFGHJKMNPQRSTUVWXYZ23456789` (`server/index.js:genCode`).
- Data model implemented: `Session {code,status,round,currentQuestionIndex,questions,players,playersByName,answers,...}`, `Player {id,name,score,connected,socketId,correctCount}`, `Question {text,options[4],correctIndex,points}`, `Answer {playerId,choice,receivedMs,correct}`.
- Events implemented (PRD §4 + extras):
  | Event | Who | Notes |
  |---|---|---|
  | `host:createSession` | Judge→Srv | returns `host:sessionCreated {roomCode}` |
  | `host:joinSession` | Judge→Srv | reconnect; resends current question/result |
  | `player:join` | Player→Srv | by name; **reconnect keeps score**; resends question state |
  | `server:playerList` | Srv→Judge | live lobby snapshot |
  | `host:startRound` | Judge→Srv | `{round}` → first question (tee for all Qs) |
  | `host:nextQuestion` | Judge→Srv | advance, or `{round:'rapidfire'}` to switch rounds |
  | `server:showQuestion` | Srv→All | judge gets `correctIndex`, players do NOT |
  | `player:submitAnswer` | Player→Srv | `{choice}`; ignores duplicates & locked/unanswered phases |
  | `host:revealAnswer` | Judge→Srv | locks answers, scores, broadcasts `server:revealResult` |
  | `server:revealResult` | Srv→All | `{correctIndex, correctAnswer, points, scores[], winnerId}` |
  | `server:timeUp` | Srv→All | after server-authoritative 20s timer |
  | `server:playerAnswered` | Srv→Judge | "X of N answered" live tracker |
  | `host:finishRound` | Judge→Srv | `server:finalScores` to all |
  | `host:newSession` | Judge→Srv | closes room (`server:sessionClosed`), makes fresh room |
  | `host:error` / `player:error` | Srv→client | bad room / empty name |
- **Rooms:** judge + players join `room:CODE`; players ALSO join `players:CODE`. The player-only `showQuestion` broadcast goes to `players:CODE` so the judge never sees the no-`correctIndex` payload (guards against overwriting its answer key). **Do not remove this split.**
- **Scoring (`reveal()` in `server/index.js`):**
  - Individual: add question points for every correct answer (wrong/skip = 0); `POINT_LADDERS.individual = [100,200,300,500,1000,2000,4000,8000,16000,32000,64000,125000,250000,500000,1000000]`.
  - Rapid fire: flat 500; only the first correct answer by **server `receivedMs`** scores; auto-reveals on arrival of the first correct answer (client clocks never trusted).
- Timer: `session.timeLimitSec = 20` fixed in `startQuestion()`. One shared countdown; expiry locks answers + broadcasts `server:timeUp`. Timer is cleared in `reveal()` and `sendNextQuestion()` (prevents stale fire).

**Frontend — `client/` (Vite 6 + React 18 + react-router 6 + socket.io-client 4)**
- `getSocket()`/`resetSocket()` shared singleton in `src/socket.js`, auto-connect + reconnection.
- `/judge` — `src/pages/Judge.jsx`: create/rejoin session, live player list (green/red dots), room hero w/ copyable invite link, round picker cards (Individual/Rapid Fire), question with answer key + countdown + points tag, "✅ X of N answered", Reveal/Next controls, live scoreboard, rapid-fire winner banner, End round → final leaderboard, New room. 100ms countdown stops at zero.
- `/play/:roomCode` — `src/pages/Player.jsx`: name join screen, waiting screen, question options (A–D), answer lock-in (`selected !== null` disables), correct/wrong/“time's up” gradient feedback, live score w/ delta pop, final leaderboard, session-closed screen. Uses refs (`nameRef`) in the `connect` handler to avoid stale-closure rejoin bugs.
- Styling: `src/styles.css` — **charcoal page background** (`#141414 → #262626 → #1a1a1a`), Fylfot School accent palette (dark green `#006A4E`/`#1F9D6E`, golden yellow `#FFB601`/`#D3A666`), gradient text/badges, emojis, cards w/ `backdrop-filter`, responsive to 1 column on ≤600px, `prefers-reduced-motion` respected.
- **Branding:** logo image served from `client/public/logo.jpg` (copied from `IMG-20260813-WA0000.jpg`) shown on the judge auth screen, all topbars, and player auth/waiting/session-end screens (`.logo-img`). Author footer "Made by nullDonut & Pranjal Bhatt" on judge + player screens (`.footer-credit`). Colors were taken from the Fylfot Public School website (`fylfotschool.in`, `webroot/css/style.css`): dark green `#006A4E`/`#114816`, golden yellow `#ffb601`/`#d3a666`.
- Dev: Vite proxy `./socket.io` → `http://localhost:3001` (`client/vite.config.js`, override target with `VITE_PROXY_TARGET`).

**Questions — `server/questions.js`**
- 15 individual + 10 rapid-fire pool. Correct indices are fixed per question; **options are NOT shuffled** (only question **order** is shuffled per session via `buildRound`).
- To edit content: edit `baseIndividual` / `baseRapid` and the ladders. To shuffle options per session, add a shuffle step there (would require remapping `correctIndex`).

---

## PRD open decisions → what was chosen

| PRD question | Decision at build |
|---|---|
| Individual wrong answer: end turn or no points? | **No points, keep playing** (simpler variant) |
| Timer: shared or per-player? | **Shared countdown**, server-authoritative |
| Reconnect: rejoin with score? | **Yes** — players keyed by exact name; rejoin re-pushes current question/result |

All three are encapsulated and easy to change (turn-elimination would need an `eliminated` flag per player + winner flow; per-player timers need per-socket timestamps).

---

## Verification done (don't skip before next change)

Automated socket tests (written ad-hoc, since removed from repo) covered:
- create session → 4 players join → player list broadcast
- individual Q1: correctIndex only to judge, scoring (+100/0/0), next question climbs points
- double-submit ignored, reveal-lock respected
- rapid fire: flat 500, only 1 gainer, auto-reveal on first correct, wrong answers don't score
- timeUp lock, finish round → sorted final scores
- disconnect marks offline (score retained), reconnect restores connected=true + score
- **reconnect mid-question** (player gets current Q without correctIndex; judge gets Q with correctIndex; reveal still works after judge reload)
- **regression**: judge does NOT receive the player-only `showQuestion`
- production mode: Express serves built client at `/judge` and `/play/*` (200s)

Client build: `node node_modules/vite/bin/vite.js build` (note: in this Termux shell use `node node_modules/vite/bin/vite.js` — plain `npx vite` fails with `vite: not found` because the PATH the shell tool uses doesn't pick up `node_modules/.bin`).

---

## How to run / test locally
See `RUN-GUIDE.md`. Quick:
```bash
npm --prefix server run dev   # :3001
npm --prefix client run dev   # :5173  (proxies sockets)
node node_modules/vite/bin/vite.js build   # prod build → client/dist
NODE_ENV=production npm --prefix server start  # self-serves API + built client
```
Multi-device test on one machine: `/judge` tab + several `/play/CODE` tabs.

---

## Deployment (next logical step)

Single-deploy path (simplest): backend serves the built client.
- Build client, set `NODE_ENV=production`, deploy `server/` to Render/Railway/Fly.io (needs `npm start`; reads `PORT`).
- If hosting the client separately (Vercel/Netlify from `client/`), set `VITE_SERVER_URL=<backend url>` and keep CORS `origin:'*'`.
- **Untested:** the PRD's "not same WiFi" requirement. Verify after deploying by joining from WiFi + mobile data.

---

## Known limitations / future work

1. **Not deployed yet** — no public URL; can't test cross-network requirement (PRD §7 step 6).
2. **`server:playerAnswered`/`server:playerList` also reach players** (they're sent to `room:CODE`). Harmless (players ignore them) but slightly wasteful — could route judge-only events to a `judges:CODE` room.
3. **Rapid fire + `timeLimitSec`:** if no one answers correctly within 20s, judge must manually reveal. Timer auto-locks but does NOT auto-finish the round.
4. **Timer drift:** client countdown is cosmetic; the server is authoritative. On reconnect the question is re-sent with a fresh full countdown — acceptable, but a `deadline`-based timestamp would be more correct.
5. **Question order** is shuffled per session; **option order is static**. `correctIndex` leaks neither to players nor to the judge's player-broadcast.
6. **Single-question-at-a-time; no full-round question list UI** on the judge — the judge can only step forwards (Next). No Previous button.
7. **No persistence across server restarts** (by design; PRD §3). All sessions live in the `sessions` Map.
8. **Sessions are never garbage-collected** when a judge just disconnects (only closed via `host:newSession`). Long-running server accumulates stale rooms.
9. **Name-based reconnect collision:** two people using the same name merge into one player. Known per PRD §8 trade-off.
10. **`server:timeUp` reaches players but the player screen doesn't auto-advance** — it waits for `server:revealResult`. Fine, but a stuck-unanswered player could be frozen if the judge never reveals.

## Rules / conventions (respect these when continuing)

- ESM everywhere (`"type": "module"`). Socket payloads default to `{}` so undefined emits don't throw.
- Never trust client data for correctness/ordering — server computes `correct` and uses server `receivedMs`.
- `correctIndex` must ONLY be sent to the judge socket (and on `host:joinSession`), and the player broadcast MUST stay on `players:CODE`.
- Keep player scores keyed by player name for reconnect; store `socketId` for liveness.
- Restart server before running socket tests (state is in-memory).

---

## File map

```
fly-fott-quiz/
├── package.json          # root convenience scripts (install:all, dev:*, build, start)
├── .gitignore            # node_modules/, dist/, .DS_Store, *.log
├── prd.md                # original requirements (source of truth for spec)
├── RUN-GUIDE.md          # how to run + gameplay + deploy steps
├── CONTEXT.md            # this file
├── IMG-20260813-WA0000.jpg  # source logo image (school quiz logo)
├── server/
│   ├── index.js          # sockets, game state, scoring, timer, static serving
│   ├── questions.js      # pools + point ladders
│   └── package.json      # express, cors, socket.io
└── client/
    ├── vite.config.js    # dev proxy
    ├── index.html
    ├── public/
    │   └── logo.jpg      # app logo (copy of IMG-20260813-WA0000.jpg)
    └── src/
        ├── main.jsx      # router: /judge, /play/:roomCode, * → /judge
        ├── socket.js     # socket singleton + reset
        ├── styles.css    # theme, components, responsive, reduced-motion
        └── pages/
            ├── Judge.jsx
            └── Player.jsx
```
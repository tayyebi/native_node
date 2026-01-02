# Task: Web Framework & Triggers (Drogon integration, TriggerBuilder)

Objective
- Integrate an optional web framework (Drogon or Oat++) and implement the request dispatcher to route HTTP to JIT `doGet`/`doPost` handlers. Add a trigger scheduler integration (libcron).

Scope
- Add CMake option `-DWEB_FRAMEWORK=DROGON` to enable Drogon builds.
- Implement a Dispatcher that maps route ➜ compiled script handler.
- Add a `triggers/` scheduler integration and an example TriggerBuilder API.

Inputs
- `RESEARCH.md`, `src/web/*`, `src/triggers/*`, `scripts/webapps/`.

Deliverables
- Example controller `src/web/controllers/ScriptDispatcher` and a sample webapp in `scripts/webapps/`.
- Tests that exercise `/api/status` and an example script endpoint (integration test).

Acceptance Criteria
- When built with `DROGON`, the server exposes a sample endpoint that invokes a compiled snippet and returns JSON.
- Trigger scheduler can register a sample job and run it in test mode.

Notes
- Keep the initial dispatcher minimal; focus on correctness and testability rather than feature completeness.
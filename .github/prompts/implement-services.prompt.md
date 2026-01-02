# Task: Implement Services (SQLite pool, MailApp)

Objective
- Implement core services required by scripts: a SQLite connection pool with WAL and a minimal MailApp sender.

Scope
- Add `services/sqlite_pool.cpp` (WAL, prepared statements, simple pool API).
- Add `services/mail_sender.cpp` using `libcurl`'s SMTP or a minimal SMTP sender (no Boost dependency initially).
- Implement `PropertyStore` backed by SQLite.

Inputs
- `REPORT.md` (services TODOs), `src/services/*`.

Deliverables
- `src/services/sqlite_pool.cpp`, `src/services/mail_sender.cpp`, tests under `tests/` that verify WAL mode and simple email send (may mock network or use a local SMTP test server).

Acceptance Criteria
- Connection pool supports at least concurrent readers and one writer.
- Mail sender can send a message to a configurable SMTP relay (mocked in CI) and expose a simple API `services::send_mail()`.

Notes
- Prefer small dependencies; revisit `mailio` and Boost if feature needs grow.
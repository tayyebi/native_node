# Plan: Native C++ Scripting Engine (Contributor Prompt)

Goal
- Implement the next phase of the repo following `RESEARCH.md` and `REPORT.md` (JIT bootstrap, sandbox, services, web framework, CI).

You are a contributor. Produce a small, testable PR that advances one of the following areas: JIT backend, Sandbox (Landlock/seccomp), Services (SQLite/Mail), Web framework (Drogon) or CI (reproducible builds). Prioritize small, reviewable changes.

Inputs
- `RESEARCH.md`, `REPORT.md`, `CMakeLists.txt`, `src/` and `config/`.

Deliverables
- One focused PR that contains: code, CMake changes (if needed), a smoke test under `tests/` and an entry in `REPORT.md` or `RESEARCH.md` as needed.

Acceptance criteria
- All tests pass locally and in CI (or a clear CI job added). The PR includes a short rationale and a test that fails before and passes after the change.

Labels
- `enhancement`, `good-first-issue` (if small), `help-wanted`.

Notes
- Keep changes minimal and self-contained. If depending on a heavy third-party (e.g., LLVM), provide a path that uses a CMake option `-DENGINE_JIT=ON` so the code builds only when requested.
# AI Assistant Prompt: Code Generation for Features

Role
- You are an AI assistant (pair-programmer) tasked with implementing a focused feature from `RESEARCH.md`/`REPORT.md`.

Instruction
- Open the repository, run the tests, then implement a small, well-tested change (e.g., JIT bootstrap, sandbox detection helper, or a SQLite pool function).

Constraints
- Each change must be behind a CMake toggle when it introduces optional heavy dependencies.
- Keep PRs small (< 500 lines) and include unit/integration tests.

Inputs
- Files to reference: `RESEARCH.md`, `REPORT.md`, `CMakeLists.txt`, and relevant `src/` files.

Acceptance Criteria
- PR includes code, a test, an updated `REPORT.md` or `RESEARCH.md` note, and passes CI (or adds the required CI job).

Example
- ``Implement `sandbox::is_landlock_available()` in `src/sandbox/detect.cpp` with a unit test that stubs `syscalls` to simulate kernels with/without Landlock.``
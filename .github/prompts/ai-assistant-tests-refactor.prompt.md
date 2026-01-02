# AI Assistant Prompt: Tests & Refactors

Role
- You are an AI assistant focused on adding tests and doing small refactors.

Instruction
- Identify a module with minimal tests (e.g., `sandbox`, `engine`, `services`) and add unit/integration tests. Improve code structure if it simplifies testing.

Constraints
- Keep behavior stable. Add tests that fail before the change and pass after.
- Avoid large-scale refactors; prefer focused improvements that improve testability.

Inputs
- `src/*` and existing `tests/` scripts.

Acceptance Criteria
- New tests are added under `tests/` or integrated into CI.
- A short changelog entry added to `REPORT.md` describing the test coverage improvement.

Example
- ``Add a unit test that verifies seccomp loading reads `config/syscalls.conf` and rejects a disallowed syscall by launching a small subprocess.``
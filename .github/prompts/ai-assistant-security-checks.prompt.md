# AI Assistant Prompt: Security Checks & Threat Modeling

Role
- You are an AI assistant conducting a focused security review and writing tests to validate sandbox enforcement.

Instruction
- Review `RESEARCH.md` and `config/syscalls.conf`. Produce a checklist and implement a test that demonstrates that an attempt to run a forbidden syscall (e.g., `execve`) fails under the sandbox.

Constraints
- Tests must be safe to run in CI (use mocks or controlled subprocesses).

Inputs
- `RESEARCH.md`, `REPORT.md`, `config/syscalls.conf`, `src/sandbox/*`.

Acceptance Criteria
- A `SECURITY_AUDIT.md` checklist is created and a test is added that asserts sandbox enforcements block a forbidden action.
- Document the recommended kernel policy (Landlock hard requirement vs fallback) and why.

Example
- ``Add a test that runs a small program under the sandbox and verifies that `execve` returns -EPERM or is otherwise terminated by the kernel.``
# Task: Security Audit & Threat Model (Landlock, seccomp, quotas)

Objective
- Create a security audit prompt for reviewers to validate sandbox design, syscall policies, cgroups limits and threat models.

Scope
- Produce an audit checklist and a set of tests/benchmarks verifying the sandbox constraints.
- Decide whether Landlock should be a hard requirement or support a strict fallback mode.

Inputs
- `RESEARCH.md`, `REPORT.md`, `config/syscalls.conf`, `src/sandbox/*`.

Deliverables
- `SECURITY_AUDIT.md` (checklist + test suggestions) and a small test harness that validates policy enforcement in the sandbox.
- A documented recommendation: Landlock hard requirement vs graceful fallback and the reasoning.

Acceptance Criteria
- Audit checklist covers filesystem, syscalls, cgroups, and network egress controls.
- Tests demonstrate that disallowed syscalls result in immediate termination in the sandboxed process.

Notes
- Ensure audit includes logging and diagnostics to help admins debug sandbox failures.
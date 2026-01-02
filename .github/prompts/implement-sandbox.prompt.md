# Task: Implement Sandbox (Landlock, Seccomp, cgroups)

Objective
- Implement runtime sandbox helpers and a conservative fallback for kernels without Landlock.

Scope
- Add Landlock detection helper `sandbox::is_landlock_available()`.
- Implement `RulesetBuilder` to read a YAML/JSON policy and apply Landlock rules per process.
- Add seccomp loader that reads `config/syscalls.conf` and applies a whitelist.
- Add basic cgroups v2 integration to place script executions into ephemeral cgroups with CPU/memory limits.

Inputs
- `RESEARCH.md` (sandbox design), `config/syscalls.conf`, `src/sandbox/*`.

Deliverables
- Implementations in `src/sandbox/` (`detect.cpp`, `ruleset_builder.cpp`, `seccomp_loader.cpp`).
- Unit tests that run on kernels with/without Landlock (tests should skip gracefully when kernel lacks features).
- Documentation in `config/` and `RESEARCH.md` describing behaviour and fallback policy.

Acceptance Criteria
- `sandbox::is_landlock_available()` returns a deterministic boolean on startup.
- Configuration-driven rules can be applied and revoked in tests.
- Tests validate seccomp rules are loaded and that a restricted process cannot perform disallowed syscalls.

Notes
- If kernel Landlock is unavailable, default to strict seccomp policy and document the degradation clearly. Aim for auditable logs.
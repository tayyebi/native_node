# Task: Implement JIT Backend (Cling vs ClangREPL evaluation)

Objective
- Add a minimal JIT harness that can compile & execute a tiny snippet at runtime and add a CI smoke test.

Scope
- Evaluate `Cling` vs `ClangREPL` (tradeoffs: build complexity, binary size, maintainability).
- Implement a minimal harness in `src/engine/jit_bootstrap.cpp` guarded by CMake option `-DENGINE_JIT=ON`.

Inputs
- `RESEARCH.md` (JIT notes), `src/engine/*`, `CMakeLists.txt`.

Deliverables
- `src/engine/jit_bootstrap.cpp` implementing a function `engine::run_snippet(const std::string&)` and a minimal example that prints `hello`.
- CMake option `ENGINE_JIT` + an associated test target `engine_jit_smoke` that runs during CI when enabled.
- A short benchmark (micro) that measures startup and reports to test logs (goal: <100ms warm start on developer hardware; document hardware).

Acceptance Criteria
- `cmake -DENGINE_JIT=ON` builds successfully inside the dev image.
- The test `engine_jit_smoke` runs and prints `hello` (or similar) and is included in CI when `ENGINE_JIT=ON`.

Notes
- Keep the harness small and instrumented for later expansion. Prefer `ClangREPL` for maintainability unless `Cling` has decisive advantages.
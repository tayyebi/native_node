# Contributing

Thanks for wanting to contribute to native_node — we welcome bug fixes, features, docs, and tests.

Getting started
- Use the Docker dev image for reproducible builds: `./scripts/dev_setup.sh build` then `./scripts/dev_setup.sh shell`.
- Local build: `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build -j $(nproc)`

Branching & commits
- Create feature branches: `feat/<short-desc>` for new features, `fix/<short-desc>` for bug fixes, `chore/<short-desc>` for maintenance.
- Write clear commit messages: short summary (50 chars), blank line, body with details.

Pull requests
- Open PRs against `main` and include a short description, testing steps, and any breaking changes.
- Add tests for new functionality when possible (unit or integration scripts under `tests/`).

Code style
- C++: target C++20. Keep code simple and documented. Follow existing repository style.
- Add `clang-format` if needed; prefer `LLVM` or project convention.

- Testing & CI
- Use `scripts/dev_setup.sh build-run` to run the full build inside the dev image.
- Note: This project requires kernel >= 5.13 for Landlock support (hard requirement) and the JIT backend uses **ClangREPL**. Ensure `clang-repl` is available in your dev environment for JIT smoke tests to pass.
- Add tests under `tests/` (shell scripts are acceptable for integration smoke tests). Update `.github/workflows/ci.yml` or add a new CI workflow for reproducible builds.

Third-party dependencies
- Prefer `FetchContent` or git submodules under `third_party/`. Add a short note in the PR describing how deps were added.

Design discussions
- For larger design changes (JIT backend, Landlock policy), open an issue to discuss before starting work.

Thanks — maintainers will review PRs and provide feedback.

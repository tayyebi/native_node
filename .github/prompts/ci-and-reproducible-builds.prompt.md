# Task: CI & Reproducible Builds (Docker dev image, split CI jobs)

Objective
- Improve CI to build dependencies and the core separately, and provide a reproducible Docker dev image for local builds (already started with `Dockerfile.dev`).

Scope
- Add CI jobs: deps-build, core-build, run-tests; add a job that runs inside `Dockerfile.dev` image.
- Document reproducible build steps and add a script to measure final binary size and LTO effectiveness.

Inputs
- `Dockerfile.dev`, `scripts/dev_setup.sh`, `.github/workflows/ci.yml`.

Deliverables
- Updated `.github/workflows/ci.yml` with separate jobs and matrix where appropriate.
- A `scripts/ci_build.sh` helper that runs inside Docker and attempts `cmake` + `ninja` LTO flow.
- Documentation in `RESEARCH.md` and `REPORT.md` describing the reproducible build process.

Acceptance Criteria
- CI shows separate job statuses for deps and core builds.
- The Docker dev image can be used locally to reproduce CI build with matching results.

Notes
- Keep the CI changes incremental; ensure that failing build artifacts expose logs and are debuggable.
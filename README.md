# native_node — Native C++ Scripting Engine

This repository contains an experimental native C++ scripting engine and serverless runtime.

Goals:
- JIT compilation via LLVM/ClangREPL
	- Note: This project requires kernel >= 5.13 for Landlock support (hard requirement) and uses ClangREPL as the initial JIT backend (stubbed).
- Kernel-level sandboxing (Landlock + seccomp)
- Integrated services: SQLite, MailApp
- Single static binary distribution using musl

See `research.md` for design and architecture notes.

Building with musl (static binary)
----------------------------------

To build a static musl-linked binary (recommended for single-binary distribution) you need a musl toolchain installed (e.g., `musl-tools` or an appropriate cross toolchain). Example:

```bash
# Create a build dir and enable static musl linking
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DMUSL_STATIC=ON
cmake --build build -j $(nproc)
```

Note: Static linking may require additional system packages (libssl, zlib, etc.) for static variants; use the provided `Dockerfile.dev` to create a reproducible musl-based build image.

Running the JIT smoke test
-------------------------

After building, run the CTest smoke test to validate JIT bootstrap (stub/ClangREPL detection):

```bash
cd build
ctest -R jit_smoke_test -V
```

Landlock policy (admins)
-------------------------

This project requires Linux kernel >= 5.13 (Landlock). Place a policy file at `config/landlock_policy.conf` or use `config/landlock_policy.example.conf` as a starting point. The format is one entry per line:

	<path> <ro|rw>

Examples:

- `./scripts rw`  — allow read/write access to the `scripts` directory
- `/tmp ro`       — allow read-only access to `/tmp`

The runtime will automatically attempt to load `config/landlock_policy.conf` at startup and apply the configured per-path rules. If Landlock isn't available on the host, the process will abort startup (Landlock is a hard requirement).

Seccomp whitelist (admins)
--------------------------

The engine can load a seccomp whitelist from `config/syscalls.conf`. This file is a simple newline-separated list of syscall names (comments start with `#`). A sample file is included at `config/syscalls.example.conf`. Keep the whitelist as small as possible — only include syscalls that are required by your scripts and platform. When `libseccomp` is present in the build, the runtime will apply the whitelist during startup after Landlock rules are applied.

Example usage:

- `cp config/syscalls.example.conf config/syscalls.conf`
- edit to remove unnecessary syscalls
- start the service: `./build/native_node`

If seccomp cannot be applied (missing `libseccomp` or runtime failure), the service will log a clear error and refuse to start in order to maintain the hard security posture.


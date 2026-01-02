# native_node — Native C++ Scripting Engine

This repository contains an experimental native C++ scripting engine and serverless runtime.

Goals:
- JIT compilation via LLVM/Cling
	- Note: This project now requires kernel >= 5.13 for Landlock support and uses Cling as the initial JIT backend (stubbed).
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

After building, run the CTest smoke test to validate JIT bootstrap (stub/Cling detection):

```bash
cd build
ctest -R jit_smoke_test -V
```


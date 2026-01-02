#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_BIN="$ROOT_DIR/build/native_node"

if [ ! -x "$BUILD_BIN" ]; then
  echo "Error: built binary not found at $BUILD_BIN" >&2
  exit 1
fi

echo "Running JIT smoke test..."
"$BUILD_BIN" --jit-smoke
rc=$?
if [ $rc -ne 0 ]; then
  echo "JIT smoke test failed (rc=$rc)" >&2
  exit $rc
fi

echo "JIT smoke test passed."

#!/usr/bin/env bash
set -euo pipefail

# Helper: build and run the Docker dev image with project mounted
IMAGE_NAME="native_node:dev"

function usage() {
  echo "Usage: $0 [build|shell|build-run]"
  exit 1
}

case ${1:-} in
  build)
    docker build -t "$IMAGE_NAME" -f Dockerfile.dev .
    ;;
  shell)
    docker run --rm -it -v "$PWD":/work -w /work "$IMAGE_NAME" /bin/bash
    ;;
  build-run)
    docker build -t "$IMAGE_NAME" -f Dockerfile.dev .
    docker run --rm -it -v "$PWD":/work -w /work "$IMAGE_NAME" /bin/bash -lc "cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build -j \$(nproc)"
    ;;
  *)
    usage
    ;;
esac

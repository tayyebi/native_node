#!/usr/bin/env bash
set -euo pipefail

BINARY=build/native_node
PIDFILE=/tmp/native_node_test.pid
LOG=/tmp/native_node_test.log

if [ ! -x "$BINARY" ]; then
  echo "Binary $BINARY not found or not executable. Build the project first." >&2
  exit 2
fi

# Start the server in background
"$BINARY" &> "$LOG" &
PID=$!
echo $PID > "$PIDFILE"

trap 'kill $PID; rm -f "$PIDFILE"' EXIT

# Wait for it to start
for i in {1..10}; do
  if curl -sSf http://localhost:8081/api/status > /dev/null 2>&1; then
    break
  fi
  sleep 1
done

RESP=$(curl -sS http://localhost:8081/api/status)

echo "Received: $RESP"

if [[ "$RESP" != *'"status":"ok"'* ]]; then
  echo "Status not ok" >&2
  exit 3
fi
if [[ "$RESP" != *'"engine":"ok"'* ]]; then
  echo "Engine not ok" >&2
  exit 4
fi
if [[ "$RESP" != *'"services":"ok"'* ]]; then
  echo "Services not ok" >&2
  exit 5
fi

echo "api/status smoke test succeeded"
exit 0

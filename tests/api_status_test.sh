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

trap 'kill $PID 2>/dev/null || true; rm -f "$PIDFILE"' EXIT

# Wait for it to start
for i in {1..10}; do
  # Abort early if the server process has already exited
  if ! kill -0 $PID 2>/dev/null; then
    echo "Server process exited unexpectedly. Server log:" >&2
    cat "$LOG" >&2
    exit 1
  fi
  if curl -sSf http://localhost:8081/api/status > /dev/null 2>&1; then
    break
  fi
  sleep 1
done

# Final check: server must still be alive before we curl
if ! kill -0 $PID 2>/dev/null; then
  echo "Server process exited unexpectedly. Server log:" >&2
  cat "$LOG" >&2
  exit 1
fi

RESP=$(curl -sSf http://localhost:8081/api/status) || {
  echo "curl failed — server did not respond on port 8081. Server log:" >&2
  cat "$LOG" >&2
  exit 1
}

echo "Received: $RESP"

if [[ "$RESP" != *'"status":"ok"'* ]]; then
  echo "Status not ok" >&2
  exit 3
fi
if [[ "$RESP" != *'"engine":"ok"'* ]] && [[ "$RESP" != *'"engine": "ok"'* ]]; then
  echo "Engine not ok" >&2
  exit 4
fi
if [[ "$RESP" != *'"services":"ok"'* ]] && [[ "$RESP" != *'"services": "ok"'* ]]; then
  echo "Services not ok" >&2
  exit 5
fi

echo "api/status smoke test succeeded"
exit 0

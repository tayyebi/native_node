#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/.." && pwd)
ANSIBLE_DIR="$ROOT/ansible"

echo "Running provision..."
ansible-playbook -i "$ANSIBLE_DIR/inventory.ini" "$ANSIBLE_DIR/playbooks/provision.yml"

echo "Building project..."
ansible-playbook -i "$ANSIBLE_DIR/inventory.ini" "$ANSIBLE_DIR/playbooks/build.yml"

echo "Running unit/integration tests..."
ansible-playbook -i "$ANSIBLE_DIR/inventory.ini" "$ANSIBLE_DIR/playbooks/tests.yml"

echo "Running full E2E..."
ansible-playbook -i "$ANSIBLE_DIR/inventory.ini" "$ANSIBLE_DIR/playbooks/e2e.yml"

echo "All steps completed. Artifacts collected under ./artifacts"

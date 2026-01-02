Ansible E2E playbooks for native_node
====================================

This directory contains a set of Ansible playbooks to provision a local test host, build the project, run unit/integration tests, and run a full end-to-end scenario.

Files
- inventory.ini — localhost inventory
- playbooks/provision.yml — install packages (Debian/Ubuntu focused)
- playbooks/build.yml — run CMake configure & build, run ctest
- playbooks/tests.yml — run selected ctest tests and the API status script
- playbooks/e2e.yml — full E2E: check kernel (>=5.13), start the `native_node` binary, run checks, gather artifacts

Usage examples

Run provision (requires sudo):

  ansible-playbook -i inventory.ini playbooks/provision.yml

Build and run tests (no sudo required if provisioning already done):

  ansible-playbook -i inventory.ini playbooks/build.yml
  ansible-playbook -i inventory.ini playbooks/tests.yml

Full E2E in sequence (requires kernel >=5.13 and sudo):

  ansible-playbook -i inventory.ini playbooks/provision.yml
  ansible-playbook -i inventory.ini playbooks/build.yml
  ansible-playbook -i inventory.ini playbooks/e2e.yml

Notes
- These playbooks are written to be pragmatic and Debian/Ubuntu focused; adjust package names or tasks for other distributions.
- The E2E playbook will fail if the running kernel is older than 5.13 because Landlock is a hard requirement.
- Some tests (cgroups and seccomp) require privileged execution; run on a host or container with appropriate privileges.

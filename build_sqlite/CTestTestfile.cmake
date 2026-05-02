# CMake generated Testfile for 
# Source directory: /home/runner/work/native_node/native_node
# Build directory: /home/runner/work/native_node/native_node/build_sqlite
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[jit_smoke_test]=] "/home/runner/work/native_node/native_node/build_sqlite/native_node" "--jit-smoke")
set_tests_properties([=[jit_smoke_test]=] PROPERTIES  LABELS "smoke;jit" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;105;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[landlock_test]=] "/home/runner/work/native_node/native_node/build_sqlite/landlock_test")
set_tests_properties([=[landlock_test]=] PROPERTIES  LABELS "smoke;landlock" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;115;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[landlock_policy_test]=] "/home/runner/work/native_node/native_node/build_sqlite/landlock_policy_test")
set_tests_properties([=[landlock_policy_test]=] PROPERTIES  LABELS "smoke;landlock;policy" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;124;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[sqlite_pool_test]=] "/home/runner/work/native_node/native_node/build_sqlite/sqlite_pool_test")
set_tests_properties([=[sqlite_pool_test]=] PROPERTIES  LABELS "smoke;sqlite" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;145;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[cgroups_test]=] "/home/runner/work/native_node/native_node/build_sqlite/cgroups_test")
set_tests_properties([=[cgroups_test]=] PROPERTIES  LABELS "smoke;cgroups" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;157;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[cgroups_limits_test]=] "/home/runner/work/native_node/native_node/build_sqlite/cgroups_limits_test")
set_tests_properties([=[cgroups_limits_test]=] PROPERTIES  LABELS "smoke;cgroups;limits" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;161;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[invocation_cgroup_test]=] "/home/runner/work/native_node/native_node/build_sqlite/invocation_cgroup_test")
set_tests_properties([=[invocation_cgroup_test]=] PROPERTIES  LABELS "smoke;cgroups;invocation" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;165;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")
add_test([=[executor_test]=] "/home/runner/work/native_node/native_node/build_sqlite/executor_test")
set_tests_properties([=[executor_test]=] PROPERTIES  LABELS "smoke;executor" _BACKTRACE_TRIPLES "/home/runner/work/native_node/native_node/CMakeLists.txt;169;add_test;/home/runner/work/native_node/native_node/CMakeLists.txt;0;")

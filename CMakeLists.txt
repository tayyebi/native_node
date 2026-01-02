cmake_minimum_required(VERSION 3.22)
project(native_node VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-fvisibility=hidden)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES src/engine/*.cpp src/engine/*.cxx src/engine/*.cc)
file(GLOB_RECURSE SANDBOX_SOURCES src/sandbox/*.cpp)
file(GLOB_RECURSE SERVICES_SOURCES src/services/*.cpp)
file(GLOB_RECURSE WEB_SOURCES src/web/*.cpp src/web/*.cc src/web/*.cxx)
file(GLOB_RECURSE TRIGGERS_SOURCES src/triggers/*.cpp)

set(SRC
  src/main.cpp
  ${ENGINE_SOURCES}
  ${SANDBOX_SOURCES}
  ${SERVICES_SOURCES}
  ${WEB_SOURCES}
  ${TRIGGERS_SOURCES}
  src/engine/jit_bootstrap.cpp
  src/web/simple_http.cpp
  src/sandbox/executor.cpp
  src/sandbox/invocation_cgroup.cpp
  src/sandbox/cgroups.cpp
)

add_executable(native_node ${SRC})

# Include directories
target_include_directories(native_node PRIVATE src)
target_include_directories(native_node PRIVATE src/web)

# Basic libraries
find_package(Threads REQUIRED)
target_link_libraries(native_node PRIVATE Threads::Threads)

# Attempt to find libseccomp for seccomp support
find_library(SECCOMP_LIB NAMES seccomp)
if(SECCOMP_LIB)
  message(STATUS "Found libseccomp: ${SECCOMP_LIB}")
  add_compile_definitions(USE_LIBSECCOMP=1)
  target_link_libraries(native_node PRIVATE ${SECCOMP_LIB})
else()
  message(WARNING "libseccomp not found: seccomp support will be disabled at compile time")
endif()

# Check for Landlock header using a C++-aware check to avoid try_compile errors
include(CheckIncludeFileCXX)
check_include_file_cxx("linux/landlock.h" HAVE_LANDLOCK)
if(HAVE_LANDLOCK)
  message(STATUS "Found linux/landlock.h: enabling Landlock features")
  add_compile_definitions(HAVE_LANDLOCK=1)
else()
  message(WARNING "linux/landlock.h not found: Landlock features will be disabled at compile time")
endif()

# Project options: JIT and ClangREPL
option(ENGINE_JIT "Build JIT engine support" ON)
option(USE_CLANGREPL "Use ClangREPL (LLVM) as the JIT backend" ON)
option(MUSL_STATIC "Link statically with musl (static binary)" OFF)

if(ENGINE_JIT)
  target_compile_definitions(native_node PRIVATE -DENGINE_JIT=1)
  if(USE_CLANGREPL)
    target_compile_definitions(native_node PRIVATE -DUSE_CLANGREPL=1)
    message(STATUS "ENGINE_JIT=ON, USE_CLANGREPL=ON (ClangREPL integration enabled - requires Clang/LLVM)")
    # Try to locate a clang-repl executable on PATH for quick local smoke tests
    find_program(CLANGREPL_EXECUTABLE clang-repl)
    if(CLANGREPL_EXECUTABLE)
      message(STATUS "Found clang-repl executable: ${CLANGREPL_EXECUTABLE}")
    else()
      message(WARNING "clang-repl executable not found. For full JIT integration add Clang/LLVM to your build (see docs). Smoke test will run in stub mode if clang-repl is unavailable.")
    endif()
  else()
    message(STATUS "ENGINE_JIT=ON, USE_CLANGREPL=OFF (stub JIT will be used)")
  endif()
else()
  message(STATUS "ENGINE_JIT=OFF (no JIT support will be compiled)")
endif()

# MUSL static linking (best-effort; requires an appropriate toolchain)
if(MUSL_STATIC)
  message(STATUS "MUSL_STATIC=ON: enabling -static linker flags (requires musl toolchain)")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -s")
  add_compile_options(-static)
endif()

# Link additional components (executor/cgroups/web) into the native_node binary
target_link_libraries(native_node PRIVATE ${CMAKE_DL_LIBS})

# Enable CTest and add a basic JIT smoke test that runs the binary with --jit-smoke
include(CTest)
enable_testing()

add_test(NAME jit_smoke_test COMMAND native_node --jit-smoke)
set_tests_properties(jit_smoke_test PROPERTIES LABELS "smoke;jit")

# Add a small landlock unit test when building on Linux
if(UNIX AND EXISTS "${CMAKE_SOURCE_DIR}/src/sandbox/ruleset.cpp")
  add_executable(landlock_test tests/landlock_test.cpp src/sandbox/ruleset.cpp src/sandbox/sandbox.cpp src/sandbox/seccomp.cpp)
  target_include_directories(landlock_test PRIVATE src)
  if(SECCOMP_LIB)
    target_link_libraries(landlock_test PRIVATE ${SECCOMP_LIB})
  endif()
  add_test(NAME landlock_test COMMAND landlock_test)
  set_tests_properties(landlock_test PROPERTIES LABELS "smoke;landlock")

  # Landlock policy test - uses a temp dir and only runs if Landlock is available
  add_executable(landlock_policy_test tests/landlock_policy_test.cpp src/sandbox/ruleset.cpp src/sandbox/sandbox.cpp src/sandbox/seccomp.cpp)
  target_include_directories(landlock_policy_test PRIVATE src)
  if(SECCOMP_LIB)
    target_link_libraries(landlock_policy_test PRIVATE ${SECCOMP_LIB})
  endif()
  add_test(NAME landlock_policy_test COMMAND landlock_policy_test)
  set_tests_properties(landlock_policy_test PROPERTIES LABELS "smoke;landlock;policy")
  
  # Add seccomp test if libseccomp is available at compile time
  if(SECCOMP_LIB)
    add_executable(seccomp_test tests/seccomp_test.cpp src/sandbox/seccomp.cpp)
    target_include_directories(seccomp_test PRIVATE src)
    target_link_libraries(seccomp_test PRIVATE ${SECCOMP_LIB})
    add_test(NAME seccomp_test COMMAND seccomp_test)
    set_tests_properties(seccomp_test PROPERTIES LABELS "smoke;seccomp")
  endif()
endif()

# SQLite pool test
find_package(SQLite3)
if (SQLite3_FOUND)
  message(STATUS "Found SQLite3: ${SQLite3_LIBRARIES}")
  add_compile_definitions(HAVE_SQLITE3=1)
  add_executable(sqlite_pool_test tests/sqlite_pool_test.cpp src/services/sqlite_pool.cpp)
  target_include_directories(sqlite_pool_test PRIVATE src)
  target_link_libraries(sqlite_pool_test PRIVATE ${SQLite3_LIBRARIES})
  add_test(NAME sqlite_pool_test COMMAND sqlite_pool_test)
  set_tests_properties(sqlite_pool_test PROPERTIES LABELS "smoke;sqlite")
  target_compile_definitions(native_node PRIVATE -DHAVE_SQLITE3=1)
  target_link_libraries(native_node PRIVATE ${SQLite3_LIBRARIES})
else()
  message(WARNING "SQLite3 not found: sqlite pool tests will be skipped")
endif()

# cgroups v2 test
if(UNIX)
  add_executable(cgroups_test tests/cgroups_test.cpp src/sandbox/cgroups.cpp)
  target_include_directories(cgroups_test PRIVATE src)
  add_test(NAME cgroups_test COMMAND cgroups_test)
  set_tests_properties(cgroups_test PROPERTIES LABELS "smoke;cgroups")
  add_executable(cgroups_limits_test tests/cgroups_limits_test.cpp src/sandbox/cgroups.cpp)
  target_include_directories(cgroups_limits_test PRIVATE src)
  add_test(NAME cgroups_limits_test COMMAND cgroups_limits_test)
  set_tests_properties(cgroups_limits_test PROPERTIES LABELS "smoke;cgroups;limits")
  add_executable(invocation_cgroup_test tests/invocation_cgroup_test.cpp src/sandbox/invocation_cgroup.cpp src/sandbox/cgroups.cpp)
  target_include_directories(invocation_cgroup_test PRIVATE src)
  add_test(NAME invocation_cgroup_test COMMAND invocation_cgroup_test)
  set_tests_properties(invocation_cgroup_test PROPERTIES LABELS "smoke;cgroups;invocation")
  add_executable(executor_test tests/executor_test.cpp src/sandbox/executor.cpp src/sandbox/invocation_cgroup.cpp src/sandbox/cgroups.cpp)
  target_include_directories(executor_test PRIVATE src)
  add_test(NAME executor_test COMMAND executor_test)
  set_tests_properties(executor_test PROPERTIES LABELS "smoke;executor")
endif()


# Installation
install(TARGETS native_node RUNTIME DESTINATION bin)

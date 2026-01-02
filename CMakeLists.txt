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
)

add_executable(native_node ${SRC})

# Include directories
target_include_directories(native_node PRIVATE src)
target_include_directories(native_node PRIVATE src/web)

# Basic libraries
find_package(Threads REQUIRED)
target_link_libraries(native_node PRIVATE Threads::Threads)

# Project options: JIT and Cling
option(ENGINE_JIT "Build JIT engine support" ON)
option(USE_CLING "Use Cling (CERN) as the JIT backend" ON)
option(MUSL_STATIC "Link statically with musl (static binary)" ON)

if(ENGINE_JIT)
  target_compile_definitions(native_node PRIVATE -DENGINE_JIT=1)
  if(USE_CLING)
    target_compile_definitions(native_node PRIVATE -DUSE_CLING=1)
    message(STATUS "ENGINE_JIT=ON, USE_CLING=ON (Cling integration enabled - requires Cling/LLVM)")
    # Try to locate a cling executable on PATH for quick local smoke tests
    find_program(CLING_EXECUTABLE cling)
    if(CLING_EXECUTABLE)
      message(STATUS "Found cling executable: ${CLING_EXECUTABLE}")
    else()
      message(WARNING "Cling executable not found. For full JIT integration add Cling/LLVM to your build (see docs). Smoke test will run in stub mode if Cling is unavailable.")
    endif()
  else()
    message(STATUS "ENGINE_JIT=ON, USE_CLING=OFF (stub JIT will be used)")
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

# Enable CTest and add a basic JIT smoke test that runs the binary with --jit-smoke
include(CTest)
enable_testing()

add_test(NAME jit_smoke_test COMMAND native_node --jit-smoke)
set_tests_properties(jit_smoke_test PROPERTIES LABELS "smoke;jit")

# Add a small landlock unit test when building on Linux
if(UNIX AND EXISTS "${CMAKE_SOURCE_DIR}/src/sandbox/ruleset.cpp")
  add_executable(landlock_test tests/landlock_test.cpp src/sandbox/ruleset.cpp src/sandbox/sandbox.cpp)
  target_include_directories(landlock_test PRIVATE src)
  add_test(NAME landlock_test COMMAND landlock_test)
  set_tests_properties(landlock_test PROPERTIES LABELS "smoke;landlock")
endif()


# Installation
install(TARGETS native_node RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 3.20)
project(compiler CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Core compiler library: the actual logic, shared by the CLI and the tests ---
add_library(compiler_core
        src/lexer/lexer.cpp
        src/parser/parser.cpp
        src/compiler/compiler.cpp
        src/vm/vm.cpp
        # headers (listed for IDE visibility)
        src/lexer/token.h
        src/lexer/lexer.h
        src/ast/expr.h
        src/ast/stmt.h
        src/parser/parser.h
        src/bytecode/instruction.h
        src/compiler/compiler.h
        src/vm/vm.h
)
# PUBLIC so anything linking compiler_core (the CLI, the tests) can #include "lexer/..."
target_include_directories(compiler_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

# --- CLI driver ---
add_executable(compiler src/main.cpp)
target_link_libraries(compiler PRIVATE compiler_core)

# --- Test runner (doctest) ---
enable_testing()
add_executable(tests
        tests/test_main.cpp
        tests/unit/test_lexer.cpp
        tests/unit/test_parser.cpp
        tests/unit/test_compiler.cpp
        tests/unit/test_vm.cpp
        tests/integration/test_eval.cpp
        tests/integration/test_control_flow.cpp
)
target_link_libraries(tests PRIVATE compiler_core)
# tests/ for doctest.h, tests/support/ for helpers.h
target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/tests
        ${CMAKE_SOURCE_DIR}/tests/support)
add_test(NAME unit_tests COMMAND tests)

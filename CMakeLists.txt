cmake_minimum_required(VERSION 4.2)
project(compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Locate LLVM's CMake package (reads LLVMConfig.cmake under CMAKE_PREFIX_PATH)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


# 2. Make LLVM's headers visible
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(compiler src/main.cpp
        src/lexer/token.h
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/ast/expr.h
        src/parser/parser.h
        src/parser/parser.cpp
        src/bytecode/instruction.h)

# 3. Turn component names into concrete library targets
llvm_map_components_to_libnames(llvm_libs core support)

# 4. Link them into our binary
target_link_libraries(compiler ${llvm_libs})

cmake_minimum_required(VERSION 3.20)
project(compiler CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compiler
        src/main.cpp
        src/lexer/token.h
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/ast/expr.h
        src/ast/stmt.h
        src/parser/parser.h
        src/parser/parser.cpp
        src/bytecode/instruction.h
        src/compiler/compiler.h
        src/compiler/compiler.cpp
        src/vm/vm.h
        src/vm/vm.cpp
)

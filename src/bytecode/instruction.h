//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include <string>

enum OpCodeKind {
    Push,
    Mul,
    Add,
    Sub,
    Min,
};

struct OpCode {
    OpCodeKind kind;
    std::optional<int> value;
};

#endif //COMPILER_INSTRUCTION_H

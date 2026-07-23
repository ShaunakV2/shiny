//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <vector>
#include "../bytecode/instruction.h"
#include "../ast/expr.h"


class Compiler {
public:
    std::vector<Instruction> compile(const Expr& root);
private:
    std::vector<Instruction> code_;      // the bytecode being built up
    void compileExpr(const Expr& node);  // recursive walker (appends to code_)
};

#endif //COMPILER_COMPILER_H

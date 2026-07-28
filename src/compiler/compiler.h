//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <map>

#include "../bytecode/instruction.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"


class Compiler {
public:
    std::vector<Instruction> compile(const std::vector<std::unique_ptr<Stmt>>& statements);
private:
    std::vector<Instruction> code_;      // the bytecode being built up
    std::map<std::string, int> symbols_;
    void compileExpr(const Expr& node);  // recursive walker (appends to code_)
    void compileStmt(const Stmt& line);
};

#endif //COMPILER_COMPILER_H

//
// Created by Shaunak Vaghasia on 7/23/26.
//

#include "compiler.h"

std::vector<Instruction> Compiler::compile(const Expr &root) {
    compileExpr(root);
    return code_;
}

void Compiler::compileExpr(const Expr& node) {
    if (auto* bin = dynamic_cast<const BinaryExpr*>(&node)) {
        compileExpr(*bin->left);
        compileExpr(*bin->right);
        if (bin->op == TokenKind::Plus) {
            Compiler::code_.push_back(Instruction(InstructionKind::Add));
        }
        else if (bin->op == TokenKind::Minus) {
            Compiler::code_.push_back(Instruction(InstructionKind::Sub));
        }
        else if (bin->op == TokenKind::Slash) {
            Compiler::code_.push_back(Instruction(InstructionKind::Div));
        }
        else if (bin->op == TokenKind::Star) {
            Compiler::code_.push_back(Instruction(InstructionKind::Mul));
        }
    }
    else if (auto* unary_expr = dynamic_cast<const UnaryExpr*>(&node)) {
        compileExpr(*unary_expr->left);
        Compiler::code_.push_back(Instruction(InstructionKind::Neg));
    }
    else if (auto* integer_literal = dynamic_cast<const IntegerLiteral*>(&node)){
        Compiler::code_.push_back(Instruction(InstructionKind::Push, integer_literal->value));
    }
}



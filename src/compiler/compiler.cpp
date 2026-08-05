//
// Created by Shaunak Vaghasia on 7/23/26.
//

#include "compiler.h"

std::vector<Instruction> Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    int i =0;
    while (statements.size() > i) {
        compileStmt(*statements.at(i));
        i+=1;
    }
    return code_;
}

void Compiler::compileStmt(const Stmt &line) {
    if (auto* lt = dynamic_cast<const LetStatement*>(&line)) {
        compileExpr(*lt->value);
        const int idx = std::ssize(symbols_);
        Compiler::symbols_[lt->name] = idx;
        Compiler::code_.push_back(Instruction(InstructionKind::Store, idx));
    }
    else if (auto* exst = dynamic_cast<const ExprStatement*>(&line)) {
        compileExpr(*exst->value);
    }

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
        else if (bin->op == TokenKind::Less) {
            Compiler::code_.push_back(Instruction(InstructionKind::Lt));
        }
        else if (bin->op == TokenKind::Greater) {
            Compiler::code_.push_back(Instruction(InstructionKind::Gt));
        }
        else if (bin->op == TokenKind::LessEqual) {
            Compiler::code_.push_back(Instruction(InstructionKind::Le));
        }
        else if (bin->op == TokenKind::GreaterEqual) {
            Compiler::code_.push_back(Instruction(InstructionKind::Ge));
        }
        else if (bin->op == TokenKind::EqualEqual) {
            Compiler::code_.push_back(Instruction(InstructionKind::Eq));
        }
        else if (bin->op == TokenKind::BangEqual) {
            Compiler::code_.push_back(Instruction(InstructionKind::Ne));
        }
    }
    else if (auto* unary_expr = dynamic_cast<const UnaryExpr*>(&node)) {
        compileExpr(*unary_expr->left);
        Compiler::code_.push_back(Instruction(InstructionKind::Neg));
    }
    else if (auto* integer_literal = dynamic_cast<const IntegerLiteral*>(&node)){
        Compiler::code_.push_back(Instruction(InstructionKind::Push, integer_literal->value));
    }
    else if (auto* variable_expr = dynamic_cast<const VariableExpr*>(&node)) {
        int var_loc = Compiler::symbols_[variable_expr->name];
        Compiler::code_.push_back(Instruction(InstructionKind::Load, var_loc));
    }
}



//
// Created by Shaunak Vaghasia on 7/20/26.
//

#ifndef COMPILER_EXPR_H
#define COMPILER_EXPR_H

#include <memory>
#include <ostream>

#include "../lexer/token.h"

inline const char* opSymbol(TokenKind op) {
    switch (op) {
        case TokenKind::Plus:  return "+";
        case TokenKind::Minus: return "-";
        case TokenKind::Star:  return "*";
        case TokenKind::Slash: return "/";
        default:               return "?";
    }
}

// Base class for all expression nodes.
struct Expr {
    virtual ~Expr() = default;
    virtual void print(std::ostream& os) const = 0;
};

// A leaf: an integer literal like 42.
struct IntegerLiteral : Expr {
    int value;
    explicit IntegerLiteral(int v) : value(v) {}
    void print(std::ostream &os) const override {
        os << value;
    }
};

// An internal node: left <op> right.
struct BinaryExpr : Expr {
    TokenKind op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    void print(std::ostream &os) const override {
        os<< "(" << opSymbol(op) << " ";
        left->print(os);
        os << " ";
        right->print(os);
        os << ")";
    }

    BinaryExpr(TokenKind op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
};

struct UnaryExpr : Expr {
    TokenKind op;
    std::unique_ptr<Expr> left;

    void print(std::ostream &os) const override {
        os << "-";
        left ->print(os);
    }

    UnaryExpr(TokenKind op, std::unique_ptr<Expr> left): op(op), left(std::move(left)){}
};

#endif // COMPILER_EXPR_H

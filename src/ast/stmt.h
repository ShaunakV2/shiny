//
// Created by Shaunak Vaghasia on 7/27/26.
//

#ifndef COMPILER_STMT_H
#define COMPILER_STMT_H
#include <iosfwd>
#include <utility>

#include "expr.h"

struct Stmt {
    virtual ~Stmt() = default;
    virtual void print(std::ostream&) const = 0;
};

struct LetStatement:Stmt {
    std::unique_ptr<Expr> value;
    std::string name;
    void print(std::ostream &os) const override {
        os << "(let " << name << " = ";
        value->print(os);
        os << ")";
    }
    explicit LetStatement(std::unique_ptr<Expr> v, std::string n)
    : value(std::move(v)), name(std::move(n)) {}
};

struct ExprStatement:Stmt {
    std::unique_ptr<Expr> value;
    void print(std::ostream &os) const override {
        os << "(";
        value->print(os);
        os << ")";
    }
    explicit ExprStatement(std::unique_ptr<Expr> v): value(std::move(v)){}
};

#endif //COMPILER_STMT_H

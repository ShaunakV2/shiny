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
    std::size_t offset;
    void print(std::ostream &os) const override {
        os << "(let " << name << " = ";
        value->print(os);
        os << ")";
    }
    explicit LetStatement(std::unique_ptr<Expr> v, std::string n, std::size_t o)
    : value(std::move(v)), name(std::move(n)), offset(o) {}
};

struct AssignStatement:Stmt {
    std::unique_ptr<Expr> value;
    std::string name;
    std::size_t offset;
    void print(std::ostream &os) const override {
        os << "(assign " << name << " = ";
        value->print(os);
        os << ")";
    }
    explicit AssignStatement(std::unique_ptr<Expr> v, std::string n, std::size_t o)
    : value(std::move(v)), name(std::move(n)), offset(o) {}
};


struct ExprStatement:Stmt {
    std::unique_ptr<Expr> value;
    void print(std::ostream &os) const override {
        value->print(os);
    }
    explicit ExprStatement(std::unique_ptr<Expr> v): value(std::move(v)){}
};

struct BlockStatement:Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    void print(std::ostream& os) const override {
        os << "(block";
        for (const std::unique_ptr<Stmt>& s : statements) {
            os << " ";
            s->print(os);       // recurse into each inner statement
        }
        os << ")";
    }
    explicit BlockStatement(std::vector<std::unique_ptr<Stmt>> s) : statements(std::move(s)){}
};

struct IfStatement:Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt>  thenBranch;
    std::unique_ptr<Stmt> elseBranch;
    void print(std::ostream& os) const override {
        os << "(if ";
        condition->print(os);
        os << " ";
        thenBranch->print(os);
        if (elseBranch) {
            os << " ";
            elseBranch->print(os);
        }
        os << ")";
    }
    explicit IfStatement(std::unique_ptr<Expr> c, std::unique_ptr<Stmt>  tb, std::unique_ptr<Stmt>  eb): condition(std::move(c)), thenBranch(std::move(tb)), elseBranch(std::move(eb)){}
};

struct WhileStatement:Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt>  body;
    void print(std::ostream& os) const override {
        os << "(while ";
        condition->print(os);
        os << " ";
        body->print(os);
        os << ")";
    }
    explicit WhileStatement(std::unique_ptr<Expr> c, std::unique_ptr<Stmt>  b): condition(std::move(c)), body(std::move(b)){}
};

struct PrintStatement:Stmt {
    std::unique_ptr<Expr> value;
    void print(std::ostream& os) const override {
        os << "(print ";
        value->print(os);
        os << ")";
    }
    explicit PrintStatement(std::unique_ptr<Expr> v): value(std::move(v)){}
};

#endif //COMPILER_STMT_H

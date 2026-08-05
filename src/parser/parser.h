//
// Created by Shaunak Vaghasia on 7/20/26.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H


#include <memory>
#include <vector>
#include "../lexer/token.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    // Entry point: parse a full expression, expect EndOfFile after.
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens_;
    std::size_t pos_ = 0;

    // --- token cursor helpers ---
    const Token& peek() const;          // current token, no consume
    const Token& advance();             // return current, then move forward
    bool check(TokenKind kind) const;   // is current token this kind?
    bool match(TokenKind kind);         // if current is `kind`, consume it & return true

    std::unique_ptr<Expr> parseExpression();

    std::unique_ptr<Expr> parseEquality();
    std::unique_ptr<Expr> parseComparison();

    std::unique_ptr<Expr> parseAdditive();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Expr> parseUnary();

    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseLetStatement();
    std::unique_ptr<Stmt> parseExprStatement();
};

#endif //COMPILER_PARSER_H

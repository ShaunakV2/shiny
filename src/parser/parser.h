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
    /// return current token, doesn't consume.
    const Token& peek() const;

    /// returns currToken + 1, doesn't consume.
    const Token& peekNext() const;

    /// returns currToken and consumes (advances)
    const Token& advance();

    /// check expected token against what it actually finds, context is plain english we give it based on the actual context of the function where it is being called
    /// e.g. after let we expect an identifier, therefore we would pass (TokenKind::Identifier, "after 'let'")
    /// returns compileError if tokenKind is not what is expected
    const Token& expect(TokenKind kind, const std::string& context);

    /// check if currToken is the same as kind provided
    bool check(TokenKind kind) const;


    std::unique_ptr<Expr> parseExpression();

    std::unique_ptr<Expr> parseEquality();
    std::unique_ptr<Expr> parseComparison();

    std::unique_ptr<Expr> parseAdditive();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Expr> parseUnary();

    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseLetStatement();
    std::unique_ptr<Stmt> parseAssignStatement();
    std::unique_ptr<Stmt> parseExprStatement();

    std::unique_ptr<Stmt> parseBlock();
    std::unique_ptr<Stmt> parseIfStatement();
    std::unique_ptr<Stmt> parseWhileStatement();
    std::unique_ptr<Stmt> parsePrintStatement();
    std::unique_ptr<Stmt> parseReturnStatement();
};

#endif //COMPILER_PARSER_H

//
// Created by Shaunak Vaghasia on 7/20/26.
//

#include "parser.h"

#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)){};

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenKind::EndOfFile)) {
        statements.push_back(parseStatement());
    }
    return statements;
}

const Token& Parser::peek() const {
    return tokens_[pos_];
}

const Token &Parser::advance() {
    return tokens_[pos_++];
}

bool Parser::check(TokenKind kind) const{
    return peek().kind == kind;
}

bool Parser::match(TokenKind kind) {
    if (check(kind)) {
        advance();
        return true;
    }
    return false;
}

std::unique_ptr<Stmt> Parser::parseLetStatement() {
    // Consume the 'Let' Token
    advance();
    // The next token is the identifier
    if (!check(TokenKind::Identifier)) return nullptr;
    const std::string identifier = advance().name;
    // Consume the Assign
    if (!check(TokenKind::Assign)) return nullptr;
    advance();
    auto initializer =  parseExpression();
    if (!check(TokenKind::Semicolon)) return nullptr;
    advance();
    return std::make_unique<LetStatement>(std::move(initializer), identifier);
}

std::unique_ptr<Stmt> Parser::parseExprStatement() {
    auto initializer = parseExpression();
    if (!check(TokenKind::Semicolon)) return nullptr;
    advance();
    return std::make_unique<ExprStatement>(std::move(initializer));
}


std::unique_ptr<Stmt> Parser::parseStatement() {
    // If the first token is let we know that the following will be a let Statement
    if (check(TokenKind::Let)) {
        return parseLetStatement();
    }
    else {
        return parseExprStatement();
    }
}

std::unique_ptr<Expr> Parser::parseExpression() {
    return parseEquality();
}

std::unique_ptr<Expr> Parser::parseEquality() {
    std::unique_ptr<Expr> left = parseComparison();
    while (check(TokenKind::BangEqual) || check(TokenKind::EqualEqual)) {
        const TokenKind op = advance().kind;
        std::unique_ptr<Expr> right = parseComparison();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseComparison() {
    std::unique_ptr<Expr> left = parseAdditive();
    while (check(TokenKind::Less) || check(TokenKind::Greater) ||
           check(TokenKind::LessEqual) || check(TokenKind::GreaterEqual)) {
        const TokenKind op = advance().kind;
        std::unique_ptr<Expr> right = parseAdditive();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;

}

std::unique_ptr<Expr> Parser::parseAdditive()  {
    std::unique_ptr<Expr> left = parseTerm();
    while (check(TokenKind::Plus) || check(TokenKind::Minus)) {
        const TokenKind op = advance().kind;
        std::unique_ptr<Expr> right = parseTerm();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;

}
std::unique_ptr<Expr> Parser::parseTerm()  {
    std::unique_ptr<Expr> left = parseUnary();
    while (check(TokenKind::Slash) || check(TokenKind::Star)) {
        const TokenKind op = advance().kind;
        std::unique_ptr<Expr> right = parseUnary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}
std::unique_ptr<Expr> Parser::parseFactor()  {
    if (check(TokenKind::Integer)) {
        const Token& tok = advance();
        int v  = tok.value.value();
        return std::make_unique<IntegerLiteral>(v);
    }
    else if (check(TokenKind::LParen)) {
        const Token& tok = advance();
        auto inner = parseExpression();
        match(TokenKind::RParen);
        return inner;
    }
    else if (check(TokenKind::Identifier)) {
        const std::string name = advance().name;
        return std::make_unique<VariableExpr>(name);
    }
    else {
        // TODO: Error handling for bad expression
        return nullptr;
    }

}

std::unique_ptr<Expr> Parser::parseUnary() {
    if (check(TokenKind::Minus)) {
        advance();
        std::unique_ptr<Expr> operand = parseUnary();
        return std::make_unique<UnaryExpr>(TokenKind::Minus, std::move(operand));
    }
    return parseFactor();
}




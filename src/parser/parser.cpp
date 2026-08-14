//
// Created by Shaunak Vaghasia on 7/20/26.
//

#include "parser.h"

#include <format>
#include <iostream>

#include "error/error.h"

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

const Token &Parser::peekNext() const {
    return tokens_[pos_ + 1];
}

const Token &Parser::advance() {
    return tokens_[pos_++];
}

const Token& Parser::expect(TokenKind kind, const std::string& context) {
    if (check(kind)) return advance();
    const Token& tok = peek();
    throw CompileError(
        std::format("expected {} {}, found {}",
                    tokenKindName(kind), context, tokenKindName(tok.kind)),
        tok.offset);
}

bool Parser::check(TokenKind kind) const{
    return peek().kind == kind;
}
std::unique_ptr<Stmt> Parser::parseLetStatement() {
    // Consume the 'Let' Token
    advance();
    // The next token has to be the identifier, so we use expect to make sure we get that.
    const std::string identifier =expect(TokenKind::Identifier,"after 'let'").name;
    expect(TokenKind::Assign, std::format("after '{}'", identifier)); // expect assign
    auto initializer =  parseExpression();
    expect(TokenKind::Semicolon, std::format("after declaration of '{}'", identifier)); // finally expect semicolon
    return std::make_unique<LetStatement>(std::move(initializer), identifier);
}

std::unique_ptr<Stmt> Parser::parseAssignStatement() {
    const std::size_t offset = peek().offset;
    const std::string identifier =advance().name;    // Consume the Identifier and save the name
    expect(TokenKind::Assign, std::format("after '{}'", identifier)); // expect assign and consume
    auto initializer = parseExpression();
    expect(TokenKind::Semicolon, std::format("after assignment to '{}'", identifier));
    return std::make_unique<AssignStatement>(std::move(initializer), identifier, offset);
}

std::unique_ptr<Stmt> Parser::parseExprStatement() {
    auto initializer = parseExpression();
    expect(TokenKind::Semicolon, "after expression statement");
    return std::make_unique<ExprStatement>(std::move(initializer));
}


std::unique_ptr<Stmt> Parser::parseStatement() {
    // If the first token is let we know that the following will be a let Statement
    if (check(TokenKind::LBrace)) {
        return parseBlock();
    }
    if (check(TokenKind::Let)) {
        return parseLetStatement();
    }
    if (check(TokenKind::If)) {
        return parseIfStatement();
    }
    if (check(TokenKind::While)) {
        return parseWhileStatement();
    }
    if (check(TokenKind::Print)) {
        return parsePrintStatement();
    }
    if (check(TokenKind::Identifier) && peekNext().kind == TokenKind::Assign) {
        return parseAssignStatement();
    }
    return parseExprStatement();
}

std::unique_ptr<Stmt> Parser::parseIfStatement() {
    advance(); // consume if
    expect(TokenKind::LParen, "after 'if'");
    std::unique_ptr<Expr> condition = parseExpression();
    expect(TokenKind::RParen, "after if condition");

    std::unique_ptr<Stmt> thenBranch = parseBlock();

    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (check(TokenKind::Else)) {
        advance();
        elseBranch = parseBlock();
    }
    return std::make_unique<IfStatement>(std::move(condition),
                                         std::move(thenBranch),
                                         std::move(elseBranch));

}

std::unique_ptr<Stmt> Parser::parseWhileStatement() {
    advance(); // consume while
    expect(TokenKind::LParen, "after 'while'");
    std::unique_ptr<Expr> condition = parseExpression();
    expect(TokenKind::RParen, "after while condition");

    std::unique_ptr<Stmt> body = parseBlock();

    return std::make_unique<WhileStatement>(std::move(condition),std::move(body));
}

std::unique_ptr<Stmt> Parser::parsePrintStatement() {
    advance(); // consume print
    std::unique_ptr<Expr> value = parseExpression();
    expect(TokenKind::Semicolon, "after print statement");
    return std::make_unique<PrintStatement>(std::move(value));
}

std::unique_ptr<Stmt> Parser::parseBlock() {
    expect(TokenKind::LBrace, "to open block");
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenKind::RBrace) && !check(TokenKind::EndOfFile)) {
        statements.push_back(parseStatement());
    }
    // On EOF (unterminated block) the current token isn't RBrace, so this throws.
    expect(TokenKind::RBrace, "to close block");
    return std::make_unique<BlockStatement>(std::move(statements));
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
        expect(TokenKind::RParen, "to close '('");
        return inner;
    }
    else if (check(TokenKind::Identifier)) {
        const std::size_t offset = peek().offset;
        const std::string name = advance().name;
        return std::make_unique<VariableExpr>(name, offset);
    }
    else {
        throw CompileError(
            std::format("expected an expression, found {}", tokenKindName(peek().kind)),
            peek().offset);
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




//
// Created by Shaunak Vaghasia on 7/20/26.
//

#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)){};

std::unique_ptr<Expr> Parser::parse() {
     return parseExpression();
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

std::unique_ptr<Expr> Parser::parseExpression()  {
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




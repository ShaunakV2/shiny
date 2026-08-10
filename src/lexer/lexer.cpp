//
// Created by Shaunak Vaghasia on 7/19/26.
//

#include "lexer.h"

#include <iostream>
#include <unordered_map>

#include "error/error.h"

Lexer::Lexer(std::string source): source_(std::move(source)) {
}

char Lexer::peek() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::peekAhead() const {
    if (pos_ >= source_.size() + 1) return '\0';
    return source_[pos_ + 1];
}

char Lexer::advance() {
    if (pos_ >= source_.size()) return '\0';
    char curr_char = source_[pos_];
    pos_+=1;
    return curr_char;
}

Token Lexer::readNumber() {
    std::size_t start = pos_;
    std::string val;
    while (peek() >= '0' && peek() <= '9') {
        val += advance();
    }
    try {
        return Token{TokenKind::Integer, std::stoi(val)};
    } catch (const std::out_of_range&) {
        throw CompileError("integer literal too large: " + val, start);
    }
}

void Lexer::skipWhitespace() {
    while (peek() == ' ' || peek() == '\n' || peek() == '\t' || peek() == '\r') {
        advance();
    }
}

Token Lexer::readIdentifier() {
    std::string val;
    while (std::isalpha(peek()) || std::isdigit(peek())|| peek() == '_') {
        val+=advance();
    }

    // Keyword table: built once (static), reused every call.
    static const std::unordered_map<std::string, TokenKind> keywords = {
        {"let",   TokenKind::Let},
        {"if",    TokenKind::If},
        {"else",  TokenKind::Else},
        {"while", TokenKind::While},
        {"print", TokenKind::Print},
    };

    if (auto it = keywords.find(val); it != keywords.end()) {
        return Token{.kind = it->second};
    }
    return Token{.kind = TokenKind::Identifier, .name = val};
}

Token Lexer::scanToken() {
    char c = peek();

    std::size_t start = pos_;

    // Character classes go first — they don't fit switch cases cleanly.
    if (c >= '0' && c <= '9')        return readNumber();
    if (std::isalpha(c) || c == '_') return readIdentifier();

    switch (c) {
        // Single-character tokens.
        case '+': advance(); return Token{TokenKind::Plus,      std::nullopt};
        case '-': advance(); return Token{TokenKind::Minus,     std::nullopt};
        case '*': advance(); return Token{TokenKind::Star,      std::nullopt};
        case '/': advance(); return Token{TokenKind::Slash,     std::nullopt};
        case '(': advance(); return Token{TokenKind::LParen,    std::nullopt};
        case ')': advance(); return Token{TokenKind::RParen,    std::nullopt};
        case '{': advance(); return Token{TokenKind::LBrace,    std::nullopt};
        case '}': advance(); return Token{TokenKind::RBrace,    std::nullopt};
        case ';': advance(); return Token{TokenKind::Semicolon, std::nullopt};

        // Multi-character operators: consume the first char, then peek for a trailing '='.
        case '<':
            advance();
            if (peek() == '=') { advance(); return Token{TokenKind::LessEqual}; }
            return Token{TokenKind::Less};
        case '>':
            advance();
            if (peek() == '=') { advance(); return Token{TokenKind::GreaterEqual}; }
            return Token{TokenKind::Greater};
        case '=':
            advance();
            if (peek() == '=') { advance(); return Token{TokenKind::EqualEqual}; }
            return Token{TokenKind::Assign};
        case '!':
            advance();
            if (peek() == '=') { advance(); return Token{TokenKind::BangEqual}; }
            throw CompileError("expected '=' after '!'", start);
        default:
            advance();
            throw CompileError("unexpected character '" + std::string(1, c) + "'", start);
           // return Token{TokenKind::Unknown};   // unknown char — sub-step 2 throws here
    }
}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos_ < source_.size()) {
        skipWhitespace();
        if (pos_ >= source_.size()) break;
        std::size_t start = pos_;
        Token tok = scanToken();
        tok.offset = start;
        tok.length = pos_ - start;
        tokens.push_back(tok);
    }
    tokens.push_back(Token{TokenKind::EndOfFile, std::nullopt});
    return tokens;
}

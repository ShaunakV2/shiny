//
// Created by Shaunak Vaghasia on 7/19/26.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <optional>
#include <string>


enum class TokenKind {
    Integer,
    Plus,
    Minus,
    Star,
    Slash,
    LParen,
    RParen,
    EndOfFile,
    Identifier,
    Let,
    Assign,
    Semicolon,
    Unknown,
};

// Human-readable name of a token kind (for debugging and error messages).
// No `default`: adding a TokenKind without handling it here trips -Wswitch.
inline std::string tokenKindName(TokenKind kind) {
    switch (kind) {
        case TokenKind::Integer:    return "Integer";
        case TokenKind::Plus:       return "Plus";
        case TokenKind::Minus:      return "Minus";
        case TokenKind::Star:       return "Star";
        case TokenKind::Slash:      return "Slash";
        case TokenKind::LParen:     return "LParen";
        case TokenKind::RParen:     return "RParen";
        case TokenKind::EndOfFile:  return "EndOfFile";
        case TokenKind::Identifier: return "Identifier";
        case TokenKind::Let:        return "Let";
        case TokenKind::Assign:     return "Assign";
        case TokenKind::Semicolon:  return "Semicolon";
        case TokenKind::Unknown:    return "Unknown";
    }
    return "Unknown";
}

// Source glyph for punctuation/operator tokens ("?" for tokens without one).
inline const char* tokenKindSymbol(TokenKind kind) {
    switch (kind) {
        case TokenKind::Plus:      return "+";
        case TokenKind::Minus:     return "-";
        case TokenKind::Star:      return "*";
        case TokenKind::Slash:     return "/";
        case TokenKind::Assign:    return "=";
        case TokenKind::Semicolon: return ";";
        case TokenKind::LParen:    return "(";
        case TokenKind::RParen:    return ")";
        default:                   return "?";
    }
}

struct Token {
    TokenKind kind;
    std::optional<int> value;
    std::string name;
};

#endif //COMPILER_TOKEN_H

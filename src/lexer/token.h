//
// Created by Shaunak Vaghasia on 7/19/26.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <optional>


enum class TokenKind {
    Integer,
    Plus,
    Minus,
    Star,
    Slash,
    LParen,
    RParen,
    EndOfFile,
    Unknown,
};

struct Token {
    TokenKind kind;
    std::optional<int> value;
};

#endif //COMPILER_TOKEN_H

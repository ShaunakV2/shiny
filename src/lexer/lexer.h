//
// Created by Shaunak Vaghasia on 7/19/26.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H
#include <vector>

#include "token.h"

class Lexer {
public:
    explicit Lexer(std::string source);

    std::vector<Token> tokenize();

private:
    std::string source_;     // the owned source text
    std::size_t pos_ = 0;    // cursor index, starts at 0

    char peek() const;
    char peekAhead() const;
    char advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
    Token scanToken();

};

#endif //COMPILER_LEXER_H

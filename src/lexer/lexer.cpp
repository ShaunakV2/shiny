//
// Created by Shaunak Vaghasia on 7/19/26.
//

#include "lexer.h"

#include <iostream>

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
    std::string val;
    while (peek() >= '0' && peek() <= '9') {
        val+=advance();
    }
    return Token{TokenKind::Integer, std::stoi(val)};
}

Token Lexer::readIdentifier() {
    std::string val;
    while (std::isalpha(peek()) || std::isdigit(peek())|| peek() == '_') {
        val+=advance();
    }
    if (val == "let") {
        return Token{.kind = TokenKind::Let, .name = val};
    }
    return Token{.kind = TokenKind::Identifier, .name = val};

}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos_ < source_.size()) {
        char c = peek();
        if (c >= '0' && c<= '9') {
            tokens.push_back(readNumber());
        }
        else if (std::isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
        }
        else if (c == '+') {
            tokens.push_back(Token{TokenKind::Plus, std::nullopt});
            advance();
        }
        else if (c == '-') {
            tokens.push_back(Token{TokenKind::Minus, std::nullopt});
            advance();
        }
        else if (c == '*') {
            tokens.push_back(Token{TokenKind::Star, std::nullopt});
            advance();
        }
        else if (c == '/') {
            tokens.push_back(Token{TokenKind::Slash, std::nullopt});
            advance();
        }
        else if (c == '(') {
            tokens.push_back(Token{TokenKind::LParen, std::nullopt});
            advance();
        }
        else if (c == ')') {
            tokens.push_back(Token{TokenKind::RParen, std::nullopt});
            advance();
        }
        else if (c == '>') {
            if (peekAhead() == '=') {
                tokens.push_back(Token{.kind = TokenKind::GreaterEqual});
                advance();
            }
            else {
                tokens.push_back(Token{.kind = TokenKind::Greater});
            }
            advance();
        }
        else if (c == '<') {
            if (peekAhead() == '=') {
                tokens.push_back(Token{.kind = TokenKind::LessEqual});
                advance();
            }
            else {
                tokens.push_back(Token{.kind = TokenKind::Less});
            }
            advance();
        }
        else if (c == '!') {
            if (peekAhead() == '=') {
                tokens.push_back(Token{.kind = TokenKind::BangEqual});
                advance();
            }
            advance();
        }
        else if (c == '=') {
            if (peekAhead() == '=') {
                tokens.push_back(Token{.kind = TokenKind::EqualEqual});
                advance();
            }
            else {
                tokens.push_back(Token{.kind = TokenKind::Assign});
            }
            advance();
        }
        else if (c == ';') {
            tokens.push_back(Token{.kind = TokenKind::Semicolon});
            advance();
        }
        else if (c == ' ') {
            advance();
        }
        else {
            tokens.push_back(Token{TokenKind::Unknown, std::nullopt});
            advance();
        }
    }
    tokens.push_back(Token{TokenKind::EndOfFile, std::nullopt});
    return tokens;
}

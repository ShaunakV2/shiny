#include <iostream>
#include <string>
#include "lexer/lexer.h"
#include "lexer/token.h"

// Map a TokenKind to a human-readable name (for debugging/printing).
// You'll want something like this again later for error messages.
static std::string kindToString(TokenKind kind) {
    switch (kind) {
        case TokenKind::Integer:   return "Integer";
        case TokenKind::Plus:      return "Plus";
        case TokenKind::Minus:     return "Minus";
        case TokenKind::Star:      return "Star";
        case TokenKind::Slash:     return "Slash";
        case TokenKind::LParen:    return "LParen";
        case TokenKind::RParen:    return "RParen";
        case TokenKind::EndOfFile: return "EndOfFile";
    }
    return "Unknown";
}

int main() {
    std::string source = "12 + 34 * (5 - 6)";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& tok : tokens) {
        std::cout << kindToString(tok.kind);
        // Only Integer tokens carry a value.
        if (tok.value.has_value()) {
            std::cout << "(" << tok.value.value() << ")";
        }
        std::cout << " ";
    }
    std::cout << std::endl;

    return 0;
}
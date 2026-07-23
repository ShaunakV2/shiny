#include <iostream>
#include <string>

#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

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
        case TokenKind::Unknown:
            break;
    }
    return "Unknown";
}

int main() {
    Lexer lexer("-12 + 34 * (5 - 6)");
    Parser parser(lexer.tokenize());
    std::unique_ptr<Expr> tree = parser.parse();
    tree->print(std::cout);
    std::cout << "\n";
    Compiler compiler;
    std::vector<Instruction> instructions=  compiler.compile(*tree);
    for (const auto& in : instructions) {
        std::cout << instructionKindToString(in.kind);
        if (in.value.has_value()) {
            std::cout << "(" << in.value.value() << ")";
        }
        std::cout << " ";
    }
    std::cout << "\n";
    return 0;
}
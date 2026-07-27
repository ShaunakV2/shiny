#include <iostream>
#include <string>

#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

int main() {
    // Lexer test for the new variable-related tokens.
    // Once readIdentifier + the keyword check + '=' and ';' are implemented,
    // this input should tokenize to exactly:
    //   Let Identifier(x) Assign Integer(12) Plus Identifier(y) Semicolon EndOfFile
    Lexer lexer("let x = 12 + y;");
    std::vector<Token> tokens = lexer.tokenize();
    for (const Token& tok : tokens) {
        std::cout << tokenKindName(tok.kind);
        if (tok.value.has_value()) {
            std::cout << "(" << tok.value.value() << ")";
        }
        if (!tok.name.empty()) {
            std::cout << "(" << tok.name << ")";
        }
        std::cout << " ";
    }
    std::cout << "\n";
    return 0;
}
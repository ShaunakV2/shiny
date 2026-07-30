#include <iostream>
#include <string>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

int main() {
    // Lexer test for multi-character comparison operators.
    // The "let x = 5;" prefix is the regression check: a single '=' must still be
    // Assign, NOT EqualEqual.
    // Expected token stream:
    //   Let Identifier(x) Assign Integer(5) Semicolon
    //   Identifier(x) LessEqual Integer(5) EqualEqual Identifier(y) BangEqual
    //   Integer(3) Greater Integer(1) Less Integer(2) GreaterEqual Integer(4) EndOfFile
    Lexer lexer("let x = 5; x <= 5 == y != 3 > 1 < 2 >= 4");
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
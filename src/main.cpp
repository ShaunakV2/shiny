#include <iostream>
#include <string>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

int main() {
    // Parser test: a 3-statement program exercising let-bindings and variable use.
    //   let x = 5;        → a LetStatement with an integer initializer
    //   let y = x * 2;    → a LetStatement whose initializer USES x (a VariableExpr)
    //   y + 1;            → an ExprStatement using y
    // Expected (one statement per line; exact format depends on your print methods),
    // roughly:
    //   (let x = 5)
    //   (let y = (* x 2))
    //   (+ y 1)
    Lexer lexer("let x = 5; let y = x * 2; y + 1;");
    Parser parser(lexer.tokenize());
    std::vector<std::unique_ptr<Stmt>> program = parser.parse();
    for (const std::unique_ptr<Stmt>& stmt : program) {
        stmt->print(std::cout);
        std::cout << "\n";
    }
    return 0;
}
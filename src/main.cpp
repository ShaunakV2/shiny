#include <iostream>
#include <string>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

int main() {
    // Parser test: comparison & equality precedence.
    // Expected ASTs (one per statement; exact outer parens depend on your
    // ExprStatement::print):
    //   (let x = 5)
    //   (== (< x 10) 1)              ← == is loosest, then <, so: ((x < 10) == 1)
    //   (< (+ 1 2) (* 3 4))          ← comparison looser than arithmetic
    //   (!= (> x 3) (<= 2 4))        ← mix of relational + equality
    Lexer lexer(
        "let x = 5;"
        "x < 10 == 1;"
        "1 + 2 < 3 * 4;"
        "x > 3 != 2 <= 4;"
    );
    Parser parser(lexer.tokenize());
    std::vector<std::unique_ptr<Stmt>> program = parser.parse();
    for (const std::unique_ptr<Stmt>& stmt : program) {
        stmt->print(std::cout);
        std::cout << "\n";
    }
    return 0;
}
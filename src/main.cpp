#include <iostream>
#include <string>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

// Run a source program end-to-end (lex → parse → compile → VM) and return the
// value left on top of the stack. Each test program ends in the expression
// whose result we want to check.
static int evaluate(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    std::vector<std::unique_ptr<Stmt>> program = parser.parse();
    Compiler compiler;
    std::vector<Instruction> code = compiler.compile(program);
    VM vm;
    return vm.run(code);
}

int main() {
    // End-to-end comparison evaluation (bool-as-int: 1 = true, 0 = false).
    struct Case { const char* src; int expected; };
    Case cases[] = {
        {"1 + 2 < 3 * 4;",    1},   // 3 < 12
        {"3 * 4 < 1 + 2;",    0},   // 12 < 3
        {"5 >= 5;",           1},
        {"5 > 5;",            0},
        {"2 == 2;",           1},
        {"2 != 2;",           0},
        {"1 + 2 == 3;",       1},   // arithmetic binds tighter than ==
        {"let x = 5; x > 3;", 1},   // comparison using a variable
    };

    for (const Case& c : cases) {
        int got = evaluate(c.src);
        std::cout << (got == c.expected ? "PASS" : "FAIL")
                  << "  " << c.src
                  << "  => " << got << " (expected " << c.expected << ")\n";
    }
    return 0;
}
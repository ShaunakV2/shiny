#include <iostream>
#include <string>

#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

int main() {
    Lexer lexer("-(10 + 2) * 3 - (48 / (2 + 2)) + -5 * -2");
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
    VM vm;
    int val = vm.run(instructions);
    std::cout<<val;
    return 0;
}
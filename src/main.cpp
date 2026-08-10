#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "error/error.h"
#include "error/reporter.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "vm/vm.h"

// CLI driver: compiles and runs a sample program, printing the result.
// (Automated tests live in tests/ — run the `tests` executable.)
int main() {
    const std::string source = "let x = 5; let y = x * 2; y + 1;";

    try {
        Lexer lexer(source);
        Parser parser(lexer.tokenize());
        std::vector<std::unique_ptr<Stmt>> program = parser.parse();

        Compiler compiler;
        std::vector<Instruction> code = compiler.compile(program);

        VM vm;
        int result = vm.run(code);

        std::cout << source << "\n=> " << result << "\n";
        return 0;
    } catch (const CompileError& e) {
        std::cerr << formatError(source, e) << "\n";
        return 1;
    }
}

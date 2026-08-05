#ifndef COMPILER_TEST_HELPERS_H
#define COMPILER_TEST_HELPERS_H

#include <memory>
#include <string>
#include <vector>

#include "ast/stmt.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "vm/vm.h"

// Runs a source program end-to-end (lex -> parse -> compile -> VM) and returns
// the value left on top of the stack. Test programs should end in the
// expression whose result is being checked.
inline int evaluate(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    std::vector<std::unique_ptr<Stmt>> program = parser.parse();
    Compiler compiler;
    std::vector<Instruction> code = compiler.compile(program);
    VM vm;
    return vm.run(code);
}

#endif  // COMPILER_TEST_HELPERS_H

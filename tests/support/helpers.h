#ifndef COMPILER_TEST_HELPERS_H
#define COMPILER_TEST_HELPERS_H

// Per-stage test helpers. Each function stops at one pipeline stage and returns
// its output in a form that's easy to assert on:
//
//   tokensToString    — Lexer output    (string of token kinds/values)
//   astToString       — Parser output   (string of the AST, S-expression form)
//   bytecodeToString  — Compiler output (string of the instruction stream)
//   run               — VM output       (int; runs a hand-built instruction vector)
//   evaluate          — end-to-end      (int; source -> result)

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ast/stmt.h"
#include "bytecode/instruction.h"
#include "compiler/compiler.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "vm/vm.h"

namespace test {

// ---- Stage 1: Lexer ----
inline std::vector<Token> lex(const std::string& src) {
    Lexer lexer(src);
    return lexer.tokenize();
}

// e.g. "Integer(1) Plus Integer(2) EndOfFile"
inline std::string tokensToString(const std::string& src) {
    std::ostringstream os;
    const std::vector<Token> toks = lex(src);
    for (std::size_t i = 0; i < toks.size(); ++i) {
        if (i) os << " ";
        os << tokenKindName(toks[i].kind);
        if (toks[i].value.has_value()) os << "(" << toks[i].value.value() << ")";
        if (!toks[i].name.empty())     os << "(" << toks[i].name << ")";
    }
    return os.str();
}

// ---- Stage 2: Parser ----
inline std::vector<std::unique_ptr<Stmt>> parse(const std::string& src) {
    Parser parser(lex(src));
    return parser.parse();
}

// Each statement's AST tree, space-separated. e.g. "(+ 1 (* 2 3))"
inline std::string astToString(const std::string& src) {
    std::ostringstream os;
    const std::vector<std::unique_ptr<Stmt>> program = parse(src);
    for (std::size_t i = 0; i < program.size(); ++i) {
        if (i) os << " ";
        program[i]->print(os);
    }
    return os.str();
}

// ---- Stage 3: Compiler ----
inline std::vector<Instruction> compile(const std::string& src) {
    const std::vector<std::unique_ptr<Stmt>> program = parse(src);
    Compiler compiler;
    return compiler.compile(program);
}

// e.g. "Push(5) Store(0) Load(0)"
inline std::string bytecodeToString(const std::string& src) {
    std::ostringstream os;
    const std::vector<Instruction> code = compile(src);
    for (std::size_t i = 0; i < code.size(); ++i) {
        if (i) os << " ";
        os << instructionKindToString(code[i].kind);
        if (code[i].value.has_value()) os << "(" << code[i].value.value() << ")";
    }
    return os.str();
}

// ---- Stage 4: VM ----
// Run a hand-built instruction vector — tests the VM with no compiler involved.
inline int run(const std::vector<Instruction>& code) {
    VM vm;
    return vm.run(code);
}

// ---- End to end ----
inline int evaluate(const std::string& src) {
    return run(compile(src));
}

}  // namespace test

#endif  // COMPILER_TEST_HELPERS_H

# A Hand-Written Compiler & Bytecode Virtual Machine

> **Work in progress.** A from-scratch compiler and virtual machine for a small
> programming language, built in modern **C++20** as a first-principles study of
> compiler construction — no parser generators, no interpreter frameworks, every
> stage written by hand.

Source text is tokenized, parsed into an abstract syntax tree, compiled to a
custom bytecode, and executed on a stack-based virtual machine — the full
front-to-back pipeline of a real language implementation.

> _The language is currently unnamed — rename the project/title as you like._
>
> _The same VM also runs on an ESP32 microcontroller, driving real hardware. That
> lives in a separate [`firmware/`](firmware/) project so the language core stays
> platform-independent — see [`firmware/README.md`](firmware/README.md)._

## Pipeline

```
Source code
    │
    ▼   Lexer              hand-written scanner  →  tokens
    ▼   Parser             recursive descent     →  AST
    ▼   AST                expression / statement node tree
    ▼   Compiler           tree walk + symbol table  →  bytecode
    ▼   Bytecode           flat stack-machine instructions
    ▼   Virtual Machine    executes bytecode     →  result
```

## What works today

- **Integer arithmetic** — `+ - * /`, parentheses, and unary minus, with correct
  operator precedence and associativity.
- **Variables** — `let` bindings plus reassignment (`x = ...`), read back and reused.
- **Comparisons** — `< > <= >= == !=`, with booleans represented as integers (`1`/`0`).
- **Control flow** — `if` / `else`, `while` loops, and `{ }` blocks, compiled to
  jump instructions over a runtime program counter. (The language is
  Turing-complete.)
- **Output** — a `print` statement.
- **Error reporting** — the lexer, parser, and semantic checker report malformed
  input as diagnostics with a source `line:column` and a caret under the offending
  token, rather than crashing or failing silently.
- **Semantic analysis** — a compile-time **symbol table** resolves each variable
  name to a numbered storage slot (and rejects undeclared or redeclared
  variables), so no variable names survive into the runtime.
- **Custom bytecode + stack VM** — a compact instruction set (`Push`, `Add`,
  `Sub`, `Mul`, `Div`, `Neg`, `Load`, `Store`, the six comparisons, `Jump`,
  `JumpIfFalse`, `Print`, `CallNative`) executed against a value stack and
  slot-indexed variable storage.
- **Bytecode serialization** — a program compiles to a compact, self-describing
  byte format and back, so compiled code can be stored or shipped over a wire.

### Example

```
let i = 0;
while (i < 3) {
    print i;
    i = i + 1;
}
```

prints `0`, `1`, `2`. And an arithmetic program like

```
let x = 5;
let y = x * 2;
y + 1
```

compiles to the bytecode

```
Push 5   Store 0   Load 0   Push 2   Mul   Store 1   Load 1   Push 1   Add
```

and evaluates to `11`.

## Roadmap

- [x] Lexer, recursive-descent parser, AST
- [x] Bytecode compiler + stack VM (arithmetic)
- [x] Variables (`let`, assignment) with symbol-table name resolution
- [x] Comparisons & booleans (`< > <= >= == !=`)
- [x] Control flow (`if` / `else` / `while`) — jumps and a program counter
- [x] `print` / output statements
- [x] Bytecode serialization (program ⇄ bytes)
- [x] Error handling & diagnostics (lexer, parser, semantic) with source locations
- [ ] Logical operators (`&& || !`) with short-circuit evaluation
- [ ] Functions (parameters, return values, a call stack)
- [ ] Call-expression syntax (so native/hardware calls can be written in-language)
- [ ] Runtime error handling in the VM (division by zero, stack checks)
- [ ] Optimization passes and internal refactors (visitor-based AST dispatch,
      jump-table VM dispatch)
- [ ] _(stretch)_ a static type system

## Design notes

- **Hand-written recursive-descent parser** — no Flex/Bison/ANTLR. Operator
  precedence and associativity emerge naturally from the grammar's layered rule
  structure and the shape of the recursion.
- **Operator-as-data AST** — a single `BinaryExpr` node stores its operator as a
  field rather than one node type per operator, so adding operators costs zero
  new node types and one `case` in each tree walker.
- **Slot-based variables** — the compiler resolves each name to an integer slot at
  compile time; the VM stores values in a flat vector indexed by slot. Names are
  resolved away before runtime.
- **Control flow via jumps** — `if`/`while` compile to `Jump` / `JumpIfFalse`
  against a mutable program counter, with forward jumps *backpatched* once their
  target is known. No control-flow constructs exist at the bytecode level.
- **Diagnostics from a single source of truth** — every token carries a byte
  offset; the parser threads it onto AST nodes; every stage throws one
  `CompileError` type carrying that offset and a message. A single reporter turns
  the offset into `line:column` and a caret at display time — locations are stored
  cheaply once and formatted only when an error is actually shown.
- **Clean stage boundaries** — the compiler and VM communicate through *only* the
  bytecode. The compiler never touches runtime storage; the VM never sees a
  variable name. (This boundary is also what lets the VM run on a microcontroller
  with the compiler left behind on the host — see [`firmware/`](firmware/).)

## Build & run

Requires **CMake** and a **C++20** compiler.

```bash
cmake -B build
cmake --build build
./build/compiler
```

The driver (`src/main.cpp`) compiles and runs a sample program and prints its
result.

## Testing

The core compiler logic is built as a static library (`compiler_core`) that both
the CLI and the test runner link against. Tests use
[doctest](https://github.com/doctest/doctest) (vendored as a single header) and
are written test-first (TDD). The suite is split **per stage** — the lexer,
parser, compiler, and VM are each tested in isolation, and integration tests
drive whole programs end-to-end:

```cpp
CHECK(tokensToString("1 + 2") == "Integer(1) Plus Integer(2) EndOfFile");  // lexer
CHECK(astToString("1 + 2 * 3;") == "(+ 1 (* 2 3))");                       // parser
CHECK(bytecodeToString("print 5;") == "Push(5) Print");                    // compiler
CHECK(evaluate("let x = 5; let y = x * 2; y + 1;") == 11);                 // end-to-end
```

Build and run the suite:

```bash
cmake --build build --target tests
./build/tests
# or, via CTest:
ctest --test-dir build
```

Run one stage with a filter, e.g. `./build/tests --test-case="vm:*"`. Within each
stage, positive tests live in feature files (`expressions`, `variables`,
`control_flow`, …) and error tests in that stage's `errors.cpp`.

## Project structure

```
src/
├── lexer/       tokenizer and token definitions
├── parser/      recursive-descent parser
├── ast/         expression and statement node types
├── bytecode/    instruction set + (de)serialization
├── compiler/    AST → bytecode, with the symbol table
├── vm/          stack-based bytecode interpreter
├── error/       CompileError type + diagnostic reporter (line:col + caret)
└── main.cpp     CLI driver
tests/
├── doctest.h        vendored test framework
├── test_main.cpp    test-runner entry point
├── support/         per-stage + end-to-end test helpers
├── unit/            per-stage folders (lexer/ parser/ compiler/ vm/),
│                    feature files + an errors.cpp each; serialize & reporter
└── integration/     end-to-end evaluation & control-flow tests
firmware/            embeds the VM on an ESP32 (separate project — see its README)
```

## Motivation

Built to understand compiler engineering from the ground up — lexing, parsing,
AST design, semantic analysis, code generation, and virtual-machine execution —
by implementing each stage by hand rather than reaching for existing tools.

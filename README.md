# A Hand-Written Compiler & Bytecode Virtual Machine

> **Work in progress.** A from-scratch compiler and virtual machine for a small
> programming language, built in modern **C++20** as a first-principles study of
> compiler construction — no parser generators, no interpreter frameworks, every
> stage written by hand.

Source text is tokenized, parsed into an abstract syntax tree, compiled to a
custom bytecode, and executed on a stack-based virtual machine — the full
front-to-back pipeline of a real language implementation.

> _The language is currently unnamed — rename the project/title as you like._

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
- **Variables** — `let` bindings that can be read back and reused.
- **Semantic analysis** — a compile-time **symbol table** resolves each variable
  name to a numbered storage slot, so no variable names survive into the runtime.
- **Custom bytecode + stack VM** — a small instruction set (`Push`, `Add`, `Sub`,
  `Mul`, `Div`, `Neg`, `Load`, `Store`, …) executed against a value stack and
  slot-indexed variable storage.

### Example

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
- [x] Variables (`let`) with symbol-table name resolution
- [ ] Comparisons & booleans (`< > <= >= == != && ||`)
- [ ] Control flow (`if` / `while`) — jumps and a program counter
- [ ] `print` / output statements
- [ ] Functions (parameters, return values, a call stack)
- [ ] Parser error handling & diagnostics
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
- **Clean stage boundaries** — the compiler and VM communicate through *only* the
  bytecode. The compiler never touches runtime storage; the VM never sees a
  variable name.

## Build & run

Requires **CMake** and a **C++20** compiler.

```bash
cmake -B build
cmake --build build
./build/compiler
```

The driver (`src/main.cpp`) currently compiles and runs a sample program and
prints its AST, bytecode, and result.

## Project structure

```
src/
├── lexer/       tokenizer and token definitions
├── parser/      recursive-descent parser
├── ast/         expression and statement node types
├── bytecode/    instruction set definition
├── compiler/    AST → bytecode, with the symbol table
├── vm/          stack-based bytecode interpreter
└── main.cpp     driver
```

## Motivation

Built to understand compiler engineering from the ground up — lexing, parsing,
AST design, semantic analysis, code generation, and virtual-machine execution —
by implementing each stage by hand rather than reaching for existing tools.

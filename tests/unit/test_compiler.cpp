#include "doctest.h"
#include "helpers.h"
#include "error/error.h"

using namespace test;

TEST_CASE("compiler: arithmetic emits post-order bytecode") {
    CHECK(bytecodeToString("1 + 2;") == "Push(1) Push(2) Add");
    CHECK(bytecodeToString("2 * 3 + 4;") == "Push(2) Push(3) Mul Push(4) Add");
}

TEST_CASE("compiler: unary minus emits Neg after its operand") {
    CHECK(bytecodeToString("-5;") == "Push(5) Neg");
}

TEST_CASE("compiler: variables resolve names to slots") {
    CHECK(bytecodeToString("let x = 5; x;") == "Push(5) Store(0) Load(0)");
    CHECK(bytecodeToString("let x = 5; let y = x * 2; y;") ==
          "Push(5) Store(0) Load(0) Push(2) Mul Store(1) Load(1)");
}

TEST_CASE("compiler: comparisons map to their opcodes") {
    // instructionKindToString names comparison opcodes after their glyph token.
    CHECK(bytecodeToString("1 < 2;") == "Push(1) Push(2) Less");
    CHECK(bytecodeToString("1 == 2;") == "Push(1) Push(2) EqualEqual");
}

// --- Milestone 7 (control flow): assignment ---
// SKIPPED until `x = ...` compiles. Assignment reuses Store into the variable's
// EXISTING slot (no new opcode) — it looks like `let` but must not allocate a
// new slot.
TEST_CASE("compiler: assignment stores into the existing slot") {
    CHECK(bytecodeToString("let x = 5; x = 10;") == "Push(5) Store(0) Push(10) Store(0)");
    CHECK(bytecodeToString("let x = 1; x = x + 4;") ==
          "Push(1) Store(0) Load(0) Push(4) Add Store(0)");
}

// SKIPPED until blocks compile. A block is transparent at the bytecode level —
// it just emits its inner statements' code, no wrapper opcode.
TEST_CASE("compiler: block emits inner statements' bytecode" * doctest::skip()) {
    CHECK(bytecodeToString("{ let x = 5; x; }") == "Push(5) Store(0) Load(0)");
}

// --- Milestone 7 (control flow): if / else jumps ---
// SKIPPED until if/else compiles. The jump TARGET ADDRESSES below assume the
// standard layout: compile the condition, then JumpIfFalse over the then-branch;
// for else, a Jump over the else-branch. VERIFY against your actual
// bytecodeToString output when you un-skip — backpatched addresses depend on
// your exact codegen, so adjust the numbers to match reality if they differ.
TEST_CASE("compiler: if without else emits a forward JumpIfFalse") {
    CHECK(bytecodeToString("let r = 0; if (2 < 1) { r = 1; }") ==
          "Push(0) Store(0) Push(2) Push(1) Less JumpIfFalse(8) Push(1) Store(0)");
}

TEST_CASE("compiler: if/else emits JumpIfFalse to else and Jump to end") {
    CHECK(bytecodeToString("let r = 0; if (2 < 1) { r = 1; } else { r = 2; }") ==
          "Push(0) Store(0) Push(2) Push(1) Less JumpIfFalse(9) "
          "Push(1) Store(0) Jump(11) Push(2) Store(0)");
}

// SKIPPED until while compiles. The forward JumpIfFalse (exit) is backpatched;
// the Jump at the bottom is a BACKWARD jump whose target (the loop top = index 2)
// is known immediately, no backpatch. VERIFY the addresses against actual output.
TEST_CASE("compiler: while loop emits a backward Jump to the loop top") {
    CHECK(bytecodeToString("let i = 0; while (i < 3) { i = i + 1; }") ==
          "Push(0) Store(0) Load(0) Push(3) Less JumpIfFalse(11) "
          "Load(0) Push(1) Add Store(0) Jump(2)");
}

// --- Milestone 8 (print) ---
TEST_CASE("compiler: print emits Print after its expression") {
    CHECK(bytecodeToString("print 5;") == "Push(5) Print");
    CHECK(bytecodeToString("print 1 + 2;") == "Push(1) Push(2) Add Print");
}

// --- Error handling (sub-step 5): semantic errors ---
// symbols_ lookups now throw instead of silently inserting slot 0.
TEST_CASE("compiler: using an undeclared variable is a reported error") {
    CHECK_THROWS_AS(compile("x + 1;"), CompileError);
    CHECK_THROWS_AS(compile("let y = x;"), CompileError);   // x never declared
}

TEST_CASE("compiler: assigning to an undeclared variable is a reported error") {
    CHECK_THROWS_AS(compile("x = 5;"), CompileError);       // no prior 'let x'
}

TEST_CASE("compiler: a declared variable still compiles fine") {
    CHECK(bytecodeToString("let x = 5; x = 10; x;") ==
          "Push(5) Store(0) Push(10) Store(0) Load(0)");
}

TEST_CASE("compiler: redeclaring a variable is a reported error") {
    CHECK_THROWS_AS(compile("let x = 1; let x = 2;"), CompileError);
}

TEST_CASE("compiler: reassigning (not redeclaring) a variable is fine") {
    // 'x = 2' is an assignment, not a second 'let' — no error, reuses slot 0.
    CHECK(bytecodeToString("let x = 1; x = 2;") == "Push(1) Store(0) Push(2) Store(0)");
}

#include "doctest.h"
#include "helpers.h"

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
TEST_CASE("compiler: assignment stores into the existing slot" * doctest::skip()) {
    CHECK(bytecodeToString("let x = 5; x = 10;") == "Push(5) Store(0) Push(10) Store(0)");
    CHECK(bytecodeToString("let x = 1; x = x + 4;") ==
          "Push(1) Store(0) Load(0) Push(4) Add Store(0)");
}

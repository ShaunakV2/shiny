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

TEST_CASE("compiler: comparisons map to their opcodes") {
    // instructionKindToString names comparison opcodes after their glyph token.
    CHECK(bytecodeToString("1 < 2;") == "Push(1) Push(2) Less");
    CHECK(bytecodeToString("1 == 2;") == "Push(1) Push(2) EqualEqual");
}

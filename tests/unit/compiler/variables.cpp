#include "doctest.h"
#include "helpers.h"

using namespace test;

TEST_CASE("compiler: variables resolve names to slots") {
    CHECK(bytecodeToString("let x = 5; x;") == "Push(5) Store(0) Load(0)");
    CHECK(bytecodeToString("let x = 5; let y = x * 2; y;") ==
          "Push(5) Store(0) Load(0) Push(2) Mul Store(1) Load(1)");
}

TEST_CASE("compiler: assignment stores into the existing slot") {
    // Assignment reuses the variable's slot (no new one, unlike 'let').
    CHECK(bytecodeToString("let x = 5; x = 10;") == "Push(5) Store(0) Push(10) Store(0)");
    CHECK(bytecodeToString("let x = 1; x = x + 4;") ==
          "Push(1) Store(0) Load(0) Push(4) Add Store(0)");
}

TEST_CASE("compiler: reassigning (not redeclaring) a variable is fine") {
    // 'x = 2' is an assignment, not a second 'let' — no error, reuses slot 0.
    CHECK(bytecodeToString("let x = 1; x = 2;") == "Push(1) Store(0) Push(2) Store(0)");
}

TEST_CASE("compiler: a declared variable still compiles fine") {
    CHECK(bytecodeToString("let x = 5; x = 10; x;") ==
          "Push(5) Store(0) Push(10) Store(0) Load(0)");
}

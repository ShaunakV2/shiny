#include "doctest.h"
#include "helpers.h"

using namespace test;

// End-to-end tests: source string -> result. These exercise the whole pipeline
// at once; when one fails, the matching unit test (lexer/parser/compiler/vm)
// tells you which stage is at fault.

TEST_CASE("integer arithmetic") {
    CHECK(evaluate("1 + 2 * 3;") == 7);
    CHECK(evaluate("(1 + 2) * 3;") == 9);
    CHECK(evaluate("10 - 4 - 2;") == 4);
    CHECK(evaluate("8 / 4 / 2;") == 1);
    CHECK(evaluate("2 + 3 * 4 - 1;") == 13);
}

TEST_CASE("unary minus") {
    CHECK(evaluate("-5 + 3;") == -2);
    CHECK(evaluate("- -5;") == 5);
    CHECK(evaluate("3 * -5;") == -15);
    CHECK(evaluate("-(2 + 3);") == -5);
}

TEST_CASE("variables") {
    CHECK(evaluate("let x = 5; x + 1;") == 6);
    CHECK(evaluate("let x = 5; let y = x * 2; y + 1;") == 11);
    CHECK(evaluate("let a = 3; let b = 4; a * a + b * b;") == 25);
}

TEST_CASE("comparisons") {
    CHECK(evaluate("1 + 2 < 3 * 4;") == 1);
    CHECK(evaluate("3 * 4 < 1 + 2;") == 0);
    CHECK(evaluate("5 >= 5;") == 1);
    CHECK(evaluate("5 > 5;") == 0);
    CHECK(evaluate("4 <= 3;") == 0);
    CHECK(evaluate("2 == 2;") == 1);
    CHECK(evaluate("2 != 2;") == 0);
    CHECK(evaluate("1 + 2 == 3;") == 1);
}

TEST_CASE("comparisons combined with variables") {
    CHECK(evaluate("let x = 5; x > 3;") == 1);
    CHECK(evaluate("let x = 5; x == 5;") == 1);
    CHECK(evaluate("let x = 5; let y = 10; x < y;") == 1);
    CHECK(evaluate("let x = 5; x < 10 == 1;") == 1);
}

// SKIPPED until assignment (Milestone 7, sub-step 1) is implemented — `x = ...`
// isn't parseable yet, so running this un-skipped would crash the suite.
TEST_CASE("assignment") {
    CHECK(evaluate("let x = 5; x = 10; x;") == 10);
    CHECK(evaluate("let x = 1; x = x + 4; x;") == 5);   // RHS reads old x
    CHECK(evaluate("let x = 2; let y = 3; x = y; x;") == 3);
}

// SKIPPED until blocks work end-to-end. A block runs its statements in order;
// with flat scoping, a block can read/write outer variables.
TEST_CASE("blocks" * doctest::skip()) {
    CHECK(evaluate("let x = 0; { x = 5; } x;") == 5);
    CHECK(evaluate("{ let x = 1; let y = 2; x + y; }") == 3);
}

// --- Milestone 8 (print): behavior via captured output ---
TEST_CASE("print outputs values (newline-separated)" * doctest::skip()) {
    CHECK(output("print 5;") == "5\n");
    CHECK(output("print 1 + 2;") == "3\n");
    CHECK(output("let x = 4; print x; print x * 2;") == "4\n8\n");
    CHECK(output("let i = 0; while (i < 3) { print i; i = i + 1; }") == "0\n1\n2\n");
}

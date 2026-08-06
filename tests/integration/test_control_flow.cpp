#include "doctest.h"
#include "helpers.h"

using namespace test;

// ---------------------------------------------------------------------------
// Control-flow tests for Milestone 7 (if / while).
//
// SKIPPED for now: the language cannot yet lex or parse `if`, `while`, blocks
// `{ }`, or assignment `x = ...`. Running them un-skipped would crash the suite
// on a null AST node, so each carries `* doctest::skip()`.
//
// PROVISIONAL C-like syntax (confirm when the milestone starts). Implies three
// sub-features: statement BLOCKS `{ }`, ASSIGNMENT (`x = expr;`), and the
// `if`/`while` keywords + jump opcodes.
//
// TDD: delete `* doctest::skip()` on a test to turn it on — red, then implement
// until green.
// ---------------------------------------------------------------------------

TEST_CASE("if without else" * doctest::skip()) {
    CHECK(evaluate("let x = 0; if (1 < 2) { x = 10; } x;") == 10);  // taken
    CHECK(evaluate("let x = 0; if (2 < 1) { x = 10; } x;") == 0);   // not taken
}

TEST_CASE("if / else" * doctest::skip()) {
    CHECK(evaluate("let x = 0; if (1 < 2) { x = 1; } else { x = 2; } x;") == 1);
    CHECK(evaluate("let x = 0; if (2 < 1) { x = 1; } else { x = 2; } x;") == 2);
}

TEST_CASE("while loop counts up" * doctest::skip()) {
    CHECK(evaluate("let x = 0; while (x < 5) { x = x + 1; } x;") == 5);
}

TEST_CASE("while loop accumulates" * doctest::skip()) {
    CHECK(evaluate("let s = 0; let i = 1; "
                   "while (i <= 4) { s = s + i; i = i + 1; } "
                   "s;") == 10);
}

TEST_CASE("nested control flow" * doctest::skip()) {
    CHECK(evaluate("let count = 0; let i = 1; "
                   "while (i <= 6) { "
                   "  if (i - (i / 2) * 2 == 0) { count = count + 1; } "
                   "  i = i + 1; "
                   "} "
                   "count;") == 3);
}

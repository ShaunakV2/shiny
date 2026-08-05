#include "doctest.h"
#include "test_helpers.h"

// ---------------------------------------------------------------------------
// Control-flow tests for the NEXT milestone (if / while).
//
// These are SKIPPED for now: the language cannot yet lex or parse `if`, `while`,
// blocks `{ }`, or assignment `x = ...`. Running them un-skipped would crash the
// suite on a null AST node rather than fail cleanly, so each carries a
// `* doctest::skip()` decorator.
//
// The syntax below is PROVISIONAL — to be confirmed when the milestone starts.
// Note it implies three sub-features the milestone will bring: statement BLOCKS
// (`{ ... }`), ASSIGNMENT (`x = expr;`, distinct from `let`), and the `if`/`while`
// keywords + jump opcodes.
//
// TDD workflow: as each feature lands, delete the `* doctest::skip()` on the
// relevant test to turn it on — watch it go red, then implement until green.
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
    // sum of 1..4 == 10
    CHECK(evaluate("let s = 0; let i = 1; "
                   "while (i <= 4) { s = s + i; i = i + 1; } "
                   "s;") == 10);
}

TEST_CASE("nested control flow" * doctest::skip()) {
    // count how many of 1..6 are even, using if inside while
    CHECK(evaluate("let count = 0; let i = 1; "
                   "while (i <= 6) { "
                   "  if (i - (i / 2) * 2 == 0) { count = count + 1; } "
                   "  i = i + 1; "
                   "} "
                   "count;") == 3);
}

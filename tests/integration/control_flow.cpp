#include "doctest.h"
#include "helpers.h"

using namespace test;

// End-to-end control flow: if / else / while, including nesting.

TEST_CASE("if without else" ) {
    CHECK(evaluate("let x = 0; if (1 < 2) { x = 10; } x;") == 10);  // taken
    CHECK(evaluate("let x = 0; if (2 < 1) { x = 10; } x;") == 0);   // not taken
}

TEST_CASE("if / else" ) {
    CHECK(evaluate("let x = 0; if (1 < 2) { x = 1; } else { x = 2; } x;") == 1);
    CHECK(evaluate("let x = 0; if (2 < 1) { x = 1; } else { x = 2; } x;") == 2);
}

TEST_CASE("while loop counts up") {
    CHECK(evaluate("let x = 0; while (x < 5) { x = x + 1; } x;") == 5);
}

TEST_CASE("while loop accumulates" ) {
    CHECK(evaluate("let s = 0; let i = 1; "
                   "while (i <= 4) { s = s + i; i = i + 1; } "
                   "s;") == 10);
}

TEST_CASE("nested control flow") {
    CHECK(evaluate("let count = 0; let i = 1; "
                   "while (i <= 6) { "
                   "  if (i - (i / 2) * 2 == 0) { count = count + 1; } "
                   "  i = i + 1; "
                   "} "
                   "count;") == 3);
}

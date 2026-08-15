#include "doctest.h"
#include "helpers.h"

using namespace test;

TEST_CASE("parser: arithmetic precedence and associativity") {
    CHECK(astToString("1 + 2 * 3;") == "(+ 1 (* 2 3))");   // * binds tighter
    CHECK(astToString("(1 + 2) * 3;") == "(* (+ 1 2) 3)"); // parens override
    CHECK(astToString("1 - 2 - 3;") == "(- (- 1 2) 3)");   // left-associative
}

TEST_CASE("parser: unary minus") {
    CHECK(astToString("-5;") == "-5");
    CHECK(astToString("--5;") == "--5");            // double negation
    CHECK(astToString("3 * -5;") == "(* 3 -5)");    // unary binds tighter than *
}

TEST_CASE("parser: comparison precedence (looser than arithmetic)") {
    CHECK(astToString("1 + 2 < 3 * 4;") == "(< (+ 1 2) (* 3 4))");
    CHECK(astToString("x < 10 == 1;") == "(== (< x 10) 1)");  // == loosest
}

// Function calls — red until parseFactor parses a trailing '(' as a CallExpr.
TEST_CASE("parser: call expression") {
    CHECK(astToString("add(1, 2);") == "(call add 1 2)");
    CHECK(astToString("foo();") == "(call foo)");                         // no args
    CHECK(astToString("f(1 + 2, x * 3);") == "(call f (+ 1 2) (* x 3))"); // args are full expressions
}

TEST_CASE("parser: calls nest in expressions and arguments") {
    CHECK(astToString("1 + f(2);") == "(+ 1 (call f 2))");
    CHECK(astToString("f(g(1));") == "(call f (call g 1))");
}

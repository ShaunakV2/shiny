#include "doctest.h"
#include "helpers.h"

using namespace test;

TEST_CASE("parser: let statement") {
    CHECK(astToString("let x = 5;") == "(let x = 5)");
    CHECK(astToString("let y = x * 2;") == "(let y = (* x 2))");
}

TEST_CASE("parser: assignment statement") {
    CHECK(astToString("x = 10;") == "(assign x = 10)");
    CHECK(astToString("x = x + 4;") == "(assign x = (+ x 4))");  // RHS is a full expression
}

TEST_CASE("parser: block groups statements") {
    CHECK(astToString("{ let x = 1; x; }") == "(block (let x = 1) x)");
    CHECK(astToString("{ }") == "(block)");
}

TEST_CASE("parser: if without else") {
    CHECK(astToString("if (1 < 2) { x = 1; }") ==
          "(if (< 1 2) (block (assign x = 1)))");
}

TEST_CASE("parser: if with else") {
    CHECK(astToString("if (1 < 2) { x = 1; } else { x = 2; }") ==
          "(if (< 1 2) (block (assign x = 1)) (block (assign x = 2)))");
}

TEST_CASE("parser: while loop" ) {
    CHECK(astToString("while (i < 3) { i = i + 1; }") ==
          "(while (< i 3) (block (assign i = (+ i 1))))");
}

TEST_CASE("parser: print statement") {
    CHECK(astToString("print x + 1;") == "(print (+ x 1))");
}

// return / function declarations — red until parseStatement dispatches Return / Fn.
TEST_CASE("parser: return statement") {
    CHECK(astToString("return 5;") == "(return 5)");
    CHECK(astToString("return a + b;") == "(return (+ a b))");
}

TEST_CASE("parser: function declaration") {
    CHECK(astToString("fn add(a, b) { return a + b; }") ==
          "(fn add (a b) (block (return (+ a b))))");
    CHECK(astToString("fn go() { print 1; }") ==
          "(fn go () (block (print 1)))");
}

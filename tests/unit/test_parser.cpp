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

TEST_CASE("parser: let statement") {
    CHECK(astToString("let x = 5;") == "(let x = 5)");
    CHECK(astToString("let y = x * 2;") == "(let y = (* x 2))");
}

// --- Milestone 7 (control flow): assignment ---
// SKIPPED until `x = ...` parses. Expected string assumes AssignStatement::print
// emits "(assign <name> <value>)" — adjust here if you choose a different format.
TEST_CASE("parser: assignment statement") {
    CHECK(astToString("x = 10;") == "(assign x = 10)");
    CHECK(astToString("x = x + 4;") == "(assign x = (+ x 4))");  // RHS is a full expression
}

// SKIPPED until blocks parse. Expected assumes BlockStatement::print emits
// "(block <stmt> <stmt> ...)" — adjust if you choose a different format.
TEST_CASE("parser: block groups statements") {
    CHECK(astToString("{ let x = 1; x; }") == "(block (let x = 1) x)");
    CHECK(astToString("{ }") == "(block)");
}

// SKIPPED until `if` parses. Expected assumes IfStatement::print emits
// "(if <cond> <then>)" and "(if <cond> <then> <else>)" — adjust if you differ.
TEST_CASE("parser: if without else") {
    CHECK(astToString("if (1 < 2) { x = 1; }") ==
          "(if (< 1 2) (block (assign x = 1)))");
}

TEST_CASE("parser: if with else") {
    CHECK(astToString("if (1 < 2) { x = 1; } else { x = 2; }") ==
          "(if (< 1 2) (block (assign x = 1)) (block (assign x = 2)))");
}

// SKIPPED until `while` parses. Expected assumes WhileStatement::print emits
// "(while <cond> <body>)" — adjust if you differ.
TEST_CASE("parser: while loop" ) {
    CHECK(astToString("while (i < 3) { i = i + 1; }") ==
          "(while (< i 3) (block (assign i = (+ i 1))))");
}

// --- Milestone 8 (print) ---
TEST_CASE("parser: print statement" * doctest::skip()) {
    CHECK(astToString("print x + 1;") == "(print (+ x 1))");
}

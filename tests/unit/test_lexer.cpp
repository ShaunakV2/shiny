#include "doctest.h"
#include "helpers.h"

using namespace test;

TEST_CASE("lexer: integers and arithmetic") {
    CHECK(tokensToString("1 + 2") == "Integer(1) Plus Integer(2) EndOfFile");
    CHECK(tokensToString("3 * (4 - 5)") ==
          "Integer(3) Star LParen Integer(4) Minus Integer(5) RParen EndOfFile");
}

TEST_CASE("lexer: multi-digit numbers and whitespace are skipped") {
    CHECK(tokensToString("  42   +  100 ") == "Integer(42) Plus Integer(100) EndOfFile");
}

TEST_CASE("lexer: let bindings and identifiers") {
    CHECK(tokensToString("let x = 5;") ==
          "Let Identifier(x) Assign Integer(5) Semicolon EndOfFile");
}

TEST_CASE("lexer: multi-character comparison operators (maximal munch)") {
    CHECK(tokensToString("a <= b >= c == d != e < f > g") ==
          "Identifier(a) LessEqual Identifier(b) GreaterEqual Identifier(c) "
          "EqualEqual Identifier(d) BangEqual Identifier(e) Less Identifier(f) "
          "Greater Identifier(g) EndOfFile");
}

TEST_CASE("lexer: single '=' stays Assign, not EqualEqual") {
    CHECK(tokensToString("x = 5") == "Identifier(x) Assign Integer(5) EndOfFile");
}

// --- Milestone 7 (control flow): new keywords and braces ---
// SKIPPED until the lexer recognizes if/else/while keywords and { } braces.
// (These fail cleanly rather than crash — `if` currently lexes as an Identifier,
// `{` as Unknown — but skipped keeps the suite green until implemented.)
// Assumes TokenKinds: If, Else, While, LBrace, RBrace.

TEST_CASE("lexer: if / else / braces") {
    CHECK(tokensToString("if (x < 2) { } else { }") ==
          "If LParen Identifier(x) Less Integer(2) RParen LBrace RBrace "
          "Else LBrace RBrace EndOfFile");
}

TEST_CASE("lexer: while / braces" ) {
    CHECK(tokensToString("while (x) { }") ==
          "While LParen Identifier(x) RParen LBrace RBrace EndOfFile");
}

// --- Milestone 8 (print) ---
TEST_CASE("lexer: print keyword") {
    CHECK(tokensToString("print x;") == "Print Identifier(x) Semicolon EndOfFile");
}

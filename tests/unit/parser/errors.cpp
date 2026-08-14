#include "doctest.h"
#include "helpers.h"
#include "error/error.h"

using namespace test;

TEST_CASE("parser: a missing semicolon is reported") {
    CHECK_THROWS_AS(parse("let x = 5"), CompileError);
    CHECK_THROWS_AS(parse("x = 5"), CompileError);
    CHECK_THROWS_AS(parse("print 5"), CompileError);
}

TEST_CASE("parser: a malformed let is reported") {
    CHECK_THROWS_AS(parse("let = 5;"), CompileError);   // missing name
    CHECK_THROWS_AS(parse("let x 5;"), CompileError);   // missing '='
}

TEST_CASE("parser: an unclosed paren or block is reported") {
    CHECK_THROWS_AS(parse("let x = (1 + 2;"), CompileError);      // missing ')'
    CHECK_THROWS_AS(parse("if (1 < 2) { x = 1;"), CompileError);  // missing '}'
}

TEST_CASE("parser: a missing expression is reported") {
    CHECK_THROWS_AS(parse("let x = ;"), CompileError);
    CHECK_THROWS_AS(parse("1 + ;"), CompileError);
}

TEST_CASE("parser: if/while without parens are reported") {
    CHECK_THROWS_AS(parse("if 1 < 2 { x = 1; }"), CompileError);
    CHECK_THROWS_AS(parse("while 1 { x = 1; }"), CompileError);
}

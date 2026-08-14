#include "doctest.h"
#include "helpers.h"
#include "error/error.h"

using namespace test;

TEST_CASE("lexer: unknown character is a reported error") {
    CHECK_THROWS_AS(lex("@"), CompileError);
    CHECK_THROWS_AS(lex("let x = 1 # 2;"), CompileError);
}

TEST_CASE("lexer: a lone '!' (not '!=') is a reported error") {
    CHECK_THROWS_AS(lex("1 ! 2"), CompileError);
}

TEST_CASE("lexer: an integer literal too large to fit is a reported error") {
    CHECK_THROWS_AS(lex("99999999999999"), CompileError);
}

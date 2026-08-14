#include "doctest.h"
#include "helpers.h"
#include "error/error.h"

using namespace test;

// Semantic analysis: symbols_ lookups throw instead of silently inserting slot 0.

TEST_CASE("compiler: using an undeclared variable is a reported error") {
    CHECK_THROWS_AS(compile("x + 1;"), CompileError);
    CHECK_THROWS_AS(compile("let y = x;"), CompileError);   // x never declared
}

TEST_CASE("compiler: assigning to an undeclared variable is a reported error") {
    CHECK_THROWS_AS(compile("x = 5;"), CompileError);       // no prior 'let x'
}

TEST_CASE("compiler: redeclaring a variable is a reported error") {
    CHECK_THROWS_AS(compile("let x = 1; let x = 2;"), CompileError);
}

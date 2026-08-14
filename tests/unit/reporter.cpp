#include "doctest.h"
#include "helpers.h"
#include "error/error.h"
#include "error/reporter.h"

using namespace test;

// The reporter turns a CompileError's byte offset into a line:col + caret
// diagnostic. We trigger real lexer errors so the lexer computes the offset,
// then check the formatted output.

TEST_CASE("reporter: computes line:col and points a caret at the offense") {
    const std::string src = "let x = 5;\nlet y = @;";   // '@' is line 2, col 9
    try {
        lex(src);
        FAIL("expected a CompileError");
    } catch (const CompileError& e) {
        const std::string out = formatError(src, e);
        CHECK(out.find("2:9") != std::string::npos);
        CHECK(out.find("unexpected character '@'") != std::string::npos);
        CHECK(out.find("^") != std::string::npos);
    }
}

TEST_CASE("reporter: a first-character error reports column 1") {
    const std::string src = "@";
    try {
        lex(src);
        FAIL("expected a CompileError");
    } catch (const CompileError& e) {
        CHECK(formatError(src, e).find("1:1") != std::string::npos);
    }
}

#include "doctest.h"
#include "helpers.h"

using namespace test;

// These tests feed hand-built bytecode straight to the VM — no lexer, parser,
// or compiler involved — so a failure here is squarely a VM bug.

TEST_CASE("vm: arithmetic") {
    CHECK(run({{InstructionKind::Push, 2}, {InstructionKind::Push, 3}, {InstructionKind::Add}}) == 5);
    CHECK(run({{InstructionKind::Push, 6}, {InstructionKind::Push, 7}, {InstructionKind::Mul}}) == 42);
}

TEST_CASE("vm: subtraction and division respect operand order") {
    // pop right first, then left, compute left OP right
    CHECK(run({{InstructionKind::Push, 10}, {InstructionKind::Push, 4}, {InstructionKind::Sub}}) == 6);
    CHECK(run({{InstructionKind::Push, 12}, {InstructionKind::Push, 4}, {InstructionKind::Div}}) == 3);
}

TEST_CASE("vm: unary negation") {
    CHECK(run({{InstructionKind::Push, 5}, {InstructionKind::Neg}}) == -5);
}

TEST_CASE("vm: store then load a slot") {
    CHECK(run({{InstructionKind::Push, 42}, {InstructionKind::Store, 0}, {InstructionKind::Load, 0}}) == 42);
}

TEST_CASE("vm: comparisons produce 1 (true) or 0 (false)") {
    CHECK(run({{InstructionKind::Push, 3}, {InstructionKind::Push, 5}, {InstructionKind::Lt}}) == 1);
    CHECK(run({{InstructionKind::Push, 5}, {InstructionKind::Push, 3}, {InstructionKind::Lt}}) == 0);
    CHECK(run({{InstructionKind::Push, 5}, {InstructionKind::Push, 5}, {InstructionKind::Eq}}) == 1);
}

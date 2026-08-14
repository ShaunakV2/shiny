#include "doctest.h"
#include "helpers.h"

using namespace test;

TEST_CASE("compiler: block emits inner statements' bytecode") {
    // A block is transparent at the bytecode level — no wrapper opcode.
    CHECK(bytecodeToString("{ let x = 5; x; }") == "Push(5) Store(0) Load(0)");
}

TEST_CASE("compiler: if without else emits a forward JumpIfFalse") {
    CHECK(bytecodeToString("let r = 0; if (2 < 1) { r = 1; }") ==
          "Push(0) Store(0) Push(2) Push(1) Less JumpIfFalse(8) Push(1) Store(0)");
}

TEST_CASE("compiler: if/else emits JumpIfFalse to else and Jump to end") {
    CHECK(bytecodeToString("let r = 0; if (2 < 1) { r = 1; } else { r = 2; }") ==
          "Push(0) Store(0) Push(2) Push(1) Less JumpIfFalse(9) "
          "Push(1) Store(0) Jump(11) Push(2) Store(0)");
}

TEST_CASE("compiler: while loop emits a backward Jump to the loop top") {
    // The forward JumpIfFalse (exit) is backpatched; the bottom Jump is a
    // backward jump whose target (the loop top) is known immediately.
    CHECK(bytecodeToString("let i = 0; while (i < 3) { i = i + 1; }") ==
          "Push(0) Store(0) Load(0) Push(3) Less JumpIfFalse(11) "
          "Load(0) Push(1) Add Store(0) Jump(2)");
}

TEST_CASE("compiler: print emits Print after its expression") {
    CHECK(bytecodeToString("print 5;") == "Push(5) Print");
    CHECK(bytecodeToString("print 1 + 2;") == "Push(1) Push(2) Add Print");
}

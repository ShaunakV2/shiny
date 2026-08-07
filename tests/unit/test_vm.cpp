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

// --- Milestone 7 (control flow): jumps ---
// Hand-built if/else-shaped bytecode. Layout (indices):
//   0: Push <cond>
//   1: JumpIfFalse 4      (false -> jump to the "else" at 4)
//   2: Push 111           "then" value
//   3: Jump 5             skip the else
//   4: Push 222           "else" value
// A correct VM returns 222 when the condition is false (jump taken) and 111 when
// true (fall through). Each result is UNIQUE to whether the jump fired, so these
// genuinely distinguish a working jump from a broken one.

TEST_CASE("vm: JumpIfFalse takes the jump when the condition is 0 (false)") {
    CHECK(run({{InstructionKind::Push, 0},
               {InstructionKind::JumpIfFalse, 4},
               {InstructionKind::Push, 111},
               {InstructionKind::Jump, 5},
               {InstructionKind::Push, 222}}) == 222);
}

TEST_CASE("vm: JumpIfFalse falls through when the condition is nonzero (true)") {
    CHECK(run({{InstructionKind::Push, 1},
               {InstructionKind::JumpIfFalse, 4},
               {InstructionKind::Push, 111},
               {InstructionKind::Jump, 5},
               {InstructionKind::Push, 222}}) == 111);
}

TEST_CASE("vm: Jump is unconditional (skips an instruction)") {
    // Jump 3 skips the Push 999; result is the earlier Push 5, not 999.
    CHECK(run({{InstructionKind::Push, 5},
               {InstructionKind::Jump, 3},
               {InstructionKind::Push, 999}}) == 5);
}

// --- ESP32 Step 1 (native calls): CallNative dials a registered C++ function ---
// On the device these natives will be digitalWrite/delay; here we register mock
// functions so we can prove the mechanism entirely on the host. A native reads
// its args from the stack (pop) and leaves its result on the stack (push), just
// like a built-in opcode.

TEST_CASE("vm: CallNative invokes the registered function, passing args via the stack") {
    VM vm;
    // Native id 7: pop one arg, push arg * 10.
    vm.registerNative(7, [](std::stack<int>& s) {
        int arg = s.top(); s.pop();
        s.push(arg * 10);
    });
    // Push 4, then CallNative 7  ->  40 left on the stack.
    CHECK(vm.run({{InstructionKind::Push, 4},
                  {InstructionKind::CallNative, 7}}) == 40);
}

TEST_CASE("vm: CallNative reaches the exact native named by its id, and records its arg") {
    VM vm;
    int seen = -1;   // what the native was actually called with
    vm.registerNative(0, [&](std::stack<int>&) { seen = 100; });  // decoy
    vm.registerNative(1, [&](std::stack<int>& s) {                // the one we dial
        seen = s.top(); s.pop();
    });
    // CallNative 1 must run native 1 (not 0) and see the pushed arg 42.
    vm.run({{InstructionKind::Push, 42},
            {InstructionKind::CallNative, 1}});
    CHECK(seen == 42);
}

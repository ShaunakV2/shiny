#include "doctest.h"
#include "helpers.h"

using namespace test;

// Hand-built bytecode for the call mechanism (Call/Return + frames), following
// the calling convention: caller pushes args -> a prologue Stores them into the
// callee frame's slots -> body runs -> Return leaves the result on the stack.
// Main never Returns; it runs to the end of the code. A leading Jump skips over
// the function body so top-level execution doesn't fall into it.

TEST_CASE("vm: a function call returns a value  (double(5) -> 10)") {
    // 0: Jump 6        main: skip over the function body
    // --- double(n), entry = 1 ---
    // 1: Store 0       prologue: bind n -> slot 0
    // 2: Load 0
    // 3: Push 2
    // 4: Mul           n * 2
    // 5: Return
    // --- main ---
    // 6: Push 5        arg
    // 7: Call 1        double(5)
    CHECK(run({{InstructionKind::Jump, 6},
               {InstructionKind::Store, 0},
               {InstructionKind::Load, 0},
               {InstructionKind::Push, 2},
               {InstructionKind::Mul},
               {InstructionKind::Return},
               {InstructionKind::Push, 5},
               {InstructionKind::Call, 1}}) == 10);
}

TEST_CASE("vm: arguments bind in order  (sub(10, 3) -> 7, not -7)") {
    // Caller pushes a then b, so b is on top; the prologue Stores b into slot 1
    // first, then a into slot 0 — so Load 0 = a, Load 1 = b.
    // 0: Jump 7
    // 1: Store 1       bind b -> slot 1 (top of stack)
    // 2: Store 0       bind a -> slot 0
    // 3: Load 0        a
    // 4: Load 1        b
    // 5: Sub           a - b
    // 6: Return
    // 7: Push 10       a
    // 8: Push 3        b
    // 9: Call 1
    CHECK(run({{InstructionKind::Jump, 7},
               {InstructionKind::Store, 1},
               {InstructionKind::Store, 0},
               {InstructionKind::Load, 0},
               {InstructionKind::Load, 1},
               {InstructionKind::Sub},
               {InstructionKind::Return},
               {InstructionKind::Push, 10},
               {InstructionKind::Push, 3},
               {InstructionKind::Call, 1}}) == 7);
}

TEST_CASE("vm: recursion keeps each call's locals separate  (factorial(3) -> 6)") {
    // fn factorial(n) { if (n < 2) return 1; return n * factorial(n - 1); }
    // Three nested calls are alive at the deepest point, each with its own n
    // (3, 2, 1) — the frame stack is what keeps them from colliding.
    //  0: Jump 15        main: skip the function body
    //  --- factorial(n), entry = 1 ---
    //  1: Store 0        bind n
    //  2: Load 0
    //  3: Push 2
    //  4: Lt             n < 2 ?
    //  5: JumpIfFalse 8  false -> else branch
    //  6: Push 1         then: return 1
    //  7: Return
    //  8: Load 0         else: n            (left operand of the multiply)
    //  9: Load 0         n                  (argument for the recursive call)
    // 10: Push 1
    // 11: Sub            n - 1
    // 12: Call 1         factorial(n - 1)
    // 13: Mul            n * factorial(n - 1)
    // 14: Return
    // --- main ---
    // 15: Push 3
    // 16: Call 1
    CHECK(run({{InstructionKind::Jump, 15},
               {InstructionKind::Store, 0},
               {InstructionKind::Load, 0},
               {InstructionKind::Push, 2},
               {InstructionKind::Lt},
               {InstructionKind::JumpIfFalse, 8},
               {InstructionKind::Push, 1},
               {InstructionKind::Return},
               {InstructionKind::Load, 0},
               {InstructionKind::Load, 0},
               {InstructionKind::Push, 1},
               {InstructionKind::Sub},
               {InstructionKind::Call, 1},
               {InstructionKind::Mul},
               {InstructionKind::Return},
               {InstructionKind::Push, 3},
               {InstructionKind::Call, 1}}) == 6);
}

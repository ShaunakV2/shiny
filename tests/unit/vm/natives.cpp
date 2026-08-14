#include "doctest.h"
#include "helpers.h"

using namespace test;

// CallNative dials a registered C++ function by id. On the device these natives
// are the hardware calls (gpio/delay); here we register mocks to prove the
// mechanism on the host. A native reads its args from the stack (pop) and leaves
// its result on the stack (push), just like a built-in opcode.

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

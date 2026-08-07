//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_VM_H
#define COMPILER_VM_H
#include <stack>
#include <vector>
#include <functional>
#include "../bytecode/instruction.h"

class VM {
public:
    int run(const std::vector<Instruction>& code);

    // Register a C++ function the bytecode can call via CallNative <id>.
    // The function receives the VM's stack: it pops its args and pushes its result.
    using NativeFn = std::function<void(std::stack<int>&)>;
    void registerNative(int id, NativeFn fn);
private:
    std::stack<int> stack_;
    std::vector<int> slots_;             // slots for variable storage.
    std::vector<NativeFn> natives_;      // indexed by id
};

#endif //COMPILER_VM_H

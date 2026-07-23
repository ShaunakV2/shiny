//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_VM_H
#define COMPILER_VM_H
#include <stack>
#include <vector>
#include "../bytecode/instruction.h"

class VM {
public:
    int run(const std::vector<Instruction>& code);
private:
    std::stack<int> stack_;

};

#endif //COMPILER_VM_H

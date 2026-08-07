//
// Created by Shaunak Vaghasia on 7/23/26.
//

#include "vm.h"

#include <iostream>

int VM::run(const std::vector<Instruction> &code) {
    size_t pc = 0;
    while (pc<code.size()) {
        const Instruction& instr = code[pc];
        pc++;
        if (instr.kind == InstructionKind::Push) {
            stack_.push(instr.value.value());
        }
        else if (instr.kind == InstructionKind::Neg) {
            int top = stack_.top();
            stack_.pop();
            stack_.push(-top);
        }
        else if (instr.kind == InstructionKind::Add) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left + right);
        }
        else if (instr.kind == InstructionKind::Sub) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left - right);
        }
        else if (instr.kind == InstructionKind::Mul) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left * right);
        }
        else if (instr.kind == InstructionKind::Div) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left / right);
        }
        else if (instr.kind == InstructionKind::Lt) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left < right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Gt) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left > right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Le) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left <= right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Ge) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left >= right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Eq) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left == right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Ne) {
            int right = stack_.top();
            stack_.pop();
            int left = stack_.top();
            stack_.pop();
            stack_.push(left != right ? 1 : 0);
        }
        else if (instr.kind == InstructionKind::Store) {
            const int val = stack_.top();
            stack_.pop();
            const int slot = instr.value.value();
            if (slot >= static_cast<int>(slots_.size())) {
                slots_.resize(slot + 1);
            }
            slots_[slot] = val;

        }
        else if (instr.kind == InstructionKind::Load) {
            int val = slots_[instr.value.value()];
            stack_.push(val);
        }
        else if (instr.kind == InstructionKind::Jump) {
            pc = instr.value.value();
        }
        else if (instr.kind == InstructionKind::JumpIfFalse) {
            int cond = stack_.top();
            stack_.pop();
            if (cond == 0) pc = instr.value.value();
        }
        else if (instr.kind == InstructionKind::Print) {
            int val = stack_.top();
            stack_.pop();
            std::cout << val << "\n";
        }
        else if (instr.kind == InstructionKind::CallNative) {
            natives_[instr.value.value()](stack_);   // look up by id, hand it the stack
        }
    }
    return stack_.empty() ? 0 : stack_.top();
};

void VM::registerNative(int id, NativeFn fn) {
    if (id >= static_cast<int>(natives_.size())) {
        natives_.resize(id + 1);
    }
    natives_[id] = std::move(fn);
}
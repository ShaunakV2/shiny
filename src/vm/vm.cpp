//
// Created by Shaunak Vaghasia on 7/23/26.
//

#include "vm.h"

#include <iostream>

int VM::run(const std::vector<Instruction> &code) {
    for (const Instruction& instr : code) {
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
    }
    return stack_.top();
};
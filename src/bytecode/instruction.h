//
// Created by Shaunak Vaghasia on 7/23/26.
//

#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include <string>
#include <optional>




enum class InstructionKind {
    Push,
    Mul,
    Add,
    Sub,
    Div,
    Neg,
};

struct Instruction {
    InstructionKind kind;
    std::optional<int> value;
};

inline std::string instructionKindToString(InstructionKind kind) {
    switch (kind) {
        case InstructionKind::Push: return "Push";
        case InstructionKind::Mul:  return "Mul";
        case InstructionKind::Add:  return "Add";
        case InstructionKind::Sub:  return "Sub";
        case InstructionKind::Div:  return "Div";
        case InstructionKind::Neg:  return "Neg";
    }
    return "Unknown";
}

#endif //COMPILER_INSTRUCTION_H

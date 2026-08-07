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
    Store,
    Load,
    Lt,
    Gt,
    Le,
    Ge,
    Eq,
    Ne,
    Jump,
    JumpIfFalse,
    While,
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
        case InstructionKind::Load: return "Load";
        case InstructionKind::Store: return "Store";
        case InstructionKind::Lt:  return "Less";
        case InstructionKind::Gt:  return "Greater";
        case InstructionKind::Le:  return "LessEqual";
        case InstructionKind::Ge:  return "GreaterEqual";
        case InstructionKind::Eq: return "EqualEqual";
        case InstructionKind::Ne: return "BangEqual";
        case InstructionKind::Jump: return "Jump";
        case InstructionKind::JumpIfFalse: return "JumpIfFalse";
        case InstructionKind::While: return "While";
    }
    return "Unknown";
}

#endif //COMPILER_INSTRUCTION_H

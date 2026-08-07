//
// Bytecode (de)serialization — flatten a program to bytes and back, so it can
// be sent over a wire (serial/WiFi) to the VM on the ESP32.
//
// Wire format (little-endian):
//   whole program:  [ uint32 count ][ record x count ]
//   one record:     [ uint8 kind ][ uint8 hasValue ][ int32 value ]
// Every record is a fixed 6 bytes; the value bytes are always written (0 when
// hasValue == 0) to keep records uniform and parsing simple.
//
// Header-only + free of exceptions/RTTI/iostream, so the SAME file compiles on
// the host (serialize, in tests) and on the device (deserialize, in firmware).
//

#ifndef COMPILER_SERIALIZE_H
#define COMPILER_SERIALIZE_H

#include <cstdint>
#include <vector>

#include "instruction.h"

// Value equality for Instruction — lets a round-trip test compare whole vectors
// with ==. Two instructions are equal iff same kind and same optional value.
inline bool operator==(const Instruction& a, const Instruction& b) {
    return a.kind == b.kind && a.value == b.value;
}

// ---- little-endian int32 <-> 4 bytes (shared by the count header and value) ----

inline void putInt32(std::vector<uint8_t>& buf, int32_t v) {
    buf.push_back(static_cast<uint8_t>( v        & 0xFF));  // lowest byte
    buf.push_back(static_cast<uint8_t>((v >> 8)  & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));  // highest byte
}

// Reads 4 bytes starting at `pos`, advances `pos` by 4, returns the rebuilt int.
inline int32_t getInt32(const std::vector<uint8_t>& buf, std::size_t& pos) {
    uint32_t v = static_cast<uint32_t>(buf[pos])
               | (static_cast<uint32_t>(buf[pos + 1]) << 8)
               | (static_cast<uint32_t>(buf[pos + 2]) << 16)
               | (static_cast<uint32_t>(buf[pos + 3]) << 24);
    pos += 4;                       // advance the cursor past the 4 bytes we read
    return static_cast<int32_t>(v);
}

// ---- program <-> bytes ----

inline std::vector<uint8_t> serialize(const std::vector<Instruction>& code) {
    std::vector<uint8_t> buf;

    // Header: how many instructions follow, so the reader knows when to stop.
    putInt32(buf, static_cast<int32_t>(code.size()));

    // One fixed 6-byte record per instruction.
    for (const Instruction& instr : code) {
        buf.push_back(static_cast<uint8_t>(instr.kind));         // kind   (1 byte)
        buf.push_back(instr.value.has_value() ? 1 : 0);          // flag   (1 byte)
        putInt32(buf, instr.value ? *instr.value : 0);           // value  (4 bytes, 0 if none)
    }
    return buf;
}

inline std::vector<Instruction> deserialize(const std::vector<uint8_t>& bytes) {
    std::vector<Instruction> code;
    std::size_t pos = 0;

    // Header: read the record count back out first.
    int32_t count = getInt32(bytes, pos);

    for (int32_t i = 0; i < count; ++i) {
        auto kind = static_cast<InstructionKind>(bytes[pos++]);  // 1 byte
        uint8_t hasValue = bytes[pos++];  // 1 byte
        int32_t value = getInt32(bytes, pos); // 4 bytes                        

        // Rebuild the instruction. The flag is what preserves nullopt vs 0.
        if (hasValue) {
            code.push_back({kind, value});
        } else {
            code.push_back({kind, std::nullopt});
        }
    }
    return code;
}

#endif // COMPILER_SERIALIZE_H

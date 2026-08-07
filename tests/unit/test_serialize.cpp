#include "doctest.h"
#include "bytecode/serialize.h"

// serialize <-> deserialize must be a LOSSLESS round trip: pack a program to
// bytes, unpack it, and get back a vector equal to the original. These run
// entirely on the host, but the same serialize.h compiles on the ESP32 — so
// proving the format here proves the exact bytes both ends will speak.

TEST_CASE("serialize: round-trips a program with and without operand values") {
    std::vector<Instruction> program = {
        {InstructionKind::Push, 42},
        {InstructionKind::Push, 8},
        {InstructionKind::Add},            // no value — flag must round-trip as "none"
        {InstructionKind::Store, 0},
        {InstructionKind::Jump, 3},
    };
    CHECK(deserialize(serialize(program)) == program);
}

TEST_CASE("serialize: round-trips the blink program") {
    std::vector<Instruction> blink = {
        {InstructionKind::Push, 8}, {InstructionKind::Push, 0}, {InstructionKind::CallNative, 0},
        {InstructionKind::Push, 500}, {InstructionKind::CallNative, 1},
        {InstructionKind::Push, 8}, {InstructionKind::Push, 1}, {InstructionKind::CallNative, 0},
        {InstructionKind::Push, 500}, {InstructionKind::CallNative, 1},
        {InstructionKind::Jump, 0},
    };
    CHECK(deserialize(serialize(blink)) == blink);
}

TEST_CASE("serialize: empty program round-trips to empty") {
    std::vector<Instruction> empty;
    CHECK(deserialize(serialize(empty)).empty());
}

TEST_CASE("serialize: negative and large values survive the round trip") {
    // Exercises the sign bit (byte 3 >= 128) and the full 32-bit width.
    std::vector<Instruction> program = {
        {InstructionKind::Push, -1},
        {InstructionKind::Push, -12345},
        {InstructionKind::Push, 2000000000},
    };
    CHECK(deserialize(serialize(program)) == program);
}

TEST_CASE("serialize: value 0 stays a value, not nullopt (the flag matters)") {
    std::vector<Instruction> program = {{InstructionKind::Push, 0}};
    std::vector<Instruction> back = deserialize(serialize(program));
    REQUIRE(back.size() == 1);
    CHECK(back[0].value.has_value());        // must NOT be confused for "no value"
    CHECK(back[0].value.value() == 0);
}

TEST_CASE("serialize: exact byte layout (count header + little-endian records)") {
    // One instruction, Push 42, pins the wire format down to the byte.
    std::vector<uint8_t> bytes = serialize({{InstructionKind::Push, 42}});

    // [count=1 : int32][kind : u8][hasValue : u8][value=42 : int32] = 4 + 6 = 10
    REQUIRE(bytes.size() == 10);
    // count = 1, little-endian
    CHECK(bytes[0] == 1); CHECK(bytes[1] == 0); CHECK(bytes[2] == 0); CHECK(bytes[3] == 0);
    // record
    CHECK(bytes[4] == static_cast<uint8_t>(InstructionKind::Push));
    CHECK(bytes[5] == 1);                     // hasValue
    CHECK(bytes[6] == 42);                    // value low byte
    CHECK(bytes[7] == 0); CHECK(bytes[8] == 0); CHECK(bytes[9] == 0);
}

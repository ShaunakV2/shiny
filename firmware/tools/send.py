#!/usr/bin/env python3
"""Push a bytecode program to the ESP32 over USB serial.

This is the host side of "flash-once, push-scripts-live": the VM is already on
the board waiting for programs; this script serializes a program to the exact
same wire format serialize.h uses and writes the bytes down the USB cable.

Note it's Python, not C++ — that's the whole point of a serialization format:
it's a language-neutral byte contract, so anything that can produce the bytes
can drive the VM.

Wire format (little-endian), mirrored from src/bytecode/serialize.h:
    [int32 count][ record x count ]
    record = [uint8 kind][uint8 hasValue][int32 value]

Usage:
    python3 send.py [port] [blink_count]
    e.g.  python3 send.py /dev/cu.usbmodem1101 3
"""

import struct
import sys

try:
    import serial  # pyserial
except ImportError:
    sys.exit("pyserial not found. Install with:  pip install pyserial\n"
             "(or run this with ESP-IDF's Python, which already has it)")

# InstructionKind values — MUST match the enum ORDER in src/bytecode/instruction.h.
PUSH = 0
CALL_NATIVE = 18

# Native ids — must match registerNatives() in firmware_main.cpp.
NATIVE_GPIO_SET = 0   # gpio_set_level(pin, level)
NATIVE_DELAY = 1      # delay(ms)

LED_PIN = 8
DELAY_MS = 300


def serialize(program):
    """program: list of (kind, value_or_None) -> bytes, matching serialize.h."""
    out = struct.pack("<i", len(program))          # count
    for kind, value in program:
        has_value = 1 if value is not None else 0
        out += struct.pack("<BBi", kind, has_value, value or 0)   # 6-byte record
    return out


def blink_program(times):
    """Blink the LED `times` times, then STOP (no loop -> finite -> returns)."""
    prog = []
    for _ in range(times):
        # LED on (active-low: level 0), wait
        prog += [(PUSH, LED_PIN), (PUSH, 0), (CALL_NATIVE, NATIVE_GPIO_SET)]
        prog += [(PUSH, DELAY_MS), (CALL_NATIVE, NATIVE_DELAY)]
        # LED off (level 1), wait
        prog += [(PUSH, LED_PIN), (PUSH, 1), (CALL_NATIVE, NATIVE_GPIO_SET)]
        prog += [(PUSH, DELAY_MS), (CALL_NATIVE, NATIVE_DELAY)]
    return prog


def main():
    port = sys.argv[1] if len(sys.argv) > 1 else "/dev/cu.usbmodem1101"
    times = int(sys.argv[2]) if len(sys.argv) > 2 else 3

    payload = serialize(blink_program(times))
    print(f"Sending {len(payload)} bytes to {port} (blink x{times})...")

    # Baud is ignored by USB-CDC, but pyserial requires a value.
    with serial.Serial(port, 115200) as ser:
        ser.write(payload)
        ser.flush()

    print("Sent. Watch the LED.")


if __name__ == "__main__":
    main()

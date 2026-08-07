# Running the Language on an ESP32

> **Work in progress.** Firmware that embeds the bytecode virtual machine from
> the parent [compiler project](../README.md) onto an **ESP32-C3** microcontroller,
> so programs written in the language can drive real hardware — and be pushed to
> the board **live over USB**, with no reflash.

The compiler stays on the host; only the VM ships to the chip. The board receives
compiled **bytecode**, deserializes it, and runs it — the same VM that powers the
desktop interpreter, cross-compiled for a RISC-V microcontroller.

## Architecture

The language pipeline is split across two machines, joined by bytecode:

```
   HOST (your computer)                               DEVICE (ESP32-C3)
   ────────────────────────────────────────          ────────────────────────
   source text
      │  lexer → parser → compiler                    ← the whole frontend
      ▼
   bytecode
      │  serialize                                    ← program ⇄ bytes
      ▼
   bytes ───────────────  USB serial  ─────────────▶  bytes
                                                          │  deserialize
                                                          ▼
                                                       bytecode
                                                          │  VM.run
                                                          ▼
                                                       GPIO / output
```

The device **never compiles**. It only knows how to deserialize and execute
bytecode; the lexer, parser, AST, and compiler never leave the host. Bytecode is
the entire interface between the two worlds.

## What works today

- **The VM runs on hardware** — the bytecode interpreter cross-compiles for the
  ESP32-C3 and executes programs on the chip.
- **Hardware access via native functions** — bytecode calls out to registered C++
  functions (`gpio_set_level`, `delay`) through a single `CallNative` opcode, so
  the VM itself stays hardware-agnostic.
- **Blink** — a hand-built bytecode program toggles the onboard LED.
- **Live program push (one-shot)** — after flashing once, the board sits in a
  receive loop: it reads a length-framed bytecode program off USB serial,
  deserializes it, runs it to completion, then waits for the next one. New
  behavior arrives as bytes, not a reflash.

## Design notes

- **Only the VM crosses over** — the ESP-IDF build wraps *just* `src/vm` and
  `src/bytecode` from the parent repo (as the `vm_core` component). The
  compiler, parser, and lexer — which use RTTI/`dynamic_cast` and heavier
  headers — stay on the host. The VM is deliberately RTTI-free so it fits.
- **Native functions as a "phone book"** — each hardware function is registered
  under a numeric id; `CallNative <id>` dials it, passing arguments on the VM
  stack. The *same* VM code runs on desktop (with mock natives in tests) and on
  the device (with real GPIO), because the phone book is filled in per platform.
- **No source copies** — the component references the parent repo's VM sources
  directly, so the desktop interpreter and the firmware always share one
  implementation.
- **Framing over a byte stream** — a 4-byte instruction *count* prefixes each
  program, so the receive loop knows exactly where one program ends and the next
  begins.

## Hardware

- **Board:** ESP32-C3 SuperMini (RISC-V, native USB Serial/JTAG).
- **LED:** onboard LED on **GPIO8**, **active-low** (level `0` = on, `1` = off).

## Build & flash

Requires **ESP-IDF v5.x** (tested on v5.5.1). From this `firmware/` directory:

```bash
idf.py set-target esp32c3
idf.py build
idf.py -p /dev/cu.usbmodem1101 flash
```

Replace the port with your board's (e.g. `ls /dev/cu.usbmodem*`). Use `flash`
without `monitor` — the live-push sender needs exclusive access to the port.

## Pushing a program live

With the board flashed and running, push a program from the host:

```bash
python3 tools/send.py /dev/cu.usbmodem1101 3    # blink the LED 3 times
python3 tools/send.py /dev/cu.usbmodem1101 6    # ...then 6 — no reflash
```

`send.py` serializes a program to the exact byte format the VM expects and writes
it down the USB cable. Because the wire format is a language-neutral contract, the
sender doesn't have to be C++ — anything that can emit the bytes can drive the VM.
(Requires `pyserial`: `pip install pyserial`, or use ESP-IDF's bundled Python.)

## Project structure

```
firmware/
├── CMakeLists.txt              ESP-IDF project root
├── components/
│   └── vm_core/
│       └── CMakeLists.txt      wraps the parent repo's src/vm + src/bytecode
├── main/
│   ├── CMakeLists.txt          the app component (depends on vm_core)
│   └── firmware_main.cpp       app_main(): receive loop, hardware natives
└── tools/
    └── send.py                 host-side program sender (USB serial)
```

## Roadmap

- [x] VM cross-compiles and runs on the ESP32-C3
- [x] Hardware natives (`gpio_set_level`, `delay`) via `CallNative`
- [x] Blink from bytecode
- [x] Live program push over USB (one-shot programs)
- [ ] Resumable VM — run long/infinite programs while staying able to accept a
      new one (replace the blocking run with bounded stepping + polling)
- [ ] Push programs compiled from real source (host `--emit` → bytes → board)
- [ ] Call-expression syntax in the language, so hardware can be scripted directly
- [ ] Program push over WiFi
```

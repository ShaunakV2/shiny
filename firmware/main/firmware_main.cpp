// Step 4b — live program push (Model A: one-shot programs).
//
// The VM is flashed ONCE. After boot, app_main loops forever:
//   1. read a 4-byte count header off USB serial   (how many instructions)
//   2. read the count*6 record bytes that follow
//   3. deserialize -> run the program to completion
//   4. go back to (1) and wait for the next program
//
// Changing what the board does no longer needs a reflash — you just send new
// bytes down the same USB cable you flash over.

#include <cstring>
#include <stack>
#include <vector>

#include "driver/gpio.h"
#include "driver/usb_serial_jtag.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bytecode/serialize.h"
#include "vm/vm.h"

static constexpr gpio_num_t LED = GPIO_NUM_8;

// Each serialized instruction is a fixed 6-byte record (see serialize.h:
// 1 byte kind + 1 byte hasValue + 4 byte value).
static constexpr int kRecordBytes = 6;

// Fill a VM's "phone book" with the hardware natives the bytecode can dial.
static void registerNatives(VM& vm) {
    vm.registerNative(0, [](std::stack<int>& s) {   // gpio_set_level(pin, level)
        int level = s.top(); s.pop();
        int pin   = s.top(); s.pop();
        gpio_set_level(static_cast<gpio_num_t>(pin), level);
    });
    vm.registerNative(1, [](std::stack<int>& s) {   // delay(ms)
        int ms = s.top(); s.pop();
        vTaskDelay(pdMS_TO_TICKS(ms));
    });
}

// Block until EXACTLY n bytes have been read into dst. The USB read can return
// fewer bytes than asked, so we loop until the frame is complete.
static void readExact(uint8_t* dst, size_t n) {
    size_t got = 0;
    while (got < n) {
        int r = usb_serial_jtag_read_bytes(dst + got, n - got, portMAX_DELAY);
        if (r > 0) got += static_cast<size_t>(r);
    }
}

extern "C" void app_main(void) {
    // One-time hardware + serial setup.
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    usb_serial_jtag_driver_config_t usb_cfg = USB_SERIAL_JTAG_DRIVER_CONFIG_DEFAULT();
    usb_serial_jtag_driver_install(&usb_cfg);

    while (true) {
        // (1) Read the 4-byte count header (little-endian).
        uint8_t header[4];
        readExact(header, 4);
        int32_t count = static_cast<int32_t>(
              static_cast<uint32_t>(header[0])
            | (static_cast<uint32_t>(header[1]) << 8)
            | (static_cast<uint32_t>(header[2]) << 16)
            | (static_cast<uint32_t>(header[3]) << 24));

        // Guard against a garbled/desynced frame so we never try to allocate a
        // wild amount of memory. Just drop it and wait for the next header.
        if (count < 0 || count > 4096) continue;

        // (2) Read the records. Rebuild the FULL buffer (header + records) so
        //     deserialize() sees exactly what serialize() produced.
        const size_t recordTotal = static_cast<size_t>(count) * kRecordBytes;
        std::vector<uint8_t> buf(4 + recordTotal);
        std::memcpy(buf.data(), header, 4);
        readExact(buf.data() + 4, recordTotal);

        // (3) Deserialize and run to completion on a FRESH VM (so a previous
        //     program's stack/variables never bleed into this one).
        std::vector<Instruction> program = deserialize(buf);
        VM vm;
        registerNatives(vm);
        vm.run(program);
        // (4) fall through to the top and wait for the next program.
    }
}

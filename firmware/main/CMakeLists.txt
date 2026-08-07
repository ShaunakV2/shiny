# The main component: the firmware entry point. It depends on vm_core (REQUIRES)
# so it can #include "vm/vm.h" and link the VM.
idf_component_register(
    SRCS "firmware_main.cpp"
    INCLUDE_DIRS "."
    REQUIRES vm_core
    PRIV_REQUIRES esp_driver_gpio esp_driver_usb_serial_jtag
)

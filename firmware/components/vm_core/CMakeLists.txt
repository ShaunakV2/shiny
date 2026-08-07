# vm_core — an ESP-IDF component that wraps the host compiler's bytecode VM so
# the firmware can reuse it verbatim (no copies). We deliberately register ONLY
# the VM + bytecode headers, NOT the lexer/parser/compiler: those stay on the
# host (they use RTTI/dynamic_cast and heavy headers the chip doesn't need).
#
# REPO_ROOT walks up out of firmware/components/vm_core/ to the repo root, so
# the paths keep working no matter where the repo is checked out.
set(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")

idf_component_register(
    SRCS "${REPO_ROOT}/src/vm/vm.cpp"
    INCLUDE_DIRS "${REPO_ROOT}/src"
)

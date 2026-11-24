// MicroPython's generic ESP32-C3 board; boards/firmware.cmake sets MICROPY_HW_BOARD_NAME.
#define MICROPY_HW_MCU_NAME "ESP32-C3"

#define MICROPY_HW_ENABLE_UART_REPL (1)

// A smaller initial GC heap means its one doubling leaves ESP-IDF room for Wi-Fi and TLS
// with ~44 KB of customer code loaded, where the 64 KB default leaves too little past ~22 KB.
#define MICROPY_GC_INITIAL_HEAP_SIZE (48 * 1024)

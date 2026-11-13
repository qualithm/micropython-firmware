# MicroPython's generic ESP32-C3 board, plus this firmware's frozen modules,
# name and version. Paths are relative to MicroPython's ports/esp32.
include(boards/ESP32_GENERIC_C3/mpconfigboard.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../firmware.cmake)

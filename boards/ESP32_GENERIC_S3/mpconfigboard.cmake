# MicroPython's generic ESP32-S3 board, plus this firmware's frozen modules,
# name and version. Paths are relative to MicroPython's ports/esp32.
include(boards/ESP32_GENERIC_S3/mpconfigboard.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../firmware.cmake)

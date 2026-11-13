# Shared by every board here, included from each board's mpconfigboard.cmake
# after the upstream board it builds on.

# The firmware's name, in the REPL banner and the release image names.
set(FIRMWARE_NAME micropython-firmware)

# version.txt is the firmware's version. It becomes PROJECT_VER, which ESP-IDF
# stamps into the image's app description, and joins the board name, so the
# REPL banner reads "MicroPython v<micropython> ...; <name> <version> with <chip>".
file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/../version.txt FIRMWARE_VERSION LIMIT_COUNT 1)
set(PROJECT_VER ${FIRMWARE_VERSION})
# FIRMWARE_VERSION is what the firmware module's version() parses.
list(APPEND MICROPY_DEF_BOARD
    MICROPY_HW_BOARD_NAME="${FIRMWARE_NAME} ${FIRMWARE_VERSION}"
    FIRMWARE_VERSION="${FIRMWARE_VERSION}"
)

set(MICROPY_FROZEN_MANIFEST ${CMAKE_CURRENT_LIST_DIR}/../manifest.py)

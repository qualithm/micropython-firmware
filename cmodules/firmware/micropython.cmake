# The firmware user C module: the binding plus the host-tested logic it wraps.
# boards/firmware.cmake defines FIRMWARE_VERSION from version.txt.

add_library(usermod_firmware INTERFACE)

target_sources(usermod_firmware INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/firmware_version.c
    ${CMAKE_CURRENT_LIST_DIR}/modfirmware.c
)

target_include_directories(usermod_firmware INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usermod INTERFACE usermod_firmware)

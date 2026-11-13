# The firmware user C module: the binding plus the host-tested logic it wraps.
# version.txt is the firmware's version, compiled in as FIRMWARE_VERSION.
file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/../../version.txt FIRMWARE_MODULE_VERSION LIMIT_COUNT 1)

add_library(usermod_firmware INTERFACE)

target_sources(usermod_firmware INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/firmware_version.c
    ${CMAKE_CURRENT_LIST_DIR}/modfirmware.c
)

target_include_directories(usermod_firmware INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_firmware INTERFACE
    FIRMWARE_VERSION="${FIRMWARE_MODULE_VERSION}"
)

target_link_libraries(usermod INTERFACE usermod_firmware)

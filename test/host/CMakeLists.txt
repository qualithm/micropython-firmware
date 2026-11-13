# Host tests for the user C modules' logic, built with the image's host GCC
# and ESP-IDF's copy of Unity. The MicroPython bindings (mod*.c) need the
# port's headers, so they stay out: keep logic in plain C files beside them.
cmake_minimum_required(VERSION 3.16)
project(host_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(UNITY_DIR $ENV{IDF_PATH}/components/unity/unity/src)
set(CMODULES ${CMAKE_CURRENT_LIST_DIR}/../../cmodules)

add_executable(host_test
    test_main.c
    test_firmware_version.c
    ${CMODULES}/firmware/firmware_version.c
    ${UNITY_DIR}/unity.c
)
target_include_directories(host_test PRIVATE ${CMODULES}/firmware ${UNITY_DIR})
target_compile_options(host_test PRIVATE -Wall -Wextra -Werror --coverage)
target_link_options(host_test PRIVATE --coverage)

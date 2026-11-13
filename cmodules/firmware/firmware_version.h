#pragma once

#include <stdbool.h>

/** firmware_version_parse returned successfully. */
#define FIRMWARE_VERSION_OK 0
/** The text is NULL or not MAJOR.MINOR.PATCH, or out is NULL. */
#define FIRMWARE_VERSION_ERR_INVALID (-1)

/** A firmware version, MAJOR.MINOR.PATCH. */
typedef struct {
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
} firmware_version_t;

/**
 * Parse a version of the form "MAJOR.MINOR.PATCH": three decimal numbers of at most 6 digits,
 * no sign, no prefix and no suffix.
 *
 * Pure C with no MicroPython or ESP-IDF calls, so the host tests reach it.
 *
 * @param text the version; NULL is rejected
 * @param out  where the parsed version goes; untouched on failure
 * @return FIRMWARE_VERSION_OK or FIRMWARE_VERSION_ERR_INVALID
 */
int firmware_version_parse(const char *text, firmware_version_t *out);

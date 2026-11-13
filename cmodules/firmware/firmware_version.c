#include "firmware_version.h"

#include <stddef.h>

#define PART_MAX_DIGITS 6

// Read one decimal part at *cursor, advancing past it.
static bool read_part(const char **cursor, unsigned int *part)
{
    unsigned int value = 0;
    int digits = 0;
    while (**cursor >= '0' && **cursor <= '9') {
        if (digits == PART_MAX_DIGITS) {
            return false;
        }
        value = (value * 10U) + (unsigned int)(**cursor - '0');
        digits++;
        (*cursor)++;
    }
    *part = value;
    return digits > 0;
}

int firmware_version_parse(const char *text, firmware_version_t *out)
{
    if (text == NULL || out == NULL) {
        return FIRMWARE_VERSION_ERR_INVALID;
    }
    firmware_version_t version = {0};
    const char *cursor = text;
    if (!read_part(&cursor, &version.major) || *cursor != '.') {
        return FIRMWARE_VERSION_ERR_INVALID;
    }
    cursor++;
    if (!read_part(&cursor, &version.minor) || *cursor != '.') {
        return FIRMWARE_VERSION_ERR_INVALID;
    }
    cursor++;
    if (!read_part(&cursor, &version.patch) || *cursor != '\0') {
        return FIRMWARE_VERSION_ERR_INVALID;
    }
    *out = version;
    return FIRMWARE_VERSION_OK;
}

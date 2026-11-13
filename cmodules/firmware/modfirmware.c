// The MicroPython binding for firmware_version.c: `import firmware; firmware.version()` returns
// this image's version as (major, minor, patch). Board glue the host tests never build.

#include "firmware_version.h"

#include "py/obj.h"
#include "py/runtime.h"

#ifndef FIRMWARE_VERSION
#error "FIRMWARE_VERSION must be defined by boards/firmware.cmake"
#endif

static mp_obj_t firmware_version(void)
{
    firmware_version_t version;
    if (firmware_version_parse(FIRMWARE_VERSION, &version) != FIRMWARE_VERSION_OK) {
        mp_raise_ValueError(MP_ERROR_TEXT("version.txt is not MAJOR.MINOR.PATCH"));
    }
    mp_obj_t parts[3] = {
        mp_obj_new_int_from_uint(version.major),
        mp_obj_new_int_from_uint(version.minor),
        mp_obj_new_int_from_uint(version.patch),
    };
    return mp_obj_new_tuple(3, parts);
}
static MP_DEFINE_CONST_FUN_OBJ_0(firmware_version_obj, firmware_version);

static const mp_rom_map_elem_t firmware_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_firmware)},
    {MP_ROM_QSTR(MP_QSTR_version), MP_ROM_PTR(&firmware_version_obj)},
};
static MP_DEFINE_CONST_DICT(firmware_module_globals, firmware_module_globals_table);

const mp_obj_module_t firmware_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&firmware_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_firmware, firmware_user_cmodule);

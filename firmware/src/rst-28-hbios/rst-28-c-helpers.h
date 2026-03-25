#ifndef __RST28_C_HELPERS
#define __RST28_C_HELPERS

#include <stdint.h>

const char *diodevice_getdriver_name(const uint8_t hbios_unit);
const char *diodevice_getattributes_name(const uint8_t hbios_unit);

const char *ciodevice_getdriver_name(const uint8_t hbios_unit);
const char *ciodevice_getattributes_name(const uint8_t hbios_unit);

#endif

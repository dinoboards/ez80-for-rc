#include "rst-28-c-helpers.h"
#include "../rst-28-vars.h"

const char *diodevice_getdriver_name(const uint8_t hbios_unit) {
  switch (diodevice_getdriver(hbios_unit)) {
  case DIODEV_RF:
    return "RF";

  case DIODEV_USB:
    return "USB";

  default:
    return "UNK";
  }
}

typedef const char *string;

const static string high_cap_names[16] = {"Hard Disk", "Compact Flash", "SD Card", "USB Drive", "ROM Disk",     "RAM Disk",
                                          "Flash ROM", "RAM Floppy",    "CD-ROM",  "Cartridge", "Mass Storage", "Floppy",
                                          "Unknown",   "Unknown",       "Unknown", "Unknown"};

const char *diodevice_getattributes_name(const uint8_t hbios_unit) {
  const uint8_t high_capacity = diodevice_getattributes(hbios_unit) & 15;

  return high_cap_names[high_capacity];
}

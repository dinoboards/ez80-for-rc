#include "../print-string.h"
#include "../rst-28-hbios/rst-28-c-helpers.h"
#include "../rst-28-vars.h"
#include "fbios-vars.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Iterate all fbios drives with an installed disk_reference
 * and report to console
 *
 */
void fbios_mount_report() {
  uint8_t          fbios_unit;
  fbios_storage_t *f;

  printf("\r\nConfiguring Drives...\r\n\r\n");

  f = &fbios_vars->fb_storage_units[0];
  for (fbios_unit = 0; fbios_unit < FBIOS_MAX_STORAGE_UNITS; fbios_unit++, f++) {
    if (f->disk_reference == 0)
      continue;

    printf("        %c:=%s%d:%d", 'A' + fbios_unit, diodevice_getdriver_name(f->hbios_unit), diodevice_getnumber(f->hbios_unit),
           f->slice);
    if (fbios_vars->fb_boot_disk == fbios_unit)
      print_string("*");
    print_string("\r\n");
  }

  printf("\r\n\r\n");
}

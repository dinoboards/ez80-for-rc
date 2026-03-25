#include "../print-string.h"
#include "../rst-28-vars.h"
#include "rst-28-c-helpers.h"
#include "rst-28-disk-inspection.h"
#include <stdint.h>
#include <stdio.h>

static const uint8_t stparmap[] = "NONENMNS";

void report_hbios_drivers() {
  uint8_t hbios_unit;
  char    buffer[32];

  const uint8_t ciocnt = sysget_ciocount();
  const uint8_t diocnt = sysget_diocount();

  print_string("\r\n"
               "Unit        Device      Type              Capacity/Mode\r\n"
               "----------  ----------  ----------------  --------------------\r\n");

  for (hbios_unit = 0; hbios_unit < ciocnt; hbios_unit++) {
    sprintf(buffer, "%s%d", ciodevice_getdriver_name(hbios_unit), ciodevice_getnumber(hbios_unit));

    {
      const uint8_t line_control = cioquery_get_line_control(hbios_unit);
      const uint8_t data_bits    = (line_control & 3) + 5;
      const char    parity       = stparmap[(line_control >> 3) & 6];
      const uint8_t stop_bits    = ((line_control >> 2) & 1) + 1;

      printf("Char %-7d%-12s%-18s%d,%d,%c,%d\r\n", hbios_unit, buffer, ciodevice_getattributes_name(hbios_unit),
             cioquery_get_baud_rate(hbios_unit), data_bits, parity, stop_bits);
    }
  }

  for (hbios_unit = 0; hbios_unit < diocnt; hbios_unit++) {
    sprintf(buffer, "%s%d", diodevice_getdriver_name(hbios_unit), diodevice_getnumber(hbios_unit));

    printf("Disk %-7d%-12s%-18s", hbios_unit, buffer, diodevice_getattributes_name(hbios_unit));

    convert_and_print_disk_size(diocapacity_get_sectors(hbios_unit));

    print_string("\r\n");
  }
}

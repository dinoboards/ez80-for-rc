#include "rst-28-disk-inspection.h"
#include "../print-string.h"
#include <stdint.h>

void convert_and_print_disk_size(uint32_t number_of_sectors) {
  if (number_of_sectors == 1474560 / 512) {
    print_string("1.44MB");
    return;
  }

  {
    const char *suffix = "KB";

    if (number_of_sectors <= 2880) {
      print_uint16(number_of_sectors / 2);
      print_string(suffix);
      return;
    }

    number_of_sectors /= 256;
    suffix = "MB";

    if (number_of_sectors >= 8192) {
      number_of_sectors /= 1024;
      suffix = "GB";
    }

    {
      const uint16_t a = (uint16_t)(number_of_sectors / 8);
      const uint16_t b = (uint16_t)(number_of_sectors % 8 * 100) / 8;

      print_uint16(a);

      if (b != 0) {
        print_string(".");
        print_uint16(b);
      }
      print_string(suffix);
    }
  }
}

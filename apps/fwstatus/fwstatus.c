#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <ifl.h>
#include <stdint.h>
#include <stdio.h>

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  uint8_t  data[2];
  uint32_t stat = IFL_ReadInfoPage(data, 0xFE, 2);
  if (stat != 0) {
    printf("Error reading flash info page: %02X\n", stat);
    return 1;
  }

  printf("ALT-FIRMWARE-STATE: %02X, %02X\n", data[0], data[1]);

  uint8_t marker[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  IFL_Read(marker, 0x01FF00, 15);

  printf("ALT-FIRMWARE-MARKER: %s\n", marker);

  return 0;
}

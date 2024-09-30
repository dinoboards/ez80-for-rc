#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <ifl.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t zeroed[2] = {0, 0};

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  uint8_t stat = IFL_ErasePages(0x01FC00, 1);
  if (stat != 0) {
    printf("Erasing flash errored: %02X\n", stat);
    return 1;
  }

  stat = IFL_ProgramInfoPage(0xFE, zeroed, 2);

  printf("On next boot, main firmware will be loaded\n");
  return 0;
}

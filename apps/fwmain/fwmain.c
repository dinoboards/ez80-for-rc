#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <ifl.h>
#include <stdint.h>
#include <stdio.h>

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  const uint8_t stat = IFL_ErasePages(0x01FC00, 1);
  printf("IFL_ErasePages: %02X\n", stat);
  printf("On next boot, main bios will be loaded\n");
  return 0;
}

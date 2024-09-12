#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <ifl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ALT_FIRMWARE_BOOT_INFO  0xFE
#define ALT_FIRMWARE_STAT       1
#define ALT_FIRMWARE_BOOT_COUNT 0

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  static uint8_t  boot_info[2];
  static uint32_t stat;
  static uint8_t  marker[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  IFL_Read(marker, 0x01FF00, 15);
  if (strncmp((char *)marker, "RC2014", 6) != 0) {
    printf("No alternate firmware marker found\n");
    return 1;
  }

  printf("ALT-FIRMWARE-ID: %s\n", marker);

  stat = IFL_ReadInfoPage(boot_info, ALT_FIRMWARE_BOOT_INFO, 2);
  if (stat != 0) {
    printf("Error reading flash info page: %02X\n", stat);
    return 1;
  }

  if (boot_info[ALT_FIRMWARE_STAT] == 0x5A && boot_info[ALT_FIRMWARE_BOOT_COUNT] == 0) {
    printf("Alternate firmware installed; yet to be booted\n");
    return 0;
  }

  if (boot_info[ALT_FIRMWARE_STAT] == 0x5A && boot_info[ALT_FIRMWARE_BOOT_COUNT] != 0) {
    printf("Alternate firmware installed; failed to boot %u times\n", boot_info[ALT_FIRMWARE_BOOT_COUNT]);
    return 0;
  }

  if (boot_info[ALT_FIRMWARE_STAT] == 0x2F) {
    printf("Alternate firmware installed; booted successfully\n");
    return 0;
  }

  printf("Unknown alternate firmware status: %02X\n", boot_info[ALT_FIRMWARE_STAT]);

  return 0;
}

#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include "hex-record.h"
#include <ifl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int8_t emit_to_null(const uint32_t offset, const uint8_t *data, const uint16_t len) {
  offset;
  data;
  len;

  return ZFL_ERR_SUCCESS;
}

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;

  static FILE  *file;
  static int8_t stat;

  printf("eZ80 for RCBus Firmware Updater\n");
  printf("Version 1.0 (C) Copyright Dean Netherton 2024-09-12\n\r\n");
  printf("Reading Firmware file: FW.HEX\n\n");

  file = fopen("fw.hex", "r");
  if (file == NULL) {
    printf("Error opening file FW.HEX\n");
    return 1;
  }

  stat = IFL_Init();
  if (stat) {
    printf("Failure initializing flash %d\n", stat);
    return 1;
  }

  stat = IFL_ErasePages(0x01FC00, 1);
  if (stat) {
    printf("Flash erase failure: %02X\n", stat);
    return 1;
  }

  stat = IFL_ErasePages(0x010000, 64);
  if (stat) {
    printf("Flash erase failure: %02X\n", stat);
    return 1;
  }

  stat = process_hex_records(file, IFL_Program);
  printf("\r                                              \n");
  fclose(file);

  if (stat) {
    fclose(file);
    printf("Flash write failure\n");
    return 1;
  }

  stat = IFL_WriteInfoByte(0xFF, 0x5A);
  if (stat) {
    printf("Flash write failure\n");
    return 1;
  }

  stat = IFL_WriteInfoByte(0xFE, 0x00);
  if (stat) {
    printf("Flash write failure\n");
    return 1;
  }

  printf("Firmware update complete\n");
  return 0;
}

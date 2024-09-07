#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <ifl.h>
#include <stdint.h>
#include <stdio.h>

#include "hex-record.h"
/*

New firmware model

First 64K is the main bios, and not typically updated - a good know simple bios
The second 64K is an alt bios, that can be switched over to from the main bios

00 0000 -> 00 FFFF : 64K - main bios - with support for booting alt bios
01 0000 -> 01 FFFF : 64K - alt bios - no need for alt bios support


startup code:
  1. configures ROM/RAM, disables interrupts, no handlers, and other I/O defaulted-disabled
  2. do a read and calculate a checksum for echo 60k bios image
  3. read special flash to discover current boot bios
  5. if boot bios X is marked as booting, try the other BIOS
  6. if both BIOS are marked as booting, then bios corrupted.
  4. mark special flash byte to say booting bios X
  4. boot the bios
  5. expect bios to boot and update the special flash to say all ok

  1. configures ROM/RAM, disables interrupts, no handlers, and other I/O defaulted-disabled
  2. read special flash flag, to see if alt bios available
  3. if alt bios was booting, then mark alt bios as bad and not available, and continue with main bios
  4. if alt bios was not booting and marked available, then mark as booting
  5. boot the alt bios
  6. expect alt bios to boot and update the special flash to say all ok

> HBIOS must invoke RST %10 function to indicate successful boot - perhaps after menu selection?
> perhaps an additional option to manually mark as working?


*/

/* App to reflash the on chip rom */

// Design constraint: can not use any RST %10 helpers - including IO - as the
// ROM will go off line Interrupts must therefore also be disabled until
// completed. System probably needs to be rebooted after completion
//

void spike_flash_info(void) {
  uint8_t data1[6] = {0, 0, 0, 0, 0, 0};
  uint8_t stat     = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);

  stat = IFL_WriteInfoByte(0, 0x55);
  printf("IFL_WriteInfoByte: %02X\n", stat);

  stat = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);

  stat = IFL_WriteInfoByte(1, 0x66);
  printf("IFL_WriteInfoByte: %02X\n", stat);

  stat = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);

  stat = IFL_WriteInfoByte(2, data1[2] + 1);
  printf("IFL_WriteInfoByte: %02X\n", stat);

  stat = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);
  // stat = IFL_EraseInfoPage();
  // printf("IFL_EraseInfoPage: %02X\n", stat);

  // stat = IFL_ReadInfoPage(data1, 0, 2);
  // printf("IFL_ReadInfoPage: %02X, %02X, %02X\n", stat, data1[0], data1[1]);

  // uint8_t data[4] = {0x55, 0x66, 0x77, 0x88};
  // stat            = IFL_ProgramInfoPage(0, data, 4);
  // printf("IFL_ProgramInfoPage: %02X\n", stat);

  stat = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);
}

void spike_flash_read_write(void) {
  uint32_t flash = (uint32_t)0x010000;

  int8_t stat = IFL_ErasePages(flash, 1);
  printf("IFL_ErasePages: %02X\n", stat);

  uint8_t data[4] = {0x55, 0x66, 0x77, 0x88};
  stat            = IFL_Program(flash, data, 4);
  printf("IFL_Program: %02X\n", stat);
}

int8_t emit_to_null(const uint32_t offset, const uint8_t *data, const uint16_t len) {
  offset;
  data;
  len;

  return ZFL_ERR_SUCCESS;
}

// int8_t emit_to_flash(const uint32_t offset, const uint8_t *data, const uint8_t len) {
//   return IFL_Program(offset, data, len);
// }

uint8_t main(const int argc, char *argv[]) {
  argc;
  argv;
  printf("Warning.  Potentially dangerous operation.\n");

  static FILE *file;
  static int8_t stat;

  stat = IFL_Init();
  if (stat) {
    printf("Failure initializing flash %d\n", stat);
    return 1;
  }

  // FILE *file = fopen("fw.hex", "r");
  // if (file == NULL) {
  //   printf("Error opening file");
  //   return 1;
  // }

  // stat = process_hex_records(file, emit_to_null);
  // fclose(file);

  // if (stat) {
  //   fclose(file);
  //   printf("Failure processing hex records\n");
  //   return 1;
  // }

  stat = IFL_ErasePages(0x010000, 64);
  if (stat) {
    printf("Flash erase failure: %02X\n", stat);
    return 1;
  }

  file = fopen("fw.hex", "r");
  if (file == NULL) {
    printf("Error opening file");
    return 1;
  }

  stat = process_hex_records(file, IFL_Program);
  fclose(file);

  if (stat) {
    fclose(file);
    printf("Flash write failure\n");
    return 1;
  }

  // parse command line and identify filename
  // open file for reading

  return 0;
}

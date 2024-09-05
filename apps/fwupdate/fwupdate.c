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
  uint8_t data1[6] = {0x11, 0x22, 0x33, 0x44, 0xAA, 0xBB};
  uint8_t stat     = IFL_ReadInfoPage(data1, 0, 6);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", stat, data1[0], data1[1], data1[2], data1[3], data1[4],
         data1[5]);

  stat = IFL_EraseInfoPage();
  printf("IFL_EraseInfoPage: %02X\n", stat);

  stat = IFL_ReadInfoPage(data1, 0, 2);
  printf("IFL_ReadInfoPage: %02X, %02X, %02X\n", stat, data1[0], data1[1]);

  uint8_t data[4] = {0x55, 0x66, 0x77, 0x88};
  stat            = IFL_ProgramInfoPage(0, data, 4);
  printf("IFL_ProgramInfoPage: %02X\n", stat);

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

uint8_t main(const int argc, char *argv[]) {
  argc;
  argv;
  printf("Warning.  Potentially dangerous operation.\n");

  int8_t stat = IFL_Init();
  printf("IFL_Init: %02X\n", stat);

  spike_flash_info();

  spike_flash_read_write();

  // // spike call into library
  // addr_24_t addr = 0x56789A;
  // uint16_t  len  = 0x1234;
  // uint8_t   stat = IFL_IsAddrValid(addr, len);
  // printf("Stat: %02X\n", stat);

  // // Open the file 'fw.hex' for reading
  // FILE *file = fopen("fw.hex", "r");
  // if (file == NULL)
  // {
  //   perror("Error opening file");
  //   return 1;
  // }

  // stat = process_hex_record(file);

  // // // Loop over each character in the file
  // // int ch;
  // // while ((ch = fgetc(file)) != EOF)
  // // {
  // //   // Process the character (for now, just print it)
  // //   putchar(ch);
  // // }

  // // Close the file
  // fclose(file);

  // parse command line and identify filename
  // open file for reading

  return 0;
}
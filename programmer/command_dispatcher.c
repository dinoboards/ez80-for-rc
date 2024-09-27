#include "read_line.h"
#include "zdi.h"
#include <stdio.h>
#include <string.h>

/*

Commands:

help or h
  Display this help message

status or s
  Display the ZDI status register (ZDI_STAT)


*/

void process_status_command(void);

void process_command() {
  printf("\r\n");

  if (strcmp(input_buffer, "help") == 0 || strcmp(input_buffer, "h") == 0) {
    printf("Commands:\r\n");
    printf("\r\n");
    printf("help or h\r\n");
    printf("  Display this help message\r\n");
    printf("\r\n");
    printf("status or s\r\n");
    printf("  Display the ZDI status register (ZDI_STAT)\r\n");
    printf("\r\n");
  } else if (strcmp(input_buffer, "status") == 0 || strcmp(input_buffer, "s") == 0) {
    process_status_command();
  } else {
    printf("Unknown command: %s\r\n", input_buffer);
  }

  printf("\r\n");
}

void process_status_command(void) {
  uint8_t status = read_reg_byte(ZDI_RD_STAT);

  printf("ZDI_STAT: 0x%02X (", status);
  printf("ACTIVE: %s; ", status & ZDI_STAT_ACTIVE ? "YES" : "NO");
  printf("HALT_SLP: %s; ", status & ZDI_STAT_HALT_SLP ? "YES" : "NO");
  printf("CPU MODE: %s; ", status & ZDI_STAT_ADL ? "ADL" : "Z80");
  printf("MADL: %s; ", status & ZDI_STAT_MADL ? "YES" : "NO");
  printf("INT: %s)\r\n", status & ZDI_STAT_IEF1 ? "EI" : "DI");
}

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

void process_command() {

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
    printf("ZDI_STAT: 0x%02x\r\n", read_reg_byte(0x00));
  } else {
    printf("Unknown command: %s\r\n", input_buffer);
  }

  printf("\r\n");
}

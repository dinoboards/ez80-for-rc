#include "firmware_version.h"
#include "hex_record.h"
#include "ifl.h"
#include "read_line.h"
#include "utils.h"
#include "zdi.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*

Commands:

help or h
  Display this help message

status or s
  Display the ZDI status register (ZDI_STAT)

flash [upload]
  Flash the eZ80 with a new firmware image - intel hex file

break or b
  break the ez80

continue or c
  continue the ez80

reset
  reset the ez80 (internal only reset line is not triggered)

mode or m [ADL|Z80]
  Set the CPU mode to ADL or Z80

set cpu-freq [FREQ]
  Set the CPU frequency in Mhz or Hz

set pc [ADDR]
  Set the PC register to the specified address

wr or read [ADDR]
  Read 16 bytes from the eZ80 memory

reboot
  Reset and restart the eZ80 firmware

*/

void process_status_command(void);
void process_flash_command(void);
void process_break_command(void);
void process_continue_command(void);
void process_reset_command(void);
void process_mode_command(void);
void process_set_command(void);
void process_read_command(void);
void process_reboot_command(void);

void process_command(void) {
  printf("\r\n");

  to_lowercase(input_buffer);
  strtok(input_buffer, " ");

  if (input_buffer[0] == '\0')
    ; // empty line buffer - so ignore
  else if (strcmp(input_buffer, "help") == 0 || strcmp(input_buffer, "h") == 0) {
    printf("Commands:\r\n"
           "\r\n"
           "help or h\r\n"
           "  Display this help message\r\n"
           "\r\n"
           "status or s\r\n"
           "  Display the ZDI status register (ZDI_STAT)\r\n"
           "\r\n"
           "flash [upload]\r\n"
           "  Flash the eZ80 with a new firmware image\r\n"
           "  default loads the firmware stored on Pi Pico\r\n"
           "  upload: expects an intel hex file to be streamed over stdin\r\n"
           "\r\n"
           "break or b\r\n"
           "  break the ez80  \r\n"
           "\r\n"
           "continue or c\r\n"
           "  continue the ez80\r\n"
           "\r\n"
           "reset\r\n"
           "  reset the ez80 (internal only reset line is not triggered)\r\n"
           "\r\n"
           "mode or m [ADL|Z80]\r\n"
           "  Set the CPU mode to ADL or Z80\r\n"
           "\r\n"
           "set cpu-freq [FREQ]\r\n"
           "  Set the CPU frequency in Mhz or Hz\r\n"
           "\r\n"
           "set pc [ADDR]\r\n"
           "  Set the PC register to the specified address\r\n"
           "\r\n"
           "wr or write [ADDR] [DATA]\r\n"
           "  Write a byte to the eZ80 memory\r\n"
           "\r\n"
           "rd or read [ADDR]\r\n"
           "  Read a byte from the eZ80 memory\r\n"
           "\r\n"
           "reboot\r\n"
           "  Reset and restart the eZ80 firmware\r\n");

    return;
  } else if (strcmp(input_buffer, "status") == 0 || strcmp(input_buffer, "s") == 0) {
    process_status_command();
  } else if (strcmp(input_buffer, "flash") == 0 || strcmp(input_buffer, "f") == 0) {
    process_flash_command();
  } else if (strcmp(input_buffer, "break") == 0 || strcmp(input_buffer, "b") == 0) {
    process_break_command();
  } else if (strcmp(input_buffer, "continue") == 0 || strcmp(input_buffer, "c") == 0) {
    process_continue_command();
  } else if (strcmp(input_buffer, "reset") == 0) {
    process_reset_command();
  } else if (strcmp(input_buffer, "mode") == 0 || strcmp(input_buffer, "m") == 0) {
    process_mode_command();
  } else if (strcmp(input_buffer, "set") == 0) {
    process_set_command();
  } else if (strcmp(input_buffer, "rd") == 0 || strcmp(input_buffer, "read") == 0) {
    process_read_command();
  } else if (strcmp(input_buffer, "reboot") == 0) {
    process_reboot_command();
  } else {
    printf("Unknown command: %s\r\n", input_buffer);
  }

  printf("\r\n");
}

void process_status_command(void) {
  uint8_t status = zdi_rd_reg_byte(ZDI_RD_STAT);

  printf("ZDI_STAT: 0x%02X (", status);
  printf("ACTIVE: %s; ", status & ZDI_STAT_ACTIVE ? "YES" : "NO");
  printf("HALT_SLP: %s; ", status & ZDI_STAT_HALT_SLP ? "YES" : "NO");
  printf("CPU MODE: %s; ", status & ZDI_STAT_ADL ? "ADL" : "Z80");
  printf("MADL: %s; ", status & ZDI_STAT_MADL ? "YES" : "NO");
  printf("INT: %s)\r\n", status & ZDI_STAT_IEF1 ? "EI" : "DI");

  uint32_t pc = read_reg_pc();
  printf("PC: 0x%06lX\r\n", pc);
}

int8_t emit_to_flash(const uint32_t offset, const uint8_t *data, const uint16_t len) {
  zdi_flash_write_bytes(offset, data, len);
  return ZFL_ERR_SUCCESS;
}

void process_flash_command(void) {
  bool  internal = true;
  char *mode     = strtok(NULL, " ");
  if (mode == NULL) {
    printf("Flashing eZ80 with firmware %d.%d.%d.%d (%s)\r\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, REVISION_VERSION,
           FIRMWARE_DATE);
  }

  if (strcmp(mode, "upload") == 0) {
    printf("Paste the intel hex file contents now..\r\n");
    internal = false;
  }

  zdi_full_reset();
  zdi_flash_write_enable();
  zdi_erase_flash();

  process_hex_records(internal, emit_to_flash);

  zdi_flash_write_disable();
  printf("\r\n");
}

void process_break_command(void) {
  printf("Pausing the eZ80\r\n");
  zdi_debug_break();
}

void process_continue_command(void) {
  printf("Continuing the eZ80\r\n");
  zdi_debug_continue();
}

void process_reset_command(void) {
  zdi_full_reset();
  printf("eZ80 Reset: PC: 0, MODE: ADL, INT: DI, OnChip IO: Reset State\r\n");
}

void process_mode_command(void) {
  char *mode = strtok(NULL, " ");
  if (mode == NULL) {
    printf("Mode command requires a mode (ADL or Z80)\r\n");
    return;
  }

  if (strcmp(mode, "adl") == 0) {
    zdi_set_mode_adl();
    printf("Set CPU mode to ADL\r\n");
  } else if (strcmp(mode, "z80") == 0) {
    zdi_set_mode_z80();
    printf("Set CPU mode to Z80\r\n");
  } else {
    printf("Unknown mode: %s\r\n", mode);
  }
}

void set_cpu_freq(void);
void set_pc_addr(void);

void process_set_command(void) {
  const char *command = strtok(NULL, " ");
  if (command == NULL) {
    printf("Set command requires a command (cpu-freq)\r\n");
    return;
  }

  if (strcmp(command, "cpu-freq") == 0) {
    set_cpu_freq();
  } else if (strcmp(command, "pc") == 0) {
    set_pc_addr();
  } else {
    printf("Unknown set command: %s\r\n", command);
  }
}

void set_cpu_freq() {
  const char *freq_str = strtok(NULL, " ");
  if (freq_str == NULL) {
    printf("Set cpu-freq command requires a frequency (Mhz or full number)\r\n");
    return;
  }

  const uint32_t freq = parse_frequency(freq_str);
  if (freq == 0) {
    printf("Invalid frequency: %s\r\n", freq_str);
    return;
  }

  zdi_set_cpu_freq(freq);

  printf("Set CPU frequency to %ld\r\n", freq);
}

void set_pc_addr(void) {
  const char *addr_str = strtok(NULL, " ");
  if (addr_str == NULL) {
    printf("Set pc command requires an address\r\n");
    return;
  }

  const uint32_t addr = strtoul(addr_str, NULL, 16);

  write_reg_pc(addr);

  printf("PC assigned to 0x%06lX\r\n", addr);
}

void process_read_command(void) {
  const char *addr_str = strtok(NULL, " ");
  if (addr_str == NULL) {
    printf("Read command requires an address\r\n");
    return;
  }

  const uint32_t addr = strtoul(addr_str, NULL, 16);

  uint32_t pc = read_reg_pc();

  uint8_t data[16];
  for (int i = 0; i < 16; i++) {
    data[i] = zdi_read_byte(addr + i);
  }

  printf("0x%06lX: ", addr);
  for (int i = 0; i < 16; i++) {
    printf("0x%02X ", data[i]);
  }
  printf("\r\n");

  write_reg_pc(pc);
}

void process_reboot_command(void) {
  zdi_debug_break();
  zdi_full_reset();
  write_reg_pc(0);
  zdi_debug_continue();

  printf("eZ80 Reset: PC: 0, ADL: Z80, INT: DI, OnChip IO: Reset State\r\n");
}

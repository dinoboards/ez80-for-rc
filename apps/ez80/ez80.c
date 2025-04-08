#include "ez80.h"
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help() {
  printf("Usage: ez80 [options]\r\n");
  printf("Options:\r\n");
  printf("  -F=<number>W      Set On-chip Flash Wait States\r\n");
  printf("  -M=<number>[W|B]  Set Main Memory Wait States or Bus Cycles (CS3)\r\n");
  printf("  -M0=<number>[W|B] Set Extended Memory Wait States or Bus Cycles (CS0)\r\n");
  printf("  -M1=<number>[W|B] Set Extended Memory Wait States or Bus Cycles (CS1)\r\n");
  printf("                    Wait States: 0-7, Bus Cycles: 1-15\r\n");
  printf("  -I=<number>[W|B]  Set I/O Wait States or Bus Cycles (CS2)\r\n");
  printf("  -S                Scan extended memory\r\n");
  printf("  -P                Measure Main Memory Performance\r\n");
  printf("  -T=<number>       Set tick frequency rate (50 or 60)\r\n");
  printf("  -? /?             Show this help message\r\n");
}

#define CMD_NONE   0
#define CMD_HELP   1
#define CMD_M      2
#define CMD_I      4
#define CMD_M0     8
#define CMD_M1     16
#define CMD_S      32
#define CMD_P      64
#define CMD_F      128
#define CMD_T_SET  256
#define CMD_T_SHOW 512

static mem_config_t flash_config;
static mem_config_t mem_config;
static mem_config_t io_config;
static mem_config_t mem0_config;
static mem_config_t mem1_config;
static uint8_t      tick_value;
static uint24_t     cmd = 0;

// if just -T, then CMD_T_SHOW
// if -T=<number>, then CMD_T_SET
bool argument_T(const char *arg) {
  if (strncmp(arg, "-T=", 3) == 0 || strncmp(arg, "/T=", 3) == 0) {
    cmd |= CMD_T_SET;
    validate_number(arg + 3, &tick_value);

    return true;
  }

  if (strcmp(arg, "-T") == 0 || strcmp(arg, "/T") == 0) {
    cmd |= CMD_T_SHOW;
    return true;
  }

  return false;
}

bool argument_F(const char *arg) {
  if (strncmp(arg, "-F=", 3) != 0 && strncmp(arg, "/F=", 3) != 0)
    return false;

  if (cmd & CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd |= CMD_F;
  validate_wait_only_set_value(arg + 3, &flash_config);

  return true;
}

bool argument_M(const char *arg) {
  if (strncmp(arg, "-M=", 3) != 0 && strncmp(arg, "/M=", 3) != 0)
    return false;

  if (cmd & CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd |= CMD_M;
  validate_mem_set_value(arg + 3, &mem_config);

  return true;
}

bool argument_I(const char *arg) {
  if (strncmp(arg, "-I=", 3) != 0 && strncmp(arg, "/I=", 3) != 0)
    return false;

  if (cmd & CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd |= CMD_I;
  validate_mem_set_value(arg + 3, &io_config);

  return true;
}

bool argument_M0(const char *arg) {
  if (strncmp(arg, "-M0=", 4) != 0 && strncmp(arg, "/M0=", 4) != 0)
    return false;

  if (cmd & CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd |= CMD_M0;
  validate_mem_set_value(arg + 4, &mem0_config);

  return true;
}

bool argument_M1(const char *arg) {
  if (strncmp(arg, "-M1=", 4) != 0 && strncmp(arg, "/M1=", 4) != 0)
    return false;

  if (cmd & CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd |= CMD_M1;
  validate_mem_set_value(arg + 4, &mem1_config);

  return true;
}

bool argument_P(const char *arg) {
  if (strcmp(arg, "-P") != 0 && strcmp(arg, "/P") != 0)
    return false;

  if (cmd & ~CMD_P) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd = CMD_P;
  return true;
}

bool argument_S(const char *arg) {
  if (strcmp(arg, "-S") != 0 && strcmp(arg, "/S") != 0)
    return false;

  if (cmd & ~CMD_S) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd = CMD_S;
  return true;
}

bool argument_help(const char *arg) {
  if (strcmp(arg, "-?") == 0 || strcmp(arg, "/?") == 0) {
    show_help();
    exit(0);
  }

  return false;
}

int main(const int argc, const char *argv[]) {
  find_ns_timing_for_memory();

  if (argc == 1) {
    report_memory_timing();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (argument_M(argv[i]))
      continue;

    if (argument_I(argv[i]))
      continue;

    if (argument_M0(argv[i]))
      continue;

    if (argument_M1(argv[i]))
      continue;

    if (argument_S(argv[i]))
      continue;

    if (argument_P(argv[i]))
      continue;

    if (argument_F(argv[i]))
      continue;

    if (argument_T(argv[i]))
      continue;

    if (argument_help(argv[i]))
      continue;

    printf("Unknown argument: %s\r\n", argv[i]);
    show_help();
    return 1;
  }

  if (cmd == CMD_P) {
    report_main_memory_performance();
    return 0;
  }

  if (cmd == CMD_S) {
    find_extended_memory();
    return 0;
  }

  if (cmd & CMD_F)
    config_flash(flash_config);

  if (cmd & CMD_M)
    config_mem(mem_config);

  if (cmd & CMD_I)
    config_io(io_config);

  if (cmd & CMD_M0)
    config_mem0(mem0_config);

  if (cmd & CMD_M1)
    config_mem1(mem1_config);

  if (cmd & CMD_T_SET)
    config_set_tick_rate(tick_value);

  report_memory_timing();
  return 0;
}

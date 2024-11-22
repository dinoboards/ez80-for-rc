#include "ez80.h"
#include "ez80-firmware.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help() {
  printf("Usage: ez80 [options]\r\n");
  printf("Options:\r\n");
  printf("  -M0=number[W|B]  Set Extended Memory Wait States or Bus Cycles (CS0)\r\n");
  printf("                   Wait States: 0-7, Bus Cycles: 1-15\r\n");
  printf("  -S0              Scan extended memory\r\n");
  printf("  -? /?            Show this help message\r\n");
}

typedef enum { CMD_NONE, CMD_HELP, CMD_M0, CMD_S0 } mem0_type_t;

static mem0_type_t cmd       = CMD_NONE;
static const char *cmd_value = NULL;

bool argument_M0(const char *arg) {
  if (strncmp(arg, "-M0=", 4) != 0 && strncmp(arg, "/M0=", 4) != 0)
    return false;

  if (cmd != CMD_NONE) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd       = CMD_M0;
  cmd_value = arg + 4;

  return true;
}

bool argument_S0(const char *arg) {
  if (strcmp(arg, "-S0") != 0 && strcmp(arg, "/S0") != 0)
    return false;

  if (cmd != CMD_NONE) {
    printf("Error: Conflicting options.\r\n");
    show_help();
    abort();
  }

  cmd = CMD_S0;
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

  if (argc == 1) {
    report_memory_timing();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (argument_M0(argv[i]))
      continue;

    if (argument_S0(argv[i]))
      continue;

    if (argument_help(argv[i]))
      continue;

    printf("Unknown argument: %s\r\n", argv[i]);
    show_help();
    return 1;
  }

  if (cmd == CMD_M0)
    config_mem0(cmd_value);

  else if (cmd == CMD_S0)
    find_extended_memory();

  return 0;
}

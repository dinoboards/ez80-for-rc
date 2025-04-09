#include "ez80.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validate_mem_set_value(const char *value, mem_config_t *mem_config) {
  mem_config->value = atoi(value);
  char type         = value[strlen(value) - 1];

  if (type == 'W') {
    if (mem_config->value >= 0 && mem_config->value <= 7) {
      mem_config->type = WAIT_STATE;
      return;
    }

    printf("Invalid wait state value: %s\n\n", value);
    show_help();
    exit(1);
  }

  if (type == 'B') {
    if (mem_config->value >= 1 && mem_config->value <= 15) {
      mem_config->type = BUS_CYCLE;
      return;
    }

    printf("Invalid bus cycle value: %s\n\n", value);
    show_help();
    exit(1);
  }

  printf("Invalid value: %s\n\n", value);
  show_help();
  exit(1);
}

void validate_wait_only_set_value(const char *value, mem_config_t *mem_config) {
  mem_config->value = atoi(value);
  char type         = value[strlen(value) - 1];

  if (type == 'W') {
    if (mem_config->value >= 0 && mem_config->value <= 7) {
      mem_config->type = WAIT_STATE;
      return;
    }

    printf("Invalid wait state value: %s\n\n", value);
    show_help();
    exit(1);
  }

  printf("Invalid value: %s\n\n", value);
  show_help();
  exit(1);
}

void validate_number(const char *value, uint8_t *number) { *number = atoi(value); }

#include "ez80.h"
#include <stdint.h>
#include <stdio.h>

static const uint8_t        test_values[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                               0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
static volatile uint8_t    *ptr;
static uint8_t *const       start              = (uint8_t *)0x200000;
static const uint8_t *const end                = (uint8_t *)0xFFFFFF;
static uint24_t             installed_memory   = 0;
static uint24_t             progress_increment = 0x10000; // 64K increment
static uint24_t             next_progress;
static uint8_t             *current_progress;
static int                  i;

void find_extended_memory(void) {
  next_progress    = progress_increment;
  current_progress = start;
  printf("Searching for memory installed from 0x200000\r\n");
  for (ptr = start; ptr <= end; ptr += 16) {

    for (i = 0; i < 16; i++)
      ptr[i] = test_values[i];

    for (i = 0; i < 16; i++)
      if (ptr[i] != test_values[i])
        goto completed;

    installed_memory += 16;

    if (installed_memory >= next_progress) {
      printf("\r\nFound %ukb bytes at %p", installed_memory / 1024, current_progress);
      current_progress += progress_increment;
      next_progress += progress_increment;
    }
  }

completed:
  printf("\r\nFound %u bytes\r\n", installed_memory);
}

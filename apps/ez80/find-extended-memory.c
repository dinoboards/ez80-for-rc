#include "ez80.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static const uint8_t        test_values[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                               0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

bool test_at(volatile uint8_t *ptr) {
  const uint8_t *values = test_values;
  for (int i = 0; i < 16; i++)
    *ptr++ = *values++;

  ptr--;
  values--;

  for (int i = 0; i < 16; i++)
    if (*ptr-- != *values--)
      return false;

  return true;
}

void find_extended_memory(void) {
  volatile uint8_t    *ptr;
  uint8_t *const       start              = (uint8_t *)0x200000;
  const uint8_t *const end                = (uint8_t *)0xFFFFFF;
  uint24_t             installed_memory   = 0;
  uint24_t             progress_increment = 0x10000; // 64K increment
  uint24_t             next_progress;
  uint8_t             *current_progress;

  next_progress    = progress_increment;
  current_progress = start;
  printf("Testing memory installed from 0x200000\r\n");
  for (ptr = start; ptr <= end; ptr += 16) {
    if(!test_at(ptr))
      goto completed;

    installed_memory += 16;

    if (installed_memory >= next_progress) {
      printf("\rFound %ukb bytes at %p", installed_memory / 1024, current_progress);
      current_progress += progress_increment;
      next_progress += progress_increment;
    }
  }

completed:
  printf("\rFound %ukb from %p to %p\r\n", installed_memory/1024, start, ptr-1);
}

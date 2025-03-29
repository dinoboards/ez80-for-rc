#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const uint8_t test_values[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

/*
Installed memory will be banks of 512K addressed from 0x200000 and up
Scan and report each continuous block discovered
*/

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

uint8_t installed_bank[8];

uint8_t *largest_continuous = NULL;
uint8_t *fastest_continuous = NULL;

void find_install_ram() {

  // Test for upto 4 slow banks
  installed_bank[0] = test_at((uint8_t *)0x200000);
  installed_bank[1] = test_at((uint8_t *)0x280000);
  installed_bank[2] = test_at((uint8_t *)0x300000);
  installed_bank[3] = test_at((uint8_t *)0x380000);

  // test for any and all fast banks
  installed_bank[4] = test_at((uint8_t *)0x400000);
  installed_bank[5] = test_at((uint8_t *)0x480000);
  installed_bank[6] = test_at((uint8_t *)0x500000);
  installed_bank[7] = test_at((uint8_t *)0x580000);

  // Find largest continuous block in slow banks
  uint8_t  count     = 0;
  uint8_t  max_count = 0;
  uint8_t *max_start = NULL;

  // Find largest continuous block in slow banks
  for (int i = 0; i < 8; i++) {
    if (installed_bank[i]) {
      if (count == 0) {
        max_start = (uint8_t *)(0x200000 + (i * 0x80000));
      }
      count++;
      if (count > max_count) {
        max_count          = count;
        largest_continuous = max_start;
      }
    } else {
      count = 0;
    }
  }

  fastest_continuous = largest_continuous;
  count              = 0;
  max_count          = 0;
  max_start          = 0;

  for (int i = 4; i < 8; i++) {
    if (installed_bank[i]) {
      if (count == 0) {
        max_start = (uint8_t *)(0x200000 + (i * 0x80000));
      }
      count++;
      if (count > max_count) {
        max_count          = count;
        fastest_continuous = max_start;
      }
    } else {
      count = 0;
    }
  }
}

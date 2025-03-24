#include "ez80.h"
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

const char progress_chars[] = "|/-\\";

bool test_512k_block(volatile uint8_t *start) {
  volatile uint8_t *ptr;
  const uint24_t    block_size     = 0x80000;
  int               progress_index = 0;

  printf(" ");

  for (ptr = start; ptr < start + block_size; ptr += 16) {
    if (!test_at(ptr)) {
      printf("\b");
      return false;
    }

    if (((uint24_t)ptr) % (16 * 1024) == 0) {
      printf("\b%c", progress_chars[progress_index]);
      progress_index = (progress_index + 1) % 4;
    }
  }

  printf("\b");
  return true;
}

struct {
  uint8_t *start;
  uint8_t *end;
} continuous_blocks[64];

void find_extended_memory(void) {
  uint8_t             *ptr;
  uint8_t *const       start                  = (uint8_t *)0x200000;
  const uint8_t *const end                    = (uint8_t *)0x5F0000;
  uint24_t             block_size             = 0x80000; // 512K block size
  uint8_t             *block_start            = NULL;
  bool                 in_block               = false;
  int                  continuous_block_count = 0;

  printf("Searching for memory installed from 0x200000 to 0x5F0000\r\n");

  const char *ram_speed = "~100ns";

  for (ptr = start; ptr < end; ptr += block_size) {
    if (ptr == (uint8_t *)0x400000)
      ram_speed = "~20ns";

    printf("512K bank at %p: ", ptr);

    if (test_512k_block(ptr)) {
      if (!in_block) {
        block_start = ptr;
        in_block    = true;
      }
      printf("OK     (%s)\r\n", ram_speed);
    } else {
      printf("None\r\n");
      if (in_block) {
        continuous_blocks[continuous_block_count].start = block_start;
        continuous_blocks[continuous_block_count].end   = ptr;
        continuous_block_count++;
        in_block = false;
      }
    }
  }

  if (in_block) {
    continuous_blocks[continuous_block_count].start = block_start;
    continuous_blocks[continuous_block_count].end   = ptr;
    continuous_block_count++;
  }

  printf("\r\n");

  for (int i = 0; i < continuous_block_count; i++) {
    uint24_t length = continuous_blocks[i].end - continuous_blocks[i].start;
    printf("%ukb available at %p\r\n", length / 1024, continuous_blocks[i].start);
  }
}

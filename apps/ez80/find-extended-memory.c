#include "ez80.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t test_values[44] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x3F,
    0xB2, 0x97, 0x4E, 0xD1, 0x8C, 0x5A, 0xF3, 0x26, 0xE8, 0x7D, 0x91, 0x4C, 0xA5, 0x0B, 0x6F, 0x55, 0xAA, // Alternating bits
    0xFF, 0x00,                                                                                           // All ones/zeros
    0x01, 0x02, 0x04, 0x08,                                                                               // Walking ones
    0xFE, 0xFD, 0xFB, 0xF7                                                                                // Walking zeros
};

/*
  Installed memory will be banks of 512K addressed from 0x200000 and up
  Scan and report each continuous block discovered

  Assume config of:
    CS0 is enabled for memory @ $200000 -> $3FFFFF
    CS1 is enabled for memory @ $400000 -> $5FFFFF
*/

bool test_at(volatile uint8_t *ptr);

const char progress_chars[] = "|/-\\";

bool test_512k_block(volatile uint8_t *start) {
  volatile uint8_t *ptr;
  const uint24_t    block_size     = 0x80000;
  int               progress_index = 0;

  printf(" ");

  uint16_t progress_period = 32000 / sizeof(test_values);

  for (ptr = start; ptr < start + block_size - 44; ptr += sizeof(test_values)) {
    if (!test_at(ptr)) {
      printf("\b");
      return false;
    }

    if (progress_period-- == 0) {
      printf("\b%c", progress_chars[progress_index]);
      progress_index  = (progress_index + 1) % 4;
      progress_period = 32000 / sizeof(test_values);
    }
  }

  printf("\b");
  return true;
}

struct {
  uint8_t *start;
  uint8_t *end;
} continuous_blocks[64];

uint8_t *cs0_block;
uint8_t *cs1_block;

void find_extended_memory(void) {
  uint8_t             *ptr;
  uint8_t *const       start                  = (uint8_t *)0x200000;
  const uint8_t *const end                    = (uint8_t *)0x5F0000;
  uint24_t             block_size             = 0x80000; // 512K block size
  uint8_t             *block_start            = NULL;
  bool                 in_block               = false;
  int                  continuous_block_count = 0;

  cs0_block = NULL;
  cs1_block = NULL;
  printf("Searching for memory installed from 0x200000 to 0x5F0000\r\n");

  for (ptr = start; ptr < end; ptr += block_size) {
    printf("512K bank at %p: ", ptr);

    if (test_512k_block(ptr)) {
      if (!in_block) {
        block_start = ptr;
        in_block    = true;
      }
      printf("OK\r\n");

      if (cs0_block == NULL && (uint24_t)ptr < 0x400000)
        cs0_block = ptr;
      else if (cs1_block == NULL && (uint24_t)ptr >= 0x400000)
        cs1_block = ptr;

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

  if (cs0_block || cs1_block)
    printf("\r\n");

  if (cs0_block)
    report_extended_performance(cs0_block);

  if (cs1_block)
    report_extended_performance(cs1_block);
}

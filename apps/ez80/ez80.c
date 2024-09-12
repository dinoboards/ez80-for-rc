#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include "ez80-firmware.h"
#include <stdint.h>
#include <stdio.h>

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  static uint32_t cpu_frequency;

  cpu_frequency = ez80_cpu_freq_get();

  printf("CPU frequency: %lu Hz\r\n", cpu_frequency);

  return 0;
}

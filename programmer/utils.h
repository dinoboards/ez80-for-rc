#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>

void     to_lowercase(char *str);
uint32_t parse_frequency(const char *str);
uint8_t  calculate_flash_div(const uint32_t cpu_frequency);

#endif

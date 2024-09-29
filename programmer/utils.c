#include "utils.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void to_lowercase(char *str) {
  for (; *str; ++str) {
    *str = tolower((unsigned char)*str);
  }
}

// Function to convert a string to int32_t, handling "MHz" suffix and float values
uint32_t parse_frequency(const char *str) {
  size_t len        = strlen(str);
  float  multiplier = 1.0;

  // Check if the string ends with "MHz" (case insensitive)
  if (len > 3 && tolower(str[len - 3]) == 'm' && tolower(str[len - 2]) == 'h' && tolower(str[len - 1]) == 'z') {
    multiplier = 1000000.0;
    len -= 3; // Remove "MHz" from the length
  }

  // Create a copy of the string without the "MHz" part
  char number_part[len + 1];
  strncpy(number_part, str, len);
  number_part[len] = '\0';

  // Convert the number part to float
  char *endptr;
  errno       = 0; // Reset errno before conversion
  float value = strtof(number_part, &endptr);

  // Check for conversion errors
  if (errno != 0 || endptr == number_part || *endptr != '\0') {
    return 0; // Invalid number
  }

  // Return the value multiplied by the appropriate factor, cast to int32_t
  return (uint32_t)(value * multiplier);
}

uint8_t calculate_flash_div(const uint32_t cpu_frequency) { return (uint8_t)((((cpu_frequency * 52L) / 100000L) + 50L) / 100L); }

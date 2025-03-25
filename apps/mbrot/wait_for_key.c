#include "wait_for_key.h"
#include <cpm.h>
#include <stdlib.h>

void wait_for_key() {
  uint8_t k = 0;
  while (k == 0) {
    k = cpm_c_rawio();
    if (k == 27)
      exit(0);
  }
}

void test_for_escape() {
  uint8_t k = 0;
  k         = cpm_c_rawio();
  if (k == 27)
    exit(0);
}

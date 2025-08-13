#include "../includes/stdint.h"

extern void print_string(char *str);

int32_t vmMain(int32_t command, int32_t arg0, int32_t arg1, int32_t arg2) {
  print_string("vmMain\r\n");

  switch (command) {
  case 0: {
    vdu((uint8_t)arg0);
    return 0;
  }

  case 255: {
    vdu_init();
    return 0;
  }

  default: {
    // unknown command
    return -1;
  }
  }
}

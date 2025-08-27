#include <host-functions.h>
#include <stdint.h>

extern void vdu(int24_t);
extern void vdu_init();

int32_t vmMain(int24_t command, int24_t arg0, int24_t arg1, int24_t arg2) {
  switch (command) {
  case 0: {
    vdu(arg0);
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

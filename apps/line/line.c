#include "../common/config_request.h"
#include "../common/v9958.h"
#include <cpm.h>
#include <math.h>
#include <stdio.h>
//
extern uint8_t get_random_seed(void);

RGB palette[16] = {
    {0, 0, 0}, {1, 0, 0}, {4, 0, 0}, {4, 1, 1}, {15, 0, 0}, {0, 1, 0},   {0, 4, 0},   {1, 4, 1},
    {1, 8, 1}, {0, 0, 1}, {0, 0, 4}, {1, 1, 4}, {1, 1, 8},  {10, 0, 10}, {0, 15, 15}, {15, 15, 15},
};

int main(void) {
  const uint8_t mode  = getVideoMode();
  const uint8_t lines = getLineCount();

  srand(get_random_seed());
  setMode6(lines, mode);
  setPalette(palette);
  clearScreenBank0(4);
  // uint8_t c = rand() & 15;
  unsigned int i = 0;

  printf("Press any key to abort\r\n");

  // for (unsigned int i = 0; i < 4000; i++) {
  while (true) {
    if (cpm_rawio() != 0)
      return 0;
    drawLine(rand() % 512, i % lines, rand() % 512, i % lines, rand() & 15, CMD_LOGIC_IMP);
    i++;
  }

  // return 0;
}

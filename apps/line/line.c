#include "../common/config_request.h"
#include <cpm.h>
#include <math.h>
#include <stdio.h>
#include <v99x8.h>
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
  vdp_set_mode(6, lines, mode);
  vdp_set_palette(palette);
  vdp_erase_bank0(4);
  unsigned int i = 0;

  printf("Press any key to abort\r\n");

  // for (unsigned int i = 0; i < 4000; i++) {
  while (true) {
    if (cpm_rawio() != 0)
      return 0;
    vdp_draw_line(rand() % 512, i % lines, rand() % 512, i % lines, rand() & 15, CMD_LOGIC_IMP);
    i++;

    if (i >= 1024) {
      i = 0;
      vdp_clear_all_memory();
    }
  }

  // return 0;
}

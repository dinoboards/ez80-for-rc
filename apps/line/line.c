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

  const uint8_t refresh_rate = getVideoMode();
  const uint8_t lines        = getLineCount();

  srand(get_random_seed());
  vdp_set_lines(lines);
  vdp_set_refresh(refresh_rate);
  vdp_set_graphic_6();
  vdp_set_palette(palette);
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 4, 0, 0);

  unsigned int i = 0;

  printf("Press any key to abort\r\n");

  // for (unsigned int i = 0; i < 4000; i++) {
  while (true) {
    if (cpm_c_rawio() != 0)
      return 0;
    vdp_draw_line(rand() % 512, i % lines, rand() % 512, i % lines, rand() & 15, CMD_LOGIC_IMP);
    i++;

    if (i >= 1024) {
      i = 0;
      vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, 0, 0);
    }
  }

  // return 0;
}

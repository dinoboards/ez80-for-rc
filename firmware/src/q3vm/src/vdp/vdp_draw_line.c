// #include <ez80.h>
#include <stdbool.h>
#include <stdlib.h>
#include <v99x8.h>

void vdp_draw_line(uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, uint8_t colour, uint8_t operation) {

  const uint16_t number_on_x_side = abs(to_x - from_x);
  const uint16_t number_on_y_side = abs(to_y - from_y);

  const bool major_direction_is_y = number_on_y_side >= number_on_x_side;

  const uint16_t long_length  = major_direction_is_y ? number_on_y_side : number_on_x_side;
  const uint16_t short_length = major_direction_is_y ? number_on_x_side : number_on_y_side;

  const bool dix = to_x < from_x;
  const bool diy = to_y < from_y;

  const uint8_t direction = (major_direction_is_y ? 1 : 0) + (dix ? 4 : 0) + (diy ? 8 : 0);

  vdp_cmd_wait_completion();
  vdp_cmd_line(from_x, from_y, long_length, short_length, direction, colour, operation);
}

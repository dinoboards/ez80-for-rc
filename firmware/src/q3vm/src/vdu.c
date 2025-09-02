#include "vdu.h"
#include "stddef.h"
#include <host-functions.h>

// Y co-ord to store font data
// need enough room to allow 2 pages for all modes
#define FONT_Y_OFFSET (current_display_mode >= 16 ? 256 : (576 * 2))

void graphic_print_char(uint8_t ch);

// Must be first function in file for vm
int24_t vdu(uint24_t ch) {
  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn  = current_fn;
      current_fn          = NULL;
      vdu_index           = 0;
      vdu_required_length = 0;

      fn(); // may error and not return
    }
    return -1;
  }

  if (ch == '\n') {
    vdu_lf();
    return ch;
  }

  if (ch == 12) { // cls
    vdu_cls();
    return -1;
  }

  if (ch == '\r') {
    vdu_cr();
    return ch;
  }

  if (ch == 17) { // vdu_colour
    current_fn          = vdu_colour;
    vdu_required_length = 1;
    return -1;
  }

  if (ch == 18) { // gcol mode, colour
    current_fn          = vdu_gcol;
    vdu_required_length = 2;
    return -1;
  }
  if (ch == 22) { // MODE
    current_fn          = vdu_mode;
    vdu_required_length = 1;
    return -1;
  }

  if (ch == 25) { // plot
    current_fn          = vdu_plot;
    vdu_required_length = 5;
    return -1;
  }

  graphic_print_char(ch);
  // print to graphic screen at current text post

  // for the time, lets dual output to serial and graphic
  if (ch <= 127)
    return ch;

  return -1;
}

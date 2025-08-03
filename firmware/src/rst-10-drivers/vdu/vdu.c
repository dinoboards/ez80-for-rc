#include "vdu.h"
#include "crt.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8-super.h>

void vdu_not_implemented() {
  // TODO: set a flag to indicate error
}

extern void vdu_mode(void);
extern const RGB palette_256[256];

void vdu_init() {
  vdp_init();

  vdp_set_extended_palette(palette_256);
  data[0] = 0;
  vdu_mode();

  crt_font_8x8_clear_patterns();
  crt_font_8x8_load_patterns(sysfont, ' ', ' ' + sizeof(sysfont) / 8);
}


static void graphic_print_char(uint8_t ch) {
  point_t gpos;

  if ((ch < ' '))
    return;

  crt_font_8x8_print(ch, current_tpos.x * 8, current_tpos.y * 8);

  current_tpos.x++;
  if (current_tpos.x > tviewport.right) {
    vdu_cr();
    vdu_lf();
  }
}

void mos_oswrite(const uint8_t ch) {

  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn  = current_fn;
      current_fn          = NULL;
      vdu_index           = 0;
      vdu_required_length = 0;

      fn(); // may error and not return
    }
    return;
  }

  if (ch == 8) {
    vdu_bs();
    return;
  }

  if (ch == '\n') {
    vdu_lf();
    return;
  }

  if (ch == 12) { // cls
    if (current_display_mode != 255)
      vdu_cls();

    return;
  }

  if (ch == '\r') {
    vdu_cr();
    return;
  }

  if (ch == 16) { // clg
    vdu_clg();
    return;
  }

  if (ch == 17) { // vdu_colour
    current_fn          = vdu_colour;
    vdu_required_length = 1;
    return;
  }

  if (ch == 18) { // gcol mode, colour
    current_fn          = vdu_gcol;
    vdu_required_length = 2;
    return;
  }

  if (ch == 19) { // colour
    current_fn          = vdu_colour_define;
    vdu_required_length = 5;
    return;
  }

  if (ch == 22) { // MODE
    current_fn          = vdu_mode;
    vdu_required_length = 1;
    return;
  }

  if (ch == 23) { // multi purpose
    current_fn          = vdu_multi_purpose;
    vdu_required_length = 9;
    return;
  }

  if (ch == 24) { // set g viewport
    current_fn          = vdu_set_gviewport;
    vdu_required_length = 8;
    return;
  }

  if (ch == 25) { // plot
    current_fn          = vdu_plot;
    vdu_required_length = 5;
    return;
  }

  if (ch == 28) { // set text viewport
    current_fn          = vdu_set_tviewport;
    vdu_required_length = 4;
    return;
  }

  if (ch == 29) { // set origin
    current_fn          = vdu_set_origin;
    vdu_required_length = 4;
    return;
  }

  if (ch == 31) {
    current_fn          = vdu_tab;
    vdu_required_length = 2;
    return;
  }

  if (current_display_mode == 255)
    return;

  graphic_print_char(ch);
}

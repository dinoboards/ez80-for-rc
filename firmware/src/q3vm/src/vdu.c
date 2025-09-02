#include "vdu.h"
#include "stddef.h"
#include <host-functions.h>

void graphic_print_char(uint24_t ch);

void vdu(uint24_t ch) {
  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn  = current_fn;
      current_fn          = NULL;
      vdu_index           = 0;
      vdu_required_length = 0;

      fn();
    }
    return;
  }

  if (ch >= ' ') {
    graphic_print_char(ch);
    return;
  }

  switch (ch) {
  case '\n': {
    vdu_lf();
    return;
  }

  case 12: {
    vdu_cls();
    return;
  }

  case '\r': {
    vdu_cr();
    return;
  }

  case 17: {
    current_fn          = vdu_colour;
    vdu_required_length = 1;
    return;
  }

  case 18: {
    current_fn          = vdu_gcol;
    vdu_required_length = 2;
    return;
  }

  case 22: {
    current_fn          = vdu_mode;
    vdu_required_length = 1;
    return;
  }

  case 25: {
    current_fn          = vdu_plot;
    vdu_required_length = 5;
    return;
  }
  }
}

#include "vdu.h"
#include "../../q3vm/vm-functions.h"
#include "stddef.h"

void graphic_print_char(uint24_t ch);

void vdu(uint8_t ch) {
  uint8_t cursor_enabled = cursor_state.enabled;
  if (cursor_enabled)
    vdu_cursor_disable();

  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn  = current_fn;
      current_fn          = NULL;
      vdu_index           = 0;
      vdu_required_length = 0;

      fn();
    }
    goto done;
  }

  if (ch >= ' ') {
    graphic_print_char(ch);
    goto done;
  }

  switch (ch) {
  case 8: {
    vdu_bs();
    goto done;
  }

  case '\n': {
    vdu_lf();
    goto done;
  }

  case 12: {
    vm_vdu_cls();
    goto done;
  }

  case '\r': {
    vdu_cr();
    goto done;
  }

  case 16: {
    vm_vdu_clg();
    goto done;
  }

  case 17: {
    current_fn          = vm_vdu_colour;
    vdu_required_length = 1;
    goto done;
  }

  case 18: {
    current_fn          = vm_vdu_gcol;
    vdu_required_length = 2;
    goto done;
  }

  case 19: {
    current_fn          = vm_vdu_colour_define;
    vdu_required_length = 5;
    goto done;
  }

  case 22: {
    current_fn          = vm_vdu_mode;
    vdu_required_length = 1;
    goto done;
  }

  case 24: {
    current_fn          = vm_vdu_set_gviewport;
    vdu_required_length = 8;
    goto done;
  }

  case 25: {
    current_fn          = vdu_plot;
    vdu_required_length = 5;
    goto done;
  }
  }

done:
  if (cursor_enabled)
    vdu_cursor_enable();
}

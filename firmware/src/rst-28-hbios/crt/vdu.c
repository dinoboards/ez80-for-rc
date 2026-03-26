#include "vdu.h"
#include "../rst-28-vars.h"
#include "stddef.h"

extern void graphic_print_char(uint24_t ch);

void vdu(uint8_t ch) {
  vdu_vars_t *const vdu            = &hbios_vars->vdu;
  uint8_t           cursor_enabled = vdu->cursor_state.enabled;
  if (cursor_enabled)
    vdu_cursor_disable();

  if (vdu->vdu_required_length) {
    vdu->data[vdu->vdu_index++] = ch;
    if (vdu->vdu_index == vdu->vdu_required_length) {
      mos_vdu_handler fn       = vdu->current_fn;
      vdu->current_fn          = NULL;
      vdu->vdu_index           = 0;
      vdu->vdu_required_length = 0;

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
    vdu_cls();
    goto done;
  }

  case '\r': {
    vdu_cr();
    goto done;
  }

  case 16: {
    vdu_clg();
    goto done;
  }

  case 17: {
    vdu->current_fn          = vdu_colour;
    vdu->vdu_required_length = 1;
    goto done;
  }

  case 18: {
    vdu->current_fn          = vdu_gcol;
    vdu->vdu_required_length = 2;
    goto done;
  }

  case 19: {
    vdu->current_fn          = vdu_colour_define;
    vdu->vdu_required_length = 5;
    goto done;
  }

  case 22: {
    vdu->current_fn          = vdu_mode;
    vdu->vdu_required_length = 1;
    goto done;
  }

  case 23: { // multi purpose
    vdu->current_fn          = vdu_multi_purpose;
    vdu->vdu_required_length = 9;
    goto done;
  }

  case 24: {
    vdu->current_fn          = vdu_set_gviewport;
    vdu->vdu_required_length = 8;
    goto done;
  }

  case 25: {
    vdu->current_fn          = vdu_plot;
    vdu->vdu_required_length = 5;
    goto done;
  }

  case 28: {
    vdu->current_fn          = vdu_set_tviewport;
    vdu->vdu_required_length = 4;
    goto done;
  }

  case 29: {
    vdu->current_fn          = vdu_set_origin;
    vdu->vdu_required_length = 4;
    goto done;
  }

  case 31: {
    vdu->current_fn          = vdu_tab;
    vdu->vdu_required_length = 2;
    goto done;
  }
  }

done:
  if (cursor_enabled)
    vdu_cursor_enable();
}

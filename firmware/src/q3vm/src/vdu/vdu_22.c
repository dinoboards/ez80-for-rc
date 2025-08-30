#include "../vdu.h"
#include <v99x8-super.h>

#include <stdio.h>

// VDU: 22 (1 byte)

// MODE 0: 640x256 graphics, 80x32 characters, 2 colours, 20kB RAM
// MODE 1: 320x256 graphics, 40x32 characters, 4 colours, 20kB RAM
// MODE 2: 160x256 graphics, 20x32 characters, 16 colours, 20kB RAM
// MODE 3: no graphics, 80x25 characters, 2 colours, 16kB RAM
// MODE 4: 320x256 graphics, 40x32 characters, 2 colours, 10kB RAM
// MODE 5: 160x256 graphics, 20x32 characters, 4 colours, 10kB RAM
// MODE 6: no graphics, 40x25 characters, 2 colours, 8kB RAM
// MODE 7: teletext, 40x25 characters, 8 colours, 1kB RAM

//  V9958 MODE
//  MULTI-COLOR   64x48 16 colours
//  G1            32*8x24*8 16 colours only 256 patterns
//  G3            32*8x24*8 16 colours 768 patterns
//  G4 (212)      256x212 16 colours
//  G4 (192)      256x192 16 colours
//  G5 (212)      512x212 4 colours
//  G5 (192)      512x192 4 colours
//  G6 (212)      512x212 16 colours
//  G6 (192)      512x192 16 colours
//  G7 (212)      256x212 256 colours
//  G7 (192)      256x192 256 colours

// SUPER 1        320 x 200 256 colours @ 60Hz
// SUPER 2        320 x 240 256 colours @ 50Hz
// SUPER 3        360 x 240 256 colours @ 60Hz
// SUPER 4        360 x 288 256 colours @ 50Hz
// SUPER 5        640 x 400 256 colours @ 60Hz
// SUPER 6        640 x 480 256 colours @ 50Hz
// SUPER 7        720 x 480 256 colours @ 60Hz
// SUPER 8        720 x 576 256 colours @ 50Hz
// SUPER 9        640 x 240 256 colours @ 50Hz
// SUPER 10       640 x 288 256 colours @ 50Hz
// SUPER 11       640 x 240 256 colours @ 50Hz
// SUPER 12       640 x 256 256 colours @ 50Hz

//    xx      | xx - serial I/O  MODE 255 (-1)

// BBC MODE   |    MODE    | MAP
//   0        |   SUPER 26 | 640x256(2)  => 640x256 (60Hz)
//   1        |   SUPER 2  | 320x256(4)  => 320x240 (50Hz)
//   2        |   SUPER 2  | 160x256(16) => 320x240 (50Hz)
//   3        |   SUPER 21 | 80x25(2) ch => (640x400) 60hz
//   4        |   SUPER 2  | 320x256(2)  => 320x240 (50Hz)
//   5        |   SUPER 2  | 160x256(4)  => 320x240 (50Hz)
//   6        |   SUPER 1  | 40x25(2) ch => 320x200 (50Hz)
//   7        |   SUPER 1  | 40x25(8) ch mode 0=> 320x200 (50Hz)
//   16       |     4      | 256x192(16) (50Hz)

// VDU 22 This VDU code is used to change MODE. It is followed by one number
// which is the new mode. Thus VDU 22,7 is exactly equivalent to MODE 7.

void vdu_mode(void) {
  current_display_mode = data[0];
  tviewport.left       = 0;
  last_text_row        = 26;
  tviewport.bottom     = 26;
  tviewport.top        = 0;

  switch (data[0]) {
  case 0:
    vdp_set_palette(default_2_colour_palette);
    current_tfg_colour       = 1;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 1;
    last_text_column         = 79;
    tviewport.right          = 79;
    last_text_row            = 31;
    tviewport.bottom         = 31;
    vdp_set_super_graphic_26();
    break;

  case 1:
    vdp_set_palette(default_4_colour_palette);
    current_tfg_colour       = 3;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 3;
    last_text_column         = 39;
    tviewport.right          = 39;
    last_text_row            = 29;
    tviewport.bottom         = 29;
    vdp_set_super_graphic_2();
    break;

  case 2:
    vdp_set_palette(default_16_colour_palette);
    current_tfg_colour       = 7;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 15;
    last_text_column         = 39;
    tviewport.right          = 39;
    last_text_row            = 29;
    tviewport.bottom         = 29;
    vdp_set_super_graphic_2();
    break;

  case 3:
    vdp_set_palette(default_2_colour_palette);
    current_tfg_colour       = 1;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 1;
    last_text_column         = 79;
    tviewport.right          = 79;
    last_text_row            = 49;
    tviewport.bottom         = 49;
    vdp_set_super_graphic_21();
    break;

  case 4:
    vdp_set_palette(default_2_colour_palette);
    current_tfg_colour       = 1;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 1;
    last_text_column         = 39;
    tviewport.right          = 39;
    last_text_row            = 29;
    tviewport.bottom         = 29;
    vdp_set_super_graphic_2();
    break;

  case 5:
    vdp_set_palette(default_4_colour_palette);
    current_tfg_colour       = 3;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 3;
    last_text_column         = 39;
    tviewport.right          = 39;
    last_text_row            = 29;
    tviewport.bottom         = 29;
    vdp_set_super_graphic_2();
    break;

  case 16:
    vdp_set_palette(default_4_colour_palette);
    current_tfg_colour       = 3;
    current_tbg_colour       = 0;
    current_mode_colour_mask = 3;
    last_text_column         = 31;
    tviewport.right          = 31;
    last_text_row            = 23;
    tviewport.bottom         = 23;
    vdp_set_lines(192);
    vdp_set_refresh(50);
    vdp_set_graphic_4();
    break;

  case 255:
    break;

  default:
    vdu_not_implemented();
  }

  gsviewport.left   = convert_x(gviewport.left);
  gsviewport.top    = convert_y(gviewport.top);
  gsviewport.right  = convert_x(gviewport.right);
  gsviewport.bottom = convert_y(gviewport.bottom);

  vdu_cls();
  init_font_patterns();
}

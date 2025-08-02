#include <stdint.h>
#include <stdio.h>
#include <v99x8-super.h>

#ifdef VDP_SUPER_HDMI

extern char log_buffer[257];

extern const uint8_t sysfont[(128 - ' ') * 8]; // 96*8

#define VRAM_SIZE         0x100000
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8L * 256L))

void print_char_at(const uint8_t ch, const uint16_t x, const uint16_t y) {
  screen_addr_t addr = FONT_8X8_STORED_Y + ((long)ch * 8L);

  vdp_cmd_wait_completion();
  vdp_cmd_move_linear_to_xy(addr, x, y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);
}

void load_font_data(void) {
  vdp_set_super_graphic_1(); // can only access higher memory directly, when super mode is on

  vdp_cpu_to_vram(sysfont, FONT_8X8_STORED_Y + ((long)' ' * 8L), sizeof(sysfont));
  printf("Fonts data loaded into VRAM at %lx\n", FONT_8X8_STORED_Y);
}

void print_to_screen(void) {
  vdp_set_remap(4, 230);

  const char *str = log_buffer;
  uint16_t    x   = 0;
  uint16_t    y   = vdp_get_screen_height() - (4 * 8);
  char        ch  = *str++;
  while (ch) {
    while (ch < ' ') {
      ch = *str++;
      if (ch == 0)
        return;
    }

    print_char_at(ch, x, y);

    ch = *str++;
    x += 8;
    if (x >= vdp_get_screen_width()) {
      x = 0;
      y += 8;
    }
  }
}

#endif

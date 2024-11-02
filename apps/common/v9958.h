#ifndef __V9958
#define __V9958

#include <ez80.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PAL  1
#define NTSC 2

typedef struct {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
} RGB;

#define VDP_DATA PORT_IO(0xFF98)
#define VDP_ADDR PORT_IO(0xFF99)
#define VDP_PALT PORT_IO(0xFF9A)
#define VDP_REGS PORT_IO(0xFF9B)

#define vdp_out_cmd(v)      port_out(VDP_ADDR, v)
#define vdp_out_dat(v)      port_out(VDP_DATA, v)
#define vdp_out_pal(v)      port_out(VDP_PALT, v)
#define vdp_out_reg_byte(v) port_out(VDP_REGS, v)

extern void vdp_clear_all_memory(void);
extern void vdp_set_palette(RGB *);
extern void vdp_set_mode(const uint8_t mode, const uint8_t lines, const uint8_t refresh_rate);

extern void vdp_erase_bank0(uint8_t color);
extern void vdp_erase_bank1(uint8_t color);
extern void _vdp_reg_write(uint16_t rd);
extern void vdp_out_reg_int16(uint16_t b);

#define vdp_reg_write(a, b) _vdp_reg_write((a)*256 + (b))

#define REGISTER_COUNT 12

#define CMD_VDP_TO_VRAM 0xC0
#define CMD_LINE(op)    (0x70 | op)
#define CMD_PSET(op)    (0x50 | op)
#define CMD_LOGIC_IMP   0x00
#define CMD_LOGIC_AND   0x01

void vmd_cmd_draw_line(void);
void vdp_cmd_wait_completion(void);

extern uint8_t  vdp_cmdp_dir;
extern uint16_t vdp_cmdp_long_side;
extern uint16_t vdp_cmdp_short_side;
extern uint16_t vdp_cmdp_from_x;
extern uint16_t vdp_cmdp_from_y;
extern uint8_t  vdp_cmdp_color;
extern uint8_t  vdp_cmdp_operation;

#define drawLine(fromX, fromY, toX, toY, color, operation)                                                                         \
  vdp_cmdp_from_x    = (fromX);                                                                                                    \
  vdp_cmdp_from_y    = (fromY);                                                                                                    \
  vdp_cmdp_color     = (color);                                                                                                    \
  vdp_cmdp_operation = CMD_LINE((operation));                                                                                      \
  _drawLine(toX, toY)

extern void _drawLine(uint16_t toX, uint16_t toY);

#define pointSet(x, y, color, operation)                                                                                           \
  vdp_cmdp_from_x    = (x);                                                                                                        \
  vdp_cmdp_from_y    = (y);                                                                                                        \
  vdp_cmdp_color     = (color);                                                                                                    \
  vdp_cmdp_operation = CMD_PSET((operation));                                                                                      \
  vmd_cmd_draw_line()

#endif

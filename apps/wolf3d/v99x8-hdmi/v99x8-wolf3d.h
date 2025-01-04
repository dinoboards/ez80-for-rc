#ifndef __V99X8_HDMI_WOLF3D_H
#define __V99X8_HDMI_WOLF3D_H

#include <v99x8.h>

/**
 * @brief copy data from CPU to VRAM address 0x000000
 *
 *
 * @param source the byte data to be copied
 * @param length the number of bytes to be copied
 * @param palette the palette to be applied
 */
extern void vdp_cpu_to_vram0_with_palette(const uint8_t *const source, uint16_t length, uint8_t palette[256]);

/**
 * @brief VDP command 'High-speed move CPU to VRAM'
 *
 * Command Code: CMD_HMMC 0xF0
 *
 * The HMMC command is used to transfer data from the CPU to the VRAM or the expansion RAM.
 * Since the data to be transferred is done in units of one byte, there is a limitation due to the display mode, on the value for x.
 *
 * @note that in the G4 and G6 modes, the lower one bit, and in the G5 mode, the lower two bits of x and width, are lost.
 *
 * @param source the byte data to be copied to the VDP's VRAM
 * @param x the starting x-coordinate of the rectangle
 * @param y the starting y-coordinate of the rectangle
 * @param width the width of the rectangle in pixels
 * @param height the height of the rectangle in pixels
 * @param direction the direction of the painting (DIX_RIGHT, DIX_LEFT, DIY_DOWN, DIY_UP)
 * @param length the number of bytes to be copied (width * height)
 * @param palette the palette to be applied
 */
extern void vdp_cmd_move_cpu_to_vram_with_palette(const uint8_t *source,
                                                  uint16_t       x,
                                                  uint16_t       y,
                                                  uint16_t       width,
                                                  uint16_t       height,
                                                  uint8_t        direction,
                                                  uint24_t       length,
                                                  uint8_t        palette[256]);

#endif

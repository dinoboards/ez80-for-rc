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
extern void vdp_cpu_to_vram0_with_palette(const uint8_t *const source, uint16_t length, byte palette[256]);

#endif

#include "fbios-get-lsb.h"
#include "fbios-vars.h"
#include <stdio.h>

uint24_t get_lsb() {
  const uint16_t sectors_per_track  = fbios_vars->fb_current_storage_unit->sectors_per_track;
  const uint24_t lsb                = fbios_vars->fb_current_storage_unit->starting_sector;
  const uint24_t track_byte_offset  = fbios_vars->fb_trck * 64 * 128;
  const uint24_t sector_byte_offset = fbios_vars->fb_sec * 128;
  const uint24_t byte_address       = track_byte_offset + sector_byte_offset;
  return lsb + (byte_address / 512);
}

uint24_t get_offset() { return (fbios_vars->fb_sec & 3) * 128; }

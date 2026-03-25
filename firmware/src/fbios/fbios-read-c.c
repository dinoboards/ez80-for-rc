#include "../rst-28-vars.h"
#include "fbios-get-lsb.h"
#include "fbios-vars.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Implement the CBIOS READ Bios Function (function 13)
 *
 * Read the currently set track and sector at the current DMA address.
 *
 * Utilizes a 512-byte sector cache, reading from the cache if
 * available, otherwise request HBIOS fetch the data from the disk.
 *
 * @return uint8_t 0 for OK, 1 for unrecoverable error, 0FFh if media changed.
 */
uint8_t fbios_read() {
  const uint8_t             hbios_unit    = fbios_vars->fb_current_storage_unit->hbios_unit;
  fbios_disk_cache_t *const fb_disk_cache = fbios_vars->fb_disk_cache;
  uint8_t *const            buffer        = fb_disk_cache->buffer;

  {
    const uint24_t lsb = get_lsb();

    if (!(fb_disk_cache->is_valid && fb_disk_cache->hbios_disk_unit == hbios_unit && fb_disk_cache->hbios_sector == lsb)) {

      if (fb_disk_cache->is_valid && fb_disk_cache->is_dirty) {
        dioseek(fb_disk_cache->hbios_disk_unit, fb_disk_cache->hbios_sector);
        diowrite(fb_disk_cache->hbios_disk_unit, 1, buffer);
      }

      dioseek(hbios_unit, lsb);
      dioread(hbios_unit, 1, buffer);

      fb_disk_cache->is_valid        = true;
      fb_disk_cache->hbios_disk_unit = hbios_unit;
      fb_disk_cache->hbios_sector    = lsb;
      fb_disk_cache->is_dirty        = false;
    }

    {
      const uint24_t offset = get_offset();
      memcpy((uint8_t *)fbios_vars->fb_dma, &buffer[offset], 128);

      return 0;
    }
  }
}

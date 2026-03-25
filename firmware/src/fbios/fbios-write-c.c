#include "../rst-28-vars.h"
#include "fbios-get-lsb.h"
#include "fbios-vars.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Write the currently set track and sector.
 *
 * deblocking=0 - Write can be deferred
 * deblocking=1 - Write must be immediate
 * deblocking=2 - Write can be deferred, no pre-read is necessary.
 * @param deblocking deblocking requirement
 * @return uint8_t return 0 for OK, 1 for unrecoverable error, 2 if disc is readonly, 0FFh if media changed.
 */
int8_t fbios_write(uint8_t deblocking) {
  const uint8_t             hbios_unit    = fbios_vars->fb_current_storage_unit->hbios_unit;
  fbios_disk_cache_t *const fb_disk_cache = fbios_vars->fb_disk_cache;
  uint8_t *const            buffer        = fb_disk_cache->buffer;
  const uint24_t            lsb           = get_lsb();

  if (!((fb_disk_cache->is_valid && fb_disk_cache->hbios_disk_unit == hbios_unit && fb_disk_cache->hbios_sector == lsb))) {
    if (fb_disk_cache->is_valid && fb_disk_cache->is_dirty) {
      dioseek(fb_disk_cache->hbios_disk_unit, fb_disk_cache->hbios_sector);
      diowrite(fb_disk_cache->hbios_disk_unit, 1, buffer);
    }

    if (deblocking != 2) {
      dioseek(hbios_unit, lsb);
      dioread(hbios_unit, 1, buffer);
    } else {
      memset(buffer, 0, 512);
    }

    fb_disk_cache->is_valid        = true;
    fb_disk_cache->hbios_disk_unit = hbios_unit;
    fb_disk_cache->hbios_sector    = lsb;
  }

  {
    const uint24_t offset = get_offset();
    memcpy(&buffer[offset], (uint8_t *)fbios_vars->fb_dma, 128);

    if (deblocking == 1) {
      dioseek(hbios_unit, lsb);
      diowrite(hbios_unit, 1, buffer);
      fb_disk_cache->is_dirty = false;
    } else {
      fb_disk_cache->is_dirty = true;
    }

    return 0;
  }
}

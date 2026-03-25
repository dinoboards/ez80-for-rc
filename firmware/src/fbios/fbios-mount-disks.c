#include "../rst-28-vars.h"
#include "fbios-vars.h"
#include <stdint.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  uint8_t  indicator;
  uint8_t  chs_start[3];
  uint8_t  type;
  uint8_t  chs_end[3];
  uint32_t starting_sector;
  uint32_t number_of_sectors;
} mbr_partition_table_t; // 16 bytes

/**
 * @brief iterate all HBIOS disks and 'mount'' CP/M compatible images
 *
 * Maps Ram Disk MID_RF as Ram Disk
 * For MID_HDNEW, searches for CP/M partition (0x2E) and install
 * upto 4 slices
 */
void fbios_mount_disks() {
  uint8_t                fbios_unit = 0;
  uint8_t                hbios_unit = 0;
  uint8_t                i;
  uint8_t                slice;
  uint8_t                slice_count;
  mbr_partition_table_t *p;
  uint8_t                media_type;
  fbios_storage_t       *f = &fbios_vars->fb_storage_units[0];
  uint8_t                buffer[512];

  fbios_vars->fb_dma  = 0x0080;
  fbios_vars->fb_disk = fbios_vars->fb_trck = fbios_vars->fb_sec = 0;

  while (diodevice_getstatus(hbios_unit) == 0) {

    media_type = diomedia(hbios_unit, 0) >> 8;

    if (media_type == MID_RF) {
      f->hbios_unit       = hbios_unit;
      f->hbios_media_type = media_type;
      f->slice            = 0;
      f->starting_sector  = 0;

      fbios_unit++;
      f++;
      hbios_unit++;

      if (fbios_unit >= FBIOS_MAX_STORAGE_UNITS)
        return;

      continue;
    }

    if (media_type == MID_HDNEW) {
      dioseek(hbios_unit, 0);
      dioread(hbios_unit, 1, buffer);
      p = (mbr_partition_table_t *)&buffer[0x1BE];

      for (i = 0; i < 4; i++) {
        if (p->type != 0x2E)
          continue;

        slice_count = p->number_of_sectors / 16484;
        for (slice = 0; slice < MIN(slice_count, 4); slice++) {
          f->hbios_unit       = hbios_unit;
          f->hbios_media_type = media_type;
          f->slice            = slice;
          f->starting_sector  = p->starting_sector;

          p->starting_sector += 16384;
          fbios_unit++;
          f++;

          if (fbios_unit >= FBIOS_MAX_STORAGE_UNITS)
            return;
        }

        p++;
      }
    }
    hbios_unit++;
  }
}

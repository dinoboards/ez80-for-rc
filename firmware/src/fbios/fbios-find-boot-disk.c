#include "../print-string.h"
#include "../rst-28-vars.h"
#include "fbios-vars.h"
#include <ez80-support.h>
#include <stdint.h>

typedef struct {
  uint8_t reserved1[128];
  uint8_t reserved2[128];
  uint8_t reserved3[128];

  uint24_t sig;
  uint16_t loader_type;

  uint8_t reserved4[0x5B];

  uint8_t  write_protect;   /* WRITE PROTECT BOOLEAN */
  uint16_t update_sequence; /* PREFIX UPDATE SEQUENCE NUMBER (DEPRECATED?) */
  uint8_t  ver[4];          /* OS BUILD VERSION - NOT RELATED ACTUAL OS VERSION */
  uint8_t  label[17];       /* DISK LABEL (EXACTLY 16 BYTES) $ TERMINATED */
  uint16_t deprecated;      /**/
  uint16_t system_location; /* ADDRESS TO START LOADING SYSTEM */
  uint16_t system_end;      /* ADDRESS TO STOP LOADING SYSTEM */
  uint16_t system_entry;    /* ADDRESS TO ENTER SYSTEM (OS) */
} boot_disk_meta_sector_t;

#define EZ80_BOOT_SIG   (((uint24_t)'E') + (((uint24_t)'Z') << 8) + (((uint24_t)'8') << 16))
#define EZ80_BOOT_16BIT ((uint16_t)((uint16_t)1 + ((uint16_t)~1 << 8)))

/**
 * @brief iterate all FBIOS disk images and searches
 * for disk image with a 'bootable' operating system
 *
 * If not bootable system found, reports 'NO OS' and halts
 *
 * @return uint8_t the fbios disk unit containing a bootable
 * operating system
 */
uint8_t fbios_find_boot_disk() {
  uint8_t          hbios_unit;
  uint8_t          fbios_unit;
  uint8_t          size;
  uint24_t         starting_sector;
  fbios_storage_t *f = &fbios_vars->fb_storage_units[0];

  boot_disk_meta_sector_t buffer;

  for (fbios_unit = 0; fbios_unit < FBIOS_MAX_STORAGE_UNITS; fbios_unit++, f++) {
    if (f->hbios_media_type == 0)
      break;

    hbios_unit      = f->hbios_unit;
    starting_sector = f->starting_sector;

    dioseek(hbios_unit, starting_sector + 2);
    dioread(hbios_unit, 1, (uint8_t *)&buffer);

    if (buffer.sig != EZ80_BOOT_SIG)
      continue;

    if (buffer.loader_type != EZ80_BOOT_16BIT)
      continue;

    size = (buffer.system_end - buffer.system_location) / 512;

    fbios_vars->fb_boot_disk        = fbios_unit;
    fbios_vars->fb_system_location  = extend_mbase(buffer.system_location);
    fbios_vars->fb_system_end       = extend_mbase(buffer.system_end);
    fbios_vars->fb_system_entry     = extend_mbase(buffer.system_entry);
    fbios_vars->fb_system_secsize   = (buffer.system_end - buffer.system_location) / 512;
    fbios_vars->fb_boot_load_sector = f->starting_sector + 3;

    return fbios_unit;
  }

  print_string("\r\nNO OS FOUND.  HALTING\r\n");
  while (1)
    ;
}

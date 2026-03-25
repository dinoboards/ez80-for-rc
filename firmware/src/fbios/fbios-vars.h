#ifndef __FBIOS_VARS
#define __FBIOS_VARS

#define FBIOS_MAX_STORAGE_UNITS (0x50 - 0x41 + 1) /* P-A+1 */

#include <stdint.h>
typedef struct {
  uint8_t  hbios_unit;        /* 1 BYTE: the hbios device index */
  uint8_t  slice;             /* 1 BYTE: the specific slice for this disk mapping*/
  uint8_t  hbios_media_type;  /* 1 BYTE: the type of media detected by hbios */
  uint16_t sectors_per_track; /* 2 BYTES: the number of sectors per track */
  uint16_t disk_reference;    /* 2 BYTES: 16 bit address within the MBASE segment. */
  uint24_t starting_sector;   /* 3 BYTES: the starting sector number for the specific slice */
  uint8_t  padding[6];        /* 6 BYTES  padding to make structure 16 bytes in length */
} fbios_storage_t;

#define SLICE_SIZE        (8 * 1024 * 1024)
#define SECTORS_PER_SLICE (SLICE_SIZE / 512)

typedef struct fbios_disk_cache_s {
  uint8_t  buffer[512];
  uint8_t  is_valid;
  uint8_t  is_dirty;
  uint8_t  hbios_disk_unit;
  uint24_t hbios_sector;

} fbios_disk_cache_t;

typedef struct fbios_var_s {
  uint24_t            fb_dma;
  uint24_t            fb_disk;
  uint24_t            fb_trck;
  uint24_t            fb_sec;
  uint24_t            fb_osloadguard; /* SET TRUE AFTER FIRMWARE HAS LOADED */
  uint24_t            fb_boot_disk;
  uint24_t            fb_system_location; /* ADDRESS TO START LOADING SYSTEM */
  uint24_t            fb_system_end;      /* ADDRESS TO STOP LOADING SYSTEM */
  uint24_t            fb_system_entry;    /* ADDRESS TO ENTER SYSTEM (OS) */
  uint24_t            fb_system_secsize;
  uint24_t            fb_boot_load_sector;
  fbios_storage_t    *fb_current_storage_unit; /* the current fb_storage_unit */
  fbios_disk_cache_t *fb_disk_cache;
  fbios_storage_t     fb_storage_units[FBIOS_MAX_STORAGE_UNITS];
} fbios_vars_t;

extern fbios_vars_t *fbios_vars;

#endif

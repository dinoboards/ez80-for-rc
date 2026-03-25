#include "../fbios/fbios-vars.h"
#include "../print-string.h"
#include "../rst-28-vars.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern bool linear_mem_test_at(uint24_t address);

/**
 * @brief Probes and identifies the amount of linear RAM installed.
 *
 * Tests specific memory addresses (0x20, 0x28, 0x30, 0x38) to determine
 * capacity in 512K increments.
 *
 * Assumes install memory has no 'gaps'.
 *
 * @return uint8_t 4 = 2048K, 3 = 1536K, 2 = 1024K, 1 = 512K, 0 = None.
 */
uint8_t probe_installed_linear_ram() {
  bool installed_at_0x20 = linear_mem_test_at(0x200001);
  bool installed_at_0x28 = linear_mem_test_at(0x280001);
  bool installed_at_0x30 = linear_mem_test_at(0x300001);
  bool installed_at_0x38 = linear_mem_test_at(0x380001);

  bool has_2048K = installed_at_0x20 & installed_at_0x28 & installed_at_0x30 & installed_at_0x38;
  bool has_1536K = installed_at_0x20 & installed_at_0x28 & installed_at_0x30 & !has_2048K;
  bool has_1024K = installed_at_0x20 & installed_at_0x28 & !has_1536K;
  bool has_512K  = installed_at_0x20 & !has_1024K;

  if (has_2048K) {
    return 4;
  }

  if (has_1536K) {
    return 3;
  }

  if (has_1024K) {
    return 2;
  }

  if (has_512K) {
    return 1;
  }

  return 0;
}

void report_installed_linear_ram(const uint8_t size) {

  switch (size) {
  case 0:
    print_string("  Linear RAM: None\r\n");
    return;

  case 1:
    print_string("  Linear RAM: 512K\r\n");
    return;

  case 2:
    print_string("  Linear RAM: 1024K\r\n");
    return;

  case 3:
    print_string("  Linear RAM: 1536K\r\n");
    return;

  case 4:
    print_string("  Linear RAM: 2048K\r\n");
    return;

  default:
    print_string("  Linear RAM: Other\r\n");
  }
}

extern uint16_t romwbw_sig;
extern uint24_t romwbw_name_0;

/*
at 03 0070
ROM_SIG:
  .DB	$76, $B5		; 2 SIGNATURE BYTES
  .DB	1			; STRUCTURE VERSION NUMBER
  .DB	7			; ROM SIZE (IN MULTIPLES OF 4KB, MINUS ONE)
  .DW	NAME			; POINTER TO HUMAN-READABLE ROM NAME
  .DW	AUTH			; POINTER TO AUTHOR INITIALS
  .DW	DESC			; POINTER TO LONGER DESCRIPTION OF ROM
  .DB	0, 0, 0, 0, 0, 0	; RESERVED FOR FUTURE USE; MUST BE ZERO

at 03 0080
NAME	.DB	"ROMWBW v", BIOSVER, ", ", CONFIG, ", ", TIMESTAMP, 0

"ROM" - as bytes = $52 $4F $4D
*/

#define ROM_SIG ((uint24_t)'R' | (((uint24_t)'O') << 8) | ((uint24_t)'M' << 16))

extern uint16_t msxrom_vdp_marker;
extern uint8_t  msxrom_ver_marker;

/*
at $0006
VDP.DR: defb    098H
VDP.DW: defb    098H

at $002D
romid:
; version ID
; MSX version number
;  0 = MSX 1
;  1 = MSX 2
;  2 = MSX 2+
;  3 = MSX turbo R
      db      2
*/

/**
 * @brief Identifies the type of external ROM present.
 *
 * Check for specific signature for external 16 bit ROM
 * for ROMWBW or MSX ROMs.
 *
 * @return uint8_t 1 for ROMWBW, 2 for MSX, 0 for None Or Unknown.
 */
uint8_t probe_extrom() {
  if (romwbw_sig == 0xB576 && romwbw_name_0 == ROM_SIG)
    return 1;

  if (msxrom_vdp_marker == 0x9898 && msxrom_ver_marker <= 2)
    return 2;

  return 0;
}

void report_extrom(const uint8_t id) {
  print_string("External ROM: ");

  switch (id) {
  case 0:
    print_string("None\r\n");
    return;

  case 1:
    print_string("RomWBW\r\n");
    return;

  case 2:
    print_string("MSX\r\n");
    return;

  default:
    print_string("Other\r\n");
    return;
  }
}

// If we have linear ram, these segment references are initialised

extern hbios_vars_t *hbios_vars;
extern fbios_vars_t *fbios_vars;
// extern uint8_t      *fbios_cache;

void allocate_ram_for_hbios_and_fbios() {
  uint8_t       sys_seg;
  const uint8_t size = probe_installed_linear_ram();

  if (size == 0) {
    sys_seg    = 0;
    hbios_vars = 0;
    fbios_vars = 0;
    return;
  }

  sys_seg = 0x20 + (size * 0x08) - 0x01;

  hbios_vars = (hbios_vars_t *)((uint24_t)sys_seg << 16);
  fbios_vars = (fbios_vars_t *)&((uint8_t *)hbios_vars)[0x8000];

  memset(hbios_vars, 0, 0x10000);
  hbios_vars->himem = (uint8_t *)hbios_vars;

  fbios_vars->fb_disk_cache = (fbios_disk_cache_t *)&((uint8_t *)hbios_vars)[0xA000];
}

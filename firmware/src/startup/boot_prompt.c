#include "../print-string.h"
#include <stdint.h>
#include <stdio.h>

extern uint8_t firmware_version_major;
extern uint8_t firmware_version_minor;
extern uint8_t firmware_version_patch;
extern uint8_t firmware_version_revision;

uint8_t util_get_day_of_month(void);
uint8_t util_get_month(void);
uint8_t util_get_year(void);

extern uint32_t cpu_freq_calculated;

uint8_t probe_installed_linear_ram();

void boot_prompt1() {
  print_string("\r\n\n\neZ80 Firmware v");
  print_uint16(firmware_version_major);
  print_string(".");
  print_uint16(firmware_version_minor);
  print_string(".");
  print_uint16(firmware_version_patch);
  print_string(".");
  print_uint16(firmware_version_revision);

  printf(", 20%02d-%02d-%02d\r\n\r\n", util_get_year(), util_get_month(), util_get_day_of_month());

  {
    uint24_t a = cpu_freq_calculated / 1000000L;
    uint24_t b = (uint24_t)(cpu_freq_calculated / 1000L) % 1000;
    printf("Frequency: %d.%03dMHz\r\n", a, b);
  }
  print_string("   Memory: 128KB ROM, ");
  switch (probe_installed_linear_ram()) {
  case 0:
    print_string("None");
    break;

  case 1:
    print_string("512KB");
    break;

  case 2:
    print_string("1024KB");
    break;

  case 3:
    print_string("1536KB");
    break;

  case 4:
    print_string("2048KB");
    break;
  }

  print_string(" LINEAR RAM\r\n");
}

/**
 * @brief External Memory timing (CS3)
 *
 * BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
 *
 */
extern uint8_t mem_bus_timings;

/**
 * @brief Linear Memory timing (CS0)
 *
 * BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
 *
 */
extern uint8_t mem0_bus_mode_and_timing;

extern uint8_t io_bus_mode_and_timing;

static void print_timing(const uint8_t t) {
  print_uint16(t & 7);
  if (t & 0x80)
    print_string(" B/C");
  else
    print_string(" W/S");
}

extern uint8_t ez80_flash_ws_get();

extern uint8_t rtc_enabled;

typedef struct rtc_time_s {
  uint8_t century;
  uint8_t year;
  uint8_t month;
  uint8_t day_of_month;
  uint8_t hour;
  uint8_t minutes;
  uint8_t seconds;
} rtc_time_t;

extern uint8_t rtc_time_get(rtc_time_t *t);

void boot_prompt2() {
  print_string("   Timing: ");
  print_timing(mem_bus_timings);
  print_string(" EXTERNAL RAM, ");
  print_timing(mem0_bus_mode_and_timing);
  print_string(" LINEAR RAM, ");
  print_timing(io_bus_mode_and_timing);
  printf(" IO, %d W/S FLASH\r\n", ez80_flash_ws_get());

  print_string("      RTC: ");
  if (rtc_enabled) {
    rtc_time_t t;
    rtc_time_get(&t);

    printf("Powered %02d%02d-%02d-%02d %02d:%02d:%02d", t.century, t.year, t.month, t.day_of_month, t.hour, t.minutes, t.seconds);
  } else
    print_string("Not Powered");

  print_string("\r\n");
}

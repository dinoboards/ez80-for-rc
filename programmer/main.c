#include "main.h"
#include "blink.pio.h"
#include "command_dispatcher.h"
#include "firmware_version.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "led.h"
#include "pico/stdlib.h"
#include "read_line.h"
#include "tusb.h"
#include "zdi.h"
#include <stdio.h>
#include <string.h>

void report_connection(bool power_detected, bool reset_detected, bool zda_detected, bool zcl_detected, bool connection_detected) {
  char buffer[256]; // Adjust the buffer size as needed

  sprintf(buffer, "ZDI Connection %s  (PWR: %s, RST: %s, ZDA: %s, ZCL: %s)                ",
          connection_detected ? "OK:" : "FAILED:  ", power_detected ? "OK" : "FAIL", reset_detected ? "OK" : "FAIL",
          zda_detected ? "OK" : "FAIL", zcl_detected ? "OK" : "FAIL");

  printf(buffer);
  int length = strlen(buffer);
  for (int i = 0; i < length; i++)
    printf("\b");
}

void report_non_connection(void) { printf("ZDI Connection ......:\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"); }

void report_zdi_id(uint8_t zdi_id_low, uint8_t zdi_id_high, uint8_t zdi_id_rev) {
  char buffer[256]; // Adjust the buffer size as needed

  sprintf(buffer, "eZ80 Detected:  ID %02X%02X Rev %02X", zdi_id_high, zdi_id_low, zdi_id_rev);

  printf(buffer);
  int length = strlen(buffer);
  for (int i = 0; i < length; i++)
    printf("\b");
}

void report_zdi_id_failed(void) { printf("Device Failure. Unable to proceed\r\n"); }

void conduct_test() {
  zdi_configure_pins();

  uint8_t buffer[13] = {0x00, 0x5A, 0x00, 0x5A};
  while (true) {
    zdi_flash_write_bytes(0x000000, buffer, 4);
    sleep_ms(1000);
  }
}

extern void process_status_command();

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq);

#define PIO_CLK_OUT 22

int main() {
  zdi_init_pins();

  stdio_init_all();
  sleep_ms(2000);

  assert(PIO_CLK_OUT < 31);
  assert(PIO_BLINK_LED3_GPIO < 31 || PIO_BLINK_LED3_GPIO >= 32);

  PIO  pio;
  uint sm;
  uint offset;

  printf("\r\n\n\n\n\n");

  // Find a free pio and state machine and add the program
  bool rc = pio_claim_free_sm_and_add_program_for_gpio_range(&blink_program, &pio, &sm, &offset, PIO_CLK_OUT, 1, true);
  hard_assert(rc);

  // Start clock flashing
  blink_pin_forever(pio, sm, offset, PIO_CLK_OUT, 16000000);

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  printf("\r\n");

#ifdef AUTOMATIC_MODE
  printf("Automatic Mode\r\n");
#endif
  printf("Available Firmware Version: %d.%d.%d.%d (%s)\r\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, REVISION_VERSION,
         FIRMWARE_DATE);

wait_for_valid_connection:
  printf("\r\n---------------\r\n");
  zdi_wait_for_connection(report_connection, report_non_connection);
  printf("\r\n");

  zdi_configure_pins();

  if (!zdi_wait_for_valid_identity(report_zdi_id, report_zdi_id_failed)) {
    return -1;
  }

  printf("\r\n");

  zdi_full_reset();
  process_status_command();

  input_buffer[0] = 0;

#ifdef AUTOMATIC_MODE
  strcpy(input_buffer, "flash");
  process_command();
  sleep_ms(500);

  strcpy(input_buffer, "verify");
  process_command();
  sleep_ms(500);

  printf("\r\n");
  while (1) {
    bool reset_detected = gpio_get(ZDI_RESET_PIN);
    if (!reset_detected) {
      printf("\r\n");
      goto wait_for_valid_connection;
    }
    printf("LED ON \r");
    turn_led_on();
    sleep_ms(500);
    printf("LED OFF\r");
    turn_led_off();
    sleep_ms(500);
  }

#endif

  while (true) {
    printf("ZDI> ");
    if (read_line(zdi_connection_lost))
      process_command();
    else
      goto wait_for_valid_connection;
  }
}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
  blink_program_init(pio, sm, offset, pin);
  pio_sm_set_enabled(pio, sm, true);

  printf("eZ80 CPU CLOCK: %d Hz\n", freq);

  // PIO counter program takes 3 more cycles in total than we pass as
  // input (wait for n + 1; mov; jmp)
  pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}

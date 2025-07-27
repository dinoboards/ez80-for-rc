#include "main.h"
#include "command_dispatcher.h"
#include "firmware_version.h"
#include "pico/stdlib.h"
#include "read_line.h"
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

void report_zdi_id_failed(void) {
  const char *waiting_message = "eZ80 Detected:  Waiting for Initialisation...";
  printf(waiting_message);
  for (size_t i = 0; i < strlen(waiting_message); i++)
    printf("\b");
}

void conduct_test() {
  zdi_configure_pins();

  uint8_t buffer[13] = {0x00, 0x5A, 0x00, 0x5A};
  while (true) {
    zdi_flash_write_bytes(0x000000, buffer, 4);
    sleep_ms(1000);
  }
}

extern void process_status_command();

int main() {
  zdi_init_pins();

  stdio_init_all();

  // conduct_test();

wait_for_valid_connection:
  printf("\r\n---------------\r\n");
  zdi_wait_for_connection(report_connection, report_non_connection);
  printf("\r\n");

  zdi_configure_pins();

  zdi_wait_for_valid_identity(report_zdi_id, report_zdi_id_failed);
  printf("\r\n");

  printf("Available Firmware Version: %d.%d.%d.%d (%s)\r\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, REVISION_VERSION,
         FIRMWARE_DATE);

  zdi_full_reset();
  process_status_command();

  while (true) {
    printf("ZDI> ");
    if (read_line(zdi_connection_lost))
      process_command();
    else
      goto wait_for_valid_connection;
  }

  // while (true) {
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   while (gpio_get(ZDI_ZDA_PIN) == 0 || gpio_get(ZDI_ZCL_PIN) == 0)
  //     ;

  //   uint8_t r0 = zdi_rd_reg_byte(3);
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   printf("R0 = %02X\r\n", r0);
  //   sleep_ms(500);
  // }
}

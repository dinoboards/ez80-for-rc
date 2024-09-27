#include "pico/stdlib.h"
#include <stdio.h>
#ifdef CYW43_WL_GPIO_LED_PIN
#include "main.h"
#include "pico/cyw43_arch.h"
#endif
#include "command_dispatcher.h"
#include "read_line.h"
#include "zdi.h"

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

int main() {
  init_pins();

  stdio_init_all();

wait_for_valid_connection:
  printf("\r\n---------------\r\n");
  wait_for_detected_zdi_connection(report_connection, report_non_connection);
  printf("\r\n");

  configure_for_zdi_interface();

  wait_for_valid_identity(report_zdi_id, report_zdi_id_failed);
  printf("\r\n");

  while (true) {
    if (read_line(zdi_connection_lost))
      process_command();
    else
      goto wait_for_valid_connection;
  }

  // while (true) {
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   while (gpio_get(ZDI_ZDA_PIN) == 0 || gpio_get(ZDI_ZCL_PIN) == 0)
  //     ;

  //   uint8_t r0 = read_reg_byte(3);
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   printf("R0 = %02X\r\n", r0);
  //   sleep_ms(500);
  // }
}

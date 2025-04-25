#include <cpm.h>
#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

usb_keyboard_report_t usb_keyboard_report = {0};

int main(/*const int argc, const char *argv[]*/) {
  int8_t result;

  // write to default char
  result = ez80_uart_out('X');

  printf("\r\nez80_firm_uart_out result: %d\r\n", result);

  printf("\r\nWaiting for char on uart input\r\n");

  result = ez80_uart_in();

  printf("Received %c (%X)\r\n", result, result);

  result = ez80_usb_kyb_report(&usb_keyboard_report);
  printf("USB Report: (Press A key on its own to abort)\r\n");
  while (usb_keyboard_report.keyCode[0] != 4) { // A

    if (result != 0) {
      printf("\r\nUSB Key State: %X: Modifiers: %X, Scan Codes: (%X, %X, %X, %X, %X, %X)", result,
             usb_keyboard_report.bModifierKeys, usb_keyboard_report.keyCode[0], usb_keyboard_report.keyCode[1],
             usb_keyboard_report.keyCode[2], usb_keyboard_report.keyCode[3], usb_keyboard_report.keyCode[4],
             usb_keyboard_report.keyCode[5]);
    }

    result = ez80_usb_kyb_report(&usb_keyboard_report);
  }

  printf("Testing usb keyboard event que\r\n");
  ez80_usb_kyb_event_t usb_key;

  do {
    result = ez80_usb_kyb_event(&usb_key);

    if (result)
      printf(": %d, %x, %x\r\n", result, usb_key.key_code, usb_key.key_down);

  } while (!(result && usb_key.key_code == 4));
}

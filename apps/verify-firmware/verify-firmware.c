#include <cpm.h>
#include <ez80-firmware-usb.h>
#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

usb_keyboard_report_t    usb_keyboard_report = {0};
ez80_usb_mse_report_ex_t usb_mouse_report    = {0};

int main(/*const int argc, const char *argv[]*/) {
  usb_error_t result;

  // write to default char
  result = ez80_uart_out('X');

  printf("\r\nez80_firm_uart_out result: %d\r\n", result);

  printf("\r\nWaiting for char on uart input\r\n");

  result = ez80_uart_in();

  printf("Received %c (%X)\r\n", result, result);

  const uint8_t keyboard_index = ez80_usb_find_device_index(USB_IS_KEYBOARD);
  if (keyboard_index) {
    printf("Keyboard Found @ %d\r\n", keyboard_index);

    ez80_usb_kyb_flush();

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
    usb_kyb_event_t key_event;

    ez80_usb_kyb_flush();

    do {
      result = ez80_usb_kyb_event(&key_event);

      if (result)
        printf(": %d, %d:%c, %x, %x\r\n", result, key_event.key_ascii, key_event.key_ascii, key_event.key_code, key_event.key_down);

    } while (!(result && key_event.key_code == 4));
  } else
    printf("Keyboard not found\r\n");

  // find the mouse
  const uint8_t mouse_index = ez80_usb_find_device_index(USB_IS_MOUSE);
  if (mouse_index == 0) {
    printf("Mouse Not Found\r\n");
    return 0;
  }

  printf("Found mouse @ %d\r\n", mouse_index);

  result = ez80_usb_mse_init(0);
  printf("ez80_usb_mse_init(0): %d\r\n", result);

  char str_buffer[32];

  result = ez80_usb_get_manufacturer(mouse_index, str_buffer, sizeof(str_buffer));
  printf("Mouse Manufacturer:       %s\r\n", str_buffer);

  result = ez80_usb_get_product(mouse_index, str_buffer, sizeof(str_buffer));
  printf("Mouse Product:            %s\r\n", str_buffer);

  result = ez80_usb_mse_read(&usb_mouse_report);

  printf("USB Mouse Report: (Press left button on its own to abort)\r\n");
  while (usb_mouse_report.buttons != 1) { // left button
    if (result == 0) {
      ez80_usb_mse_state(&usb_mouse_report);
      printf("\r\n%d: Buttons: %x; Move: (%d, %d); Current Btn State: %x", result, usb_mouse_report.buttons, usb_mouse_report.x,
             usb_mouse_report.y, usb_mouse_report.buttons);
    }

    result = ez80_usb_mse_read(&usb_mouse_report);
  }
}

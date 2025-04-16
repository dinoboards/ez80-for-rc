#include <hbios.h>
#include <cpm.h>
#include <ez80.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vda_info_t            vda_info;
vda_keyrd_info_t      vda_keyrd_info;
usb_keyboard_report_t usb_keyboard_report = {0};

int main(/*const int argc, const char *argv[]*/) {
  int8_t result;
  uint16_t r;

  // write to default char
  result = hbios_cio_out(0x80, 'A');

  printf("\r\nhbios_cio_out result: %d\r\n", result);

  result = hbios_vda_qry(0x0, NULL, &vda_info);

  printf("\r\nhbios_vda_qry result: %d. (Mode: %d, Rows: %d, Cols: %d, FontMap: %p)\r\n", result, vda_info.video_mode,
         vda_info.rows, vda_info.columns, vda_info.font_map);


  r = hbios_vda_kstu(0, &usb_keyboard_report);

  printf("USB Report: (Press A key on its own to abort)\r\n");
  while (usb_keyboard_report.keyCode[0] != 4) { // A

    r = hbios_vda_kstu(0x0, &usb_keyboard_report);

    if ((r >> 8) != 0) {
      printf("\r\nUSB Key State: %X: Modifiers: %X, Scan Codes: (%X, %X, %X, %X, %X, %X)", r, usb_keyboard_report.bModifierKeys,
            usb_keyboard_report.keyCode[0], usb_keyboard_report.keyCode[1], usb_keyboard_report.keyCode[2],
            usb_keyboard_report.keyCode[3], usb_keyboard_report.keyCode[4], usb_keyboard_report.keyCode[5]);
      }
  }

  printf("\r\nClearing Que\r\n");
  while (result != 0) {
    result = hbios_vda_krd(0, &vda_keyrd_info);
    printf("hbios_vda_krd result: %d. (ScanCode: %X, KeyState: %X, KeyCode: %X)\r\n", result, vda_keyrd_info.scan_code,
           vda_keyrd_info.key_state, vda_keyrd_info.key_code);

    result = hbios_vda_kst(0x0);
  }
}

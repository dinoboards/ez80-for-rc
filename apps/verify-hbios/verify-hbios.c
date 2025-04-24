#include <cpm.h>
#include <ez80.h>
#include <hbios.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vda_info_t            vda_info;
vda_keyrd_info_t      vda_keyrd_info;
usb_keyboard_report_t usb_keyboard_report = {0};

int main(/*const int argc, const char *argv[]*/) {
  int8_t   result;

  // write to default char
  result = hbios_cio_out(0x80, 'A');

  printf("\r\nhbios_cio_out result: %d\r\n", result);

  result = hbios_vda_qry(0x0, NULL, &vda_info);

  printf("\r\nhbios_vda_qry result: %d. (Mode: %d, Rows: %d, Cols: %d, FontMap: %p)\r\n", result, vda_info.video_mode,
         vda_info.rows, vda_info.columns, vda_info.font_map);


  printf("USB Report: (Type A key to abort)\r\n");
  result = 0;
  while (result == 0) {

    // wait for a char in the que
    while(result == 0)
      result = hbios_vda_kst(0);

    result = hbios_vda_krd(0, &vda_keyrd_info);
    printf("hbios_vda_krd result: %d. (KeyCode: %X)\r\n", result, vda_keyrd_info.key_code);

    if (vda_keyrd_info.key_code == 'A')
      break;
  }
}

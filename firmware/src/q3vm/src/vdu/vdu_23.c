#include <string.h>
#include <vm-shared-mem.h>

#include <stdio.h>

/*
VDU 23,32 to 255,n1,n2,n3,n4,n5,n6,n7,n8
These redefine the printable ASCII characters. The bit pattern of each of the
parameters n1 to n8 corresponds to a row in the eight-by-eight grid of the
character
*/

static void vdu_redefine_font(void) { vdp_cpu_to_vram(&data[1], FONT_8X8_STORED_Y + data[0] * 8, 8); }

/*
VDU 23,0,n,m|
If n = 8, this sets the interlace as follows:
Value Effect
m = 0 Toggles the screen interlace state
m = 1 Sets the screen interlace state to the current *TV setting
m = &80 Turns the screen interlace off
m = &81 Turns the screen interlace on

If n = 10, then m defines the start line for the cursor and its appearance. Thus:
Bits Effect
0-4 define the start line
5-6 define its appearance:
Bit 6 Bit 5 Meaning
0 0 Steady
0 1 Off
1 0 Fast flash
1 1 Slow flash
If n = 11, then m defines the end line for the cursor
*/

static void vdu_cursor_or_interlace(void) {
  uint8_t n = data[1];
  uint8_t m = data[2];

  if (n == 8) { // set interlace
    // N/A
    return;
  }

  if (n == 10) { // m defines the start line for the cursor and its appearance.
    uint8_t start_line = m & 0xF;
    uint8_t appearance = (m >> 5) & 3;

    printf("Start line: %d\n", start_line);

    switch (appearance) {
    case 0:
      printf("Steady Cursor todo!\n");
      return;

    case 1:
      printf("Cursor off\n");
      return;

    case 2:
      printf("Fast flashing cursor\n");
      return;

    case 3:
      printf("Slow flashing cursor\n");
      return;
    }
  }

  // if (n == 11) { // m defines the end line for the cursor
  //   vdu_not_implemented(); //not sure what this means?
  // }

  // vdu_not_implemented();
}

/*
VDU 23 is a multi-purpose command taking nine parameters, of which the first
identifies a particular function. Each of the available functions is described below.
Eight additional parameters are required in each case
*/
void vdu_multi_purpose(void) {

  if (data[0] == 0) {
    vdu_cursor_or_interlace();
    return;
  }

  if (data[0] >= 32) {
    vdu_redefine_font();
    return;
  }
}

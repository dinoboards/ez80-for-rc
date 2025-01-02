#include <cpm.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define KEY_A 0x04 // Keyboard a and A
#define KEY_B 0x05 // Keyboard b and B
#define KEY_C 0x06 // Keyboard c and C
#define KEY_D 0x07 // Keyboard d and D
#define KEY_E 0x08 // Keyboard e and E
#define KEY_F 0x09 // Keyboard f and F
#define KEY_G 0x0a // Keyboard g and G
#define KEY_H 0x0b // Keyboard h and H
#define KEY_I 0x0c // Keyboard i and I
#define KEY_J 0x0d // Keyboard j and J
#define KEY_K 0x0e // Keyboard k and K
#define KEY_L 0x0f // Keyboard l and L
#define KEY_M 0x10 // Keyboard m and M
#define KEY_N 0x11 // Keyboard n and N
#define KEY_O 0x12 // Keyboard o and O
#define KEY_P 0x13 // Keyboard p and P
#define KEY_Q 0x14 // Keyboard q and Q
#define KEY_R 0x15 // Keyboard r and R
#define KEY_S 0x16 // Keyboard s and S
#define KEY_T 0x17 // Keyboard t and T
#define KEY_U 0x18 // Keyboard u and U
#define KEY_V 0x19 // Keyboard v and V
#define KEY_W 0x1a // Keyboard w and W
#define KEY_X 0x1b // Keyboard x and X
#define KEY_Y 0x1c // Keyboard y and Y
#define KEY_Z 0x1d // Keyboard z and Z

#define KEY_1 0x1e // Keyboard 1 and !
#define KEY_2 0x1f // Keyboard 2 and @
#define KEY_3 0x20 // Keyboard 3 and #
#define KEY_4 0x21 // Keyboard 4 and $
#define KEY_5 0x22 // Keyboard 5 and %
#define KEY_6 0x23 // Keyboard 6 and ^
#define KEY_7 0x24 // Keyboard 7 and &
#define KEY_8 0x25 // Keyboard 8 and *
#define KEY_9 0x26 // Keyboard 9 and (
#define KEY_0 0x27 // Keyboard 0 and )

#define KEY_ENTER 0x28 // Keyboard Return (ENTER)
#define KEY_ESC   0x29 // Keyboard ESCAPE

#define KEY_F1  0x3a // Keyboard F1
#define KEY_F2  0x3b // Keyboard F2
#define KEY_F3  0x3c // Keyboard F3
#define KEY_F4  0x3d // Keyboard F4
#define KEY_F5  0x3e // Keyboard F5
#define KEY_F6  0x3f // Keyboard F6
#define KEY_F7  0x40 // Keyboard F7
#define KEY_F8  0x41 // Keyboard F8
#define KEY_F9  0x42 // Keyboard F9
#define KEY_F10 0x43 // Keyboard F10
#define KEY_F11 0x44 // Keyboard F11
#define KEY_F12 0x45 // Keyboard F12

#define KEY_UP    0x52 // Keyboard Up Arrow
#define KEY_RIGHT 0x4f // Keyboard Right Arrow
#define KEY_LEFT  0x50 // Keyboard Left Arrow
#define KEY_DOWN  0x51 // Keyboard Down Arrow

typedef enum { EVENT_KEY_DOWN = 1, EVENT_KEY_UP = 2 } KeyEvent;

typedef struct {
  uint8_t  scan_code;
  KeyEvent event;
} KeyEventInfo;

static uint8_t last_key;

static bool poll_for_key_down_event(uint8_t *key) {
  uint8_t k = cpm_c_rawio();
  if (k == 0)
    return false;

  if (k >= 'A' && k <= 'Z')
    k = k - 'A' + KEY_A;

  else if (k >= 'a' && k <= 'z')
    k = k - 'a' + KEY_A;

  else if (k >= '0' && k <= '9')
    k = k - '0' + KEY_0;

  else if (k == 13)
    k = KEY_ENTER;

  else if (k == 27) {
    printf("ESC\r\n");

    uint8_t s = cpm_c_rawio();
    printf("next: %d\r\n", k);

    switch (s) {
    case 0:
      k = KEY_ESC;
      break;

    case 91:
      printf("91\r\n");

      s = cpm_c_rawio();
      printf("next.next: %d\r\n", s);
      if (s == 0)
        break;

      switch (s) {
      case 65: // up
        k = KEY_UP;
        break;
      case 66: // down
        k = KEY_DOWN;
        break;
      case 67: // right
        k = KEY_RIGHT;
        break;
      case 68: // left
        k = KEY_LEFT;
        break;
      default:
        printf("unknown: %d\r\n", s);
        break;
      }
      break;

    case 79:
      printf("79\r\n");
      s = cpm_c_rawio();
      printf("next.next: %d\r\n", s);
      if (s == 0)
        break;

      if (s >= 80 && s <= 91)
        k = s - 80 + KEY_F1;
      else
        printf("unknown: %d\r\n", s);
      break;

    default:
      printf("unknown: %d\r\n", s);
      break;
    }
  }

  *key = last_key = k;
  return true;
}

static bool poll_for_key_up_event(uint8_t *key) {
  if (last_key) {
    *key     = last_key;
    last_key = 0;
    return true;
  }
  return false;
}

bool poll_for_key_event(KeyEventInfo *key_event) {
  if (poll_for_key_up_event(&key_event->scan_code)) {
    key_event->event = EVENT_KEY_UP;
    return true;
  }

  if (poll_for_key_down_event(&key_event->scan_code)) {
    key_event->event = EVENT_KEY_DOWN;
    return true;
  }

  return false;
}

bool key_event_pending() {
  if (last_key != 0)
    return true;

  return cpm_c_stat();
}

void test_key_serial_to_event(void) {
  KeyEventInfo key_event = {0};

  // start by waiting for a char on the serial input

  while (!key_event_pending())
    ;

  poll_for_key_event(&key_event);

  printf("poll: %x, %d\r\n", key_event.scan_code, key_event.event);

  while (!key_event_pending())
    ;

  poll_for_key_event(&key_event);

  printf("poll: %x, %d\r\n", key_event.scan_code, key_event.event);
}

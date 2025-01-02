#include "keyboard.h"
#include <cpm.h>
#include <stdint.h>

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
    uint8_t s = cpm_c_rawio();

    switch (s) {
    case 0:
      k = KEY_ESC;
      break;

    case 91:
      s = cpm_c_rawio();
      if (s == 0)
        break;

      switch (s) {
      case 65:
        k = KEY_UP;
        break;
      case 66:
        k = KEY_DOWN;
        break;
      case 67:
        k = KEY_RIGHT;
        break;
      case 68:
        k = KEY_LEFT;
        break;
      default:
        break;
      }
      break;

    case 79:
      s = cpm_c_rawio();
      if (s == 0)
        break;

      if (s >= 80 && s <= 91)
        k = s - 80 + KEY_F1;
      break;

    default:
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

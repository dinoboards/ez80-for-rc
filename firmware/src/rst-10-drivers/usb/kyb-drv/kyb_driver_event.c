#include "class_hid_keyboard.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief USB HID Keyboard scan codes
 *
 * @notes Follows typical us keyboard assignments
 *
 * @see https://usb.org/sites/default/files/hut1_21.pdf
 *
 */
#define USB_KEY_CAPSLOCK 0x39
#define USB_KEY_LCTRL    0xE0 /* Left Control key */
#define USB_KEY_LSHIFT   0xE1 /* Left Shift key */
#define USB_KEY_LALT     0xE2 /* Left Alt key */
#define USB_KEY_LMETA    0xE3 /* Left Meta/Windows key */
#define USB_KEY_RCTRL    0xE4 /* Right Control key */
#define USB_KEY_RSHIFT   0xE5 /* Right Shift key */
#define USB_KEY_RALT     0xE6 /* Right Alt key */
#define USB_KEY_RMETA    0xE7 /* Right Meta/Windows key */

static bool    caps_lock_engaged;
static uint8_t modifier_bits;

uint24_t get_ascii_from_code(const uint16_t x) { // key code in low byte,key_down in upper byte
  uint8_t key_code = x & 255;
  uint8_t key_down = x >> 8;

  if (key_code == USB_KEY_CAPSLOCK && key_down) {
    caps_lock_engaged = !caps_lock_engaged;
    return x << 8;
  }

  if (key_code == USB_KEY_LCTRL) {
    if (key_down)
      modifier_bits |= KEY_MOD_LCTRL;
    else
      modifier_bits &= ~KEY_MOD_LCTRL;
  }

  if (key_code == USB_KEY_RCTRL) {
    if (key_down)
      modifier_bits |= KEY_MOD_RCTRL;
    else
      modifier_bits &= ~KEY_MOD_RCTRL;
  }

  if (key_code == USB_KEY_LSHIFT) {
    if (key_down)
      modifier_bits |= KEY_MOD_LSHIFT;
    else
      modifier_bits &= ~KEY_MOD_LSHIFT;
  }

  if (key_code == USB_KEY_RSHIFT) {
    if (key_down)
      modifier_bits |= KEY_MOD_RSHIFT;
    else
      modifier_bits &= ~KEY_MOD_RSHIFT;
  }

  return (x << 8) + scancode_to_char(modifier_bits, key_code, caps_lock_engaged);
}

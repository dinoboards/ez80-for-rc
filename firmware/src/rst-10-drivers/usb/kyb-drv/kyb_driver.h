#ifndef __KYB_DRIVER__
#define __KYB_DRIVER__

#include "../base-drv/ch376.h"
#include "class_hid_keyboard.h"
#include <stdint.h>

#define KEYBOARD_BUFFER_SIZE      8
#define KEYBOARD_BUFFER_SIZE_MASK 7

extern void usb_kyb_install_timer_tick(void);

extern usb_error usb_kyb_init(const uint8_t dev_index);
extern uint8_t   usb_kyb_flush(void);
extern uint8_t   usb_kyb_status(void);
extern uint24_t  usb_kyb_read();

#endif

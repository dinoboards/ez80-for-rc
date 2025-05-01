#ifndef __USB_STATE
#define __USB_STATE

#include "ch376.h"
#include "protocol.h"
#include <stdlib.h>

#define MAX_NUMBER_OF_DEVICES     6
#define DEVICE_CONFIG_STRUCT_SIZE sizeof(device_config_storage_t) /* Assumes is largest struct */

typedef struct __usb_state {
  uint8_t device_configs[DEVICE_CONFIG_STRUCT_SIZE * MAX_NUMBER_OF_DEVICES];

  uint8_t device_configs_end; // always zero to mark end
} usb_state_t;

extern device_config_t *find_first_free(void);
extern uint8_t          count_of_devices(void);
extern device_config_t *get_usb_device_config(const uint8_t device_index);
extern device_config_t *find_by_device_type(const usb_device_t dev_type);

extern device_config_t *usb_get_device_type(const uint8_t dev_index);

#endif

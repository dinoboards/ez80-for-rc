#ifndef __USB_ENUMERATE
#define __USB_ENUMERATE

#include "ch376.h"
#include "protocol.h"
#include "usb_state.h"

#define MAX_CONFIG_SIZE 140

typedef struct {
  uint8_t next_device_address; /* Track the count of installed usb devices*/
  uint8_t storage_count;       /* Track the count of storage devices (scsi, ufi) */
} enumeration_state_t;

typedef struct __working {
  enumeration_state_t *state;

  usb_device_t         usb_device;
  device_descriptor_t  desc;
  uint8_t              config_index;
  uint8_t              interface_count;
  uint8_t              endpoint_count;
  uint8_t              current_device_address;
  device_config_hub_t *hub_config;

  uint8_t         *ptr;
  device_config_t *p_current_device;

  union {
    uint8_t             buffer[MAX_CONFIG_SIZE];
    config_descriptor_t desc;
  } config;

} _working_t;

extern usb_error_t read_all_configs(enumeration_state_t *const state);
extern usb_error_t enumerate_all_devices(void);

#endif

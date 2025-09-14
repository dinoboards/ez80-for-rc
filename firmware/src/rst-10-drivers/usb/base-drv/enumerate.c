#include "enumerate.h"
#include "enumerate_hub.h"
#include "enumerate_storage.h"
#include "protocol.h"
#include "work-area.h"
#include <string.h>

usb_error_t op_id_class_drv(_working_t *const working);
usb_error_t op_parse_endpoint(_working_t *const working);

static usb_error_t advance_to_next_descriptor(_working_t *const working, const uint8_t descriptor_type) {
  usb_descriptor_t *d;
  const uint8_t    *buffer_end = working->config.buffer + MAX_CONFIG_SIZE;

  if (working->ptr >= buffer_end)
    return USB_ERR_BUFF_TO_LARGE;

  d = (usb_descriptor_t *)working->ptr;

  do {
    working->ptr += d->bLength;

    if (working->ptr >= buffer_end)
      return USB_ERR_BUFF_TO_LARGE;

    d = (usb_descriptor_t *)working->ptr;
  } while (d->bDescriptorType != descriptor_type);

  if (working->ptr + d->bLength >= buffer_end)
    return USB_ERR_BUFF_TO_LARGE;

  return USB_ERR_OK;
}

void parse_endpoint_keyboard(device_config_boot_hid_t *const keyboard_config, const endpoint_descriptor_t *pEndpoint) {
  endpoint_param_t *const ep = &keyboard_config->endpoints[0];
  ep->number                 = pEndpoint->bEndpointAddress;
  ep->toggle                 = 0;
  ep->max_packet_size        = calc_max_packet_size(pEndpoint->wMaxPacketSize);
}

usb_device_t identify_class_driver(_working_t *const working) {
  const interface_descriptor_t *const p = (const interface_descriptor_t *)working->ptr;
  if (p->bInterfaceClass == 2)
    return USB_IS_CDC;

  if (p->bInterfaceClass == 8 && (p->bInterfaceSubClass == 6 || p->bInterfaceSubClass == 5) && p->bInterfaceProtocol == 80)
    return USB_IS_MASS_STORAGE;

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0)
    return USB_IS_FLOPPY;

  if (p->bInterfaceClass == 9 && p->bInterfaceSubClass == 0 && p->bInterfaceProtocol == 0)
    return USB_IS_HUB;

  if (p->bInterfaceClass == 3 && p->bInterfaceSubClass == 1 && p->bInterfaceProtocol == 1) // Keyboard boot protocol
    return USB_IS_KEYBOARD;

  if (p->bInterfaceClass == 3 && p->bInterfaceSubClass == 1 && p->bInterfaceProtocol == 2) // Mouse boot protocol
    return USB_IS_MOUSE;

  return USB_IS_UNKNOWN;
}

usb_error_t op_interface_next(_working_t *const working) {
  uint8_t result;

  if (--working->interface_count == 0)
    return USB_ERR_OK;

  CHECK(advance_to_next_descriptor(working, USB_DESCR_INTERFACE));
  return op_id_class_drv(working);

done:
  return result;
}

usb_error_t op_endpoint_next(_working_t *const working) {
  usb_error_t result;

  if (working->endpoint_count != 0 && --working->endpoint_count > 0) {
    CHECK(advance_to_next_descriptor(working, USB_DESCR_ENDPOINT));
    return op_parse_endpoint(working);
  }

  return op_interface_next(working);

done:
  return result;
}

usb_error_t op_parse_endpoint(_working_t *const working) {
  const endpoint_descriptor_t *endpoint = (endpoint_descriptor_t *)working->ptr;
  device_config_t *const       device   = working->p_current_device;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE: {
    parse_endpoints((device_config_storage_t *)device, endpoint);
    break;
  }

  case USB_IS_KEYBOARD: {
    parse_endpoint_keyboard((device_config_boot_hid_t *)device, endpoint);
    break;
  }

  case USB_IS_MOUSE: {
    parse_endpoint_keyboard((device_config_boot_hid_t *)device, endpoint);
    break;
  }
  }

  return op_endpoint_next(working);
}

usb_error_t
configure_device(const _working_t *const working, const interface_descriptor_t *const interface, device_config_t *const dev_cfg) {
  dev_cfg->interface_number = interface->bInterfaceNumber;
  dev_cfg->max_packet_size  = working->desc.bMaxPacketSize0;
  dev_cfg->address          = working->current_device_address;
  dev_cfg->type             = working->usb_device;

  return usbtrn_set_configuration(dev_cfg->address, dev_cfg->max_packet_size, working->config.desc.bConfigurationvalue);
}

usb_error_t op_capture_hub_driver_interface(_working_t *const working) {
  const interface_descriptor_t *const interface = (interface_descriptor_t *)working->ptr;

  usb_error_t         result;
  device_config_hub_t hub_config;
  working->hub_config = &hub_config;

  hub_config.type = USB_IS_HUB;
  CHECK(configure_device(working, interface, (device_config_t *const)&hub_config));
  RETURN_CHECK(configure_usb_hub(working));
done:
  return result;
}

usb_error_t op_cap_drv_intf(_working_t *const working) {
  usb_error_t                         result;
  const interface_descriptor_t *const interface = (interface_descriptor_t *)working->ptr;

  working->endpoint_count = interface->bNumEndpoints;
  if (working->endpoint_count > 0)
    CHECK(advance_to_next_descriptor(working, USB_DESCR_ENDPOINT));
  working->p_current_device = NULL;

  switch (working->usb_device) {
  case USB_IS_HUB: {
    CHECK(op_capture_hub_driver_interface(working))
    break;
  }

  case USB_IS_UNKNOWN: {
    device_config_t unkown_dev_cfg;
    memset(&unkown_dev_cfg, 0, sizeof(device_config_t));
    working->p_current_device = &unkown_dev_cfg;
    CHECK(configure_device(working, interface, &unkown_dev_cfg));
    break;
  }

  default: {
    device_config_t *dev_cfg = find_first_free();
    if (dev_cfg == NULL)
      return USB_ERR_OUT_OF_MEMORY;
    working->p_current_device = dev_cfg;
    CHECK(configure_device(working, interface, dev_cfg));
    break;
  }
  }

  return op_parse_endpoint(working);

done:
  return result;
}

usb_error_t op_id_class_drv(_working_t *const working) {
  const interface_descriptor_t *const ptr = (const interface_descriptor_t *)working->ptr;

  if (ptr->bDescriptorType != USB_DESCR_INTERFACE)
    return USB_ERR_FAIL;

  working->usb_device = identify_class_driver(working);

  return op_cap_drv_intf(working);
}

usb_error_t op_get_cfg_desc(_working_t *const working) {
  usb_error_t result;

  const uint8_t max_packet_size = working->desc.bMaxPacketSize0;

  memset(working->config.buffer, 0, MAX_CONFIG_SIZE);
  working->ptr = working->config.buffer;

  CHECK(usbtrn_get_full_config_descriptor(working->config_index, working->current_device_address, max_packet_size, MAX_CONFIG_SIZE,
                                          working->config.buffer));

  CHECK(advance_to_next_descriptor(working, USB_DESCR_INTERFACE));
  working->interface_count = working->config.desc.bNumInterfaces;

  return op_id_class_drv(working);

done:
  return result;
}

usb_error_t read_all_configs(enumeration_state_t *const state) {
  uint8_t    result;
  uint8_t    config_index;
  _working_t working;
  uint8_t    retry_count = 0;

  memset(&working, 0, sizeof(_working_t));
  working.state = state;

retry:
  CHECK(usbtrn_get_descriptor(&working.desc));

  state->next_device_address++;
  working.current_device_address = state->next_device_address;
  CHECK(usbtrn_set_address(working.current_device_address));

  for (config_index = 0; config_index < working.desc.bNumConfigurations; config_index++) {
    working.config_index = config_index;

    CHECK(op_get_cfg_desc(&working));
  }

  return USB_ERR_OK;

done:
  if (result == USB_ERR_STALL && retry_count == 0) {
    retry_count++;
    ch_command(CMD1H_CLR_STALL);
    ch_get_status();
    goto retry;
  }
  return result;
}

usb_error_t enumerate_all_devices(void) {
  enumeration_state_t state;
  memset(&state, 0, sizeof(enumeration_state_t));

  return read_all_configs(&state);
}

/*
  enumerate_all_devices
    -> read_all_configs
      -> parse_config
        -> op_get_cfg_desc
          -> op_id_class_drv
            -> op_cap_drv_intf (increment index)
              -> op_parse_endpoint
                -> parse_endpoints
                -> parse_endpoint_hub
                -> op_endpoint_next
                  -> op_parse_endpoint -^ (install driver endpoint)
                  -> op_interface_next
                    -> return
                    -> op_id_class_drv -^


*/

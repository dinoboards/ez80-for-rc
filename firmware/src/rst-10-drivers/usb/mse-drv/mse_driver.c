#include "../base-drv/ch376.h"
#include "../base-drv/class_hid.h"
#include "../base-drv/dev_transfers.h"
#include "../base-drv/usb_state.h"
#include <stdint.h>

device_config_boot_hid_t *mouse_config = NULL;

usb_error_t usb_mse_init(const uint8_t dev_index) {
  uint8_t                   result;
  device_config_boot_hid_t *config;

  config = (device_config_boot_hid_t *)(dev_index == 0 ? find_by_device_type(USB_IS_MOUSE) : get_usb_device_config(dev_index));

  if (config == NULL)
    return USB_ERROR_DEVICE_NOT_FOUND;

  if (config->type != USB_IS_MOUSE)
    return USB_ERROR_DEVICE_NOT_FOUND;

  CHECK(hid_set_protocol(config, 1));
  CHECK(hid_set_idle(config, 0x80));

  mouse_config = config;

done:
  return USB_ERR_OK;
}

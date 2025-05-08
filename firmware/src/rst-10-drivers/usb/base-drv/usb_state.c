#include "usb_state.h"
#include "ch376.h"
#include "work-area.h"

#define /*device_config_t */ first_device_config(/*const usb_state_t *const*/ p) ((device_config_t *)&(usb_state.device_configs[0]))
extern device_config_t *next_device_config(const device_config_t *const p);

const uint8_t device_config_sizes[_USB_LAST_DEVICE_TYPE] = {
    0,                                /* USB_NOT_SUPPORTED   = 0 */
    sizeof(device_config_storage_t),  /* USB_IS_FLOPPY       = 1 */
    sizeof(device_config_storage_t),  /* USB_IS_MASS_STORAGE = 2 */
    sizeof(device_config_t),          /* USB_IS_CDC          = 3 */
    sizeof(device_config_boot_hid_t), /* USB_IS_KEYBOARD     = 4 */
    sizeof(device_config_boot_hid_t), /* USB_IS_MOUSE        = 5 */
};

// always usb work area
uint8_t count_of_devices(void) {
  uint8_t count = 0;

  const device_config_t *p_config = first_device_config();
  while (p_config) {
    const uint8_t type = p_config->type;

    if (type != USB_IS_HUB && type)
      count++;
    ;

    p_config = next_device_config(p_config);
  };

  return count;
}

// always search in boot
device_config_t *find_first_free(void) {
  device_config_t *p = first_device_config();
  while (p) {
    if (p->type == 0)
      return p;

    p = next_device_config(p);
  }

  return NULL;
}

device_config_t *next_device_config(const device_config_t *const p) {
  device_config_t *result;

  if (p->type == 0)
    return NULL;

  result = (device_config_t *)(((uint8_t *)p) + device_config_sizes[p->type]);

  if (result >= (device_config_t *)&usb_state.device_configs_end)
    return NULL;

  return result;
}

device_config_t *get_usb_device_config(const uint8_t device_index) {
  device_config_t *p;
  uint8_t          counter = 1;

  for (p = first_device_config(); p; p = next_device_config(p)) {
    if (p->type != USB_NOT_SUPPORTED) {
      if (counter == device_index)
        return p;
      counter++;
    }
  }

  return NULL; // is not a usb device
}

const device_config_t no_device = {-1, -1, -1, -1};
device_config_t      *usb_get_device_type(const uint8_t dev_index) {
  const device_config_t *dev = get_usb_device_config(dev_index);

  if (dev == NULL)
    return &no_device;

  return dev;
}

/**
 * @brief find the first device config with the specified dev_type
 *
 * @param dev
 * @return device_config_t
 *
 */
device_config_t *find_by_device_type(const usb_device_t dev_type) {
  device_config_t *p;

  for (p = first_device_config(); p; p = next_device_config(p)) {
    if (p->type != USB_NOT_SUPPORTED) {
      if (p->type == dev_type)
        return p;
    }
  }

  return NULL; // not found
}

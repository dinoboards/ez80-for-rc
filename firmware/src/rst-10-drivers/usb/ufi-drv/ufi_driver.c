#include "ufi_driver.h"
#include "../base-drv/dev_transfers.h"
#include "class_ufi.h"
#include <string.h>

uint32_t usb_ufi_get_cap(const uint16_t dev_index) {
  ufi_inquiry_response inquiry;
  usb_error            result;

  device_config_storage *const dev = (device_config_storage *)get_usb_device_config(dev_index);

  ufi_format_capacities_response response;
  memset(&response, 0, sizeof(ufi_format_capacities_response));

  wait_for_device_ready((device_config *)dev, 25);

  // not sure if we need to do this to 'clear' some state
  ufi_inquiry((device_config *)dev, &inquiry);

  wait_for_device_ready((device_config *)dev, 15);

  result = ufi_read_frmt_caps((device_config *)dev, &response);
  if (result != USB_ERR_OK)
    return 0;

  return convert_from_msb_first(response.descriptors[0].number_of_blocks);
}

usb_error usb_ufi_read(const uint16_t dev_index, uint8_t *const buffer) {
  usb_error                  result;
  ufi_interrupt_status       sense_codes;
  ufi_request_sense_response response;
  uint8_t                    asc;
  uint8_t                    ascq;
  uint8_t                    sense_key;

  device_config_storage *const dev = (device_config_storage *)get_usb_device_config(dev_index);

  if (wait_for_device_ready((device_config *)dev, 20) != 0)
    return -1; // Not READY!

  memset(&sense_codes, 0, sizeof(sense_codes));

  if (ufi_read_write_sector((device_config *)dev, false, dev->current_lba, 1, buffer, (uint8_t *)&sense_codes) != USB_ERR_OK)
    return -1; // general error

  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense((device_config *)dev, &response)) != USB_ERR_OK)
    return -1; // error

  asc       = response.asc;
  ascq      = response.ascq;
  sense_key = response.sense_key & 15;

  if (sense_key != 0)
    return -1;

  return USB_ERR_OK;
}

usb_error usb_ufi_write(const uint16_t dev_index, uint8_t *const buffer) {
  ufi_interrupt_status       sense_codes;
  ufi_request_sense_response response;
  uint8_t                    asc;
  uint8_t                    ascq;
  uint8_t                    sense_key;

  device_config_storage *const dev = (device_config_storage *)get_usb_device_config(dev_index);

  if (wait_for_device_ready((device_config *)dev, 20) != 0)
    return -1; // Not READY!

  memset(&sense_codes, 0, sizeof(sense_codes));
  if ((ufi_read_write_sector((device_config *)dev, true, dev->current_lba, 1, buffer, (uint8_t *)&sense_codes)) != USB_ERR_OK) {
    return -1;
  }

  memset(&response, 0, sizeof(response));

  if ((ufi_request_sense((device_config *)dev, &response)) != USB_ERR_OK) {
    return -1;
  }

  asc       = response.asc;
  ascq      = response.ascq;
  sense_key = response.sense_key & 15;

  if (sense_key != 0)
    return -1;

  return USB_ERR_OK;
}

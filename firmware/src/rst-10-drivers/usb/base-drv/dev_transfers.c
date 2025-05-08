/**
 * @file transfers.c
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "dev_transfers.h"
#include "ch376.h"
#include "critical-section.h"
#include "delay.h"
#include "protocol.h"
#include <stdlib.h>

usb_error_t usbdev_blk_out_trnsfer(device_config_t *const dev, const uint8_t *const buffer, const uint16_t buffer_size) {
  usb_error_t result;

  endpoint_param_t *const endpoint = &dev->endpoints[ENDPOINT_BULK_OUT];

  result = usb_data_out_transfer(buffer, buffer_size, dev->address, endpoint);

  if (result == USB_ERR_STALL) {
    usbtrn_clear_endpoint_halt(endpoint->number, dev->address, dev->max_packet_size);
    endpoint->toggle = 0;
    return USB_ERR_STALL;
  }

  return result;
}

usb_error_t usbdev_bulk_in_transfer(device_config_t *const dev, uint8_t *const buffer, uint8_t *const buffer_size) {
  usb_error_t result;

  endpoint_param_t *const endpoint = &dev->endpoints[ENDPOINT_BULK_IN];

  result = usb_data_in_transfer_n(buffer, buffer_size, dev->address, endpoint);

  if (result == USB_ERR_STALL) {
    usbtrn_clear_endpoint_halt(endpoint->number, dev->address, dev->max_packet_size);
    endpoint->toggle = 0;
    return USB_ERR_STALL;
  }

  return result;
}

usb_error_t usbdev_dat_in_trnsfer(device_config_t *const device,
                                  uint8_t *const         buffer,
                                  const uint16_t         buffer_size,
                                  const usb_endpoint_t   endpoint_type) {
  usb_error_t result;

  endpoint_param_t *const endpoint = &device->endpoints[endpoint_type];

  result = usb_data_in_transfer(buffer, buffer_size, device->address, endpoint);

  if (result == USB_ERR_STALL) {
    usbtrn_clear_endpoint_halt(endpoint->number, device->address, device->max_packet_size);
    endpoint->toggle = 0;
    return USB_ERR_STALL;
  }

  return result;
}

usb_error_t usbdev_dat_in_trnsfer_0(device_config_t *const device, uint8_t *const buffer, const uint8_t buffer_size) {
  usb_error_t             result;
  endpoint_param_t *const endpoint = &device->endpoints[0];

  result = usb_data_in_transfer(buffer, buffer_size, device->address, endpoint);

  if (result == USB_ERR_STALL) {
    usbtrn_clear_endpoint_halt(endpoint->number, device->address, device->max_packet_size);
    endpoint->toggle = 0;
    return USB_ERR_STALL;
  }

  return result;
}

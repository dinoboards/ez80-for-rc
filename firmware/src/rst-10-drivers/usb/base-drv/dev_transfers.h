/**
 * @file transfer.h
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __USBDEV_TRANSFERS
#define __USBDEV_TRANSFERS

#include "ch376.h"
#include "transfers.h"
#include <stdlib.h>

// 3 bytes
#define COMMON_DEVICE_CONFIG                                                                                                       \
  usb_device_t type;                                                                                                               \
  uint8_t      address;                                                                                                            \
  uint8_t      max_packet_size;                                                                                                    \
  uint8_t      interface_number

typedef struct {
  COMMON_DEVICE_CONFIG;
  endpoint_param_t endpoints[3]; // bulk in/out and interrupt
} device_config_t;

typedef struct {
  COMMON_DEVICE_CONFIG;            // bytes: 0-3
  endpoint_param_t endpoints[3];   // bytes: 4-6, 7-9, 10-12  bulk in/out and interrupt
  uint8_t          current_lba[4]; // bytes 13-16
} device_config_storage_t;

typedef struct {
  COMMON_DEVICE_CONFIG;
} device_config_hub_t;

typedef struct {
  COMMON_DEVICE_CONFIG;
  endpoint_param_t endpoints[1]; // Isochronous
} device_config_boot_hid_t;

extern usb_error_t usbdev_control_transfer(device_config_t *const device, const setup_packet_t *const cmd, uint8_t *const buffer);

extern usb_error_t usbdev_blk_out_trnsfer(device_config_t *const device, const uint8_t *const buffer, const uint16_t buffer_size);

extern usb_error_t usbdev_bulk_in_transfer(device_config_t *const dev, uint8_t *const buffer, uint8_t *const buffer_size);

extern usb_error_t usbdev_dat_in_trnsfer(device_config_t *const device,
                                         uint8_t *const         buffer,
                                         const uint16_t         buffer_size,
                                         const usb_endpoint_t   endpoint_type);

extern usb_error_t usbdev_dat_in_trnsfer_0(device_config_t *const device, uint8_t *const buffer, const uint8_t buffer_size);

/**
 * @brief Perform a USB control transfer (in or out)
 * See https://www.beyondlogic.org/usbnutshell/usb4.shtml for a description of the USB control transfer
 *
 * @param device the usb device
 * @param cmd_packet Pointer to the setup packet - top bit of bmRequestType indicate data direction
 * @param buffer Pointer of data to send or receive into
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
#define /*usb_error_t*/ usbdev_control_transfer(/*device_config_t *const*/ device, /*const setup_packet_t *const*/ cmd_packet,     \
                                                /*uint8_t *const */ buffer)                                                        \
  usb_control_transfer(cmd_packet, buffer, device->address, device->max_packet_size)

#endif

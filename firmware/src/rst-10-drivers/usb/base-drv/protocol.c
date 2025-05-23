/**
 * @file protocol.c
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "protocol.h"
#include "ch376.h"
#include "delay.h"
#include <stdlib.h>

const setup_packet_t cmd_get_device_descriptor = {0x80, 6, {0, 1}, {0, 0}, 8};

/**
 * @brief Issue GET_DESCRIPTOR request to retrieve the device descriptor for usb device at address 0
 *
 * @param buffer the buffer to store the device descriptor in
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
usb_error_t usbtrn_get_descriptor(device_descriptor_t *const buffer) {
  uint8_t        result;
  setup_packet_t cmd;
  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 8;

  CHECK(usb_control_transfer(&cmd, (uint8_t *)buffer, 0, 8));

  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 18;
  return usb_control_transfer(&cmd, (uint8_t *)buffer, 0, buffer->bMaxPacketSize0);

done:
  return result;
}

/**
 * @brief Issue GET_DESCRIPTOR request to retrieve the device descriptor for usb device at the specified address
 *
 * @param buffer the buffer to store the device descriptor in
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
usb_error_t usbtrn_get_descriptor2(device_descriptor_t *const buffer, const uint8_t device_address) {
  uint8_t        result;
  setup_packet_t cmd;
  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 8;

  CHECK(usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, 8));

  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 18;

  return usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, buffer->bMaxPacketSize0);

done:
  return result;
}

const setup_packet_t cmd_set_device_address = {0x00, 5, {0, 0}, {0, 0}, 0};

/**
 * @brief configure device at address 0 to be assigned a new device address
 *
 * @param device_address the new device address
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
usb_error_t usbtrn_set_address(const uint8_t device_address) {
  setup_packet_t cmd;
  cmd           = cmd_set_device_address;
  cmd.bValue[0] = device_address;

  return usb_control_transfer(&cmd, 0, 0, 0);
}

const setup_packet_t cmd_set_configuration = {0x00, 9, {0, 0}, {0, 0}, 0};

/**
 * @brief configure device_address to be assigned a new configuration
 *
 * @param config the device to be configured
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
usb_error_t usbtrn_set_configuration(const uint8_t device_address, const uint8_t max_packet_size, const uint8_t configuration) {
  setup_packet_t cmd;
  cmd           = cmd_set_configuration;
  cmd.bValue[0] = configuration;

  return usb_control_transfer(&cmd, 0, device_address, max_packet_size);
}

const setup_packet_t cmd_get_config_descriptor = {0x80, 6, {0, 2}, {0, 0}, 0};

/**
 * @brief request the config descriptor for the specific config index of the specified device
 *
 * @param buffer the buffer to store the config descriptor in
 * @param config_index the index of the config descriptor to retrieve
 * @param buffer_size the size of the buffer
 * @param device_address the usb address of the device
 * @param max_packet_size the max packet size for control transfers (endpoint 0)
 * @return usb_error_t USB_ERR_OK if all good, otherwise specific error code
 */
usb_error_t usbtrn_get_config_descriptor(config_descriptor_t *const buffer,
                                         const uint8_t              config_index,
                                         const uint8_t              buffer_size,
                                         const uint8_t              device_address,
                                         const uint8_t              max_packet_size) {
  setup_packet_t cmd;
  cmd           = cmd_get_config_descriptor;
  cmd.bValue[0] = config_index;
  cmd.wLength   = (uint16_t)buffer_size;

  return usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, max_packet_size);
}

usb_error_t usbtrn_get_full_config_descriptor(const uint8_t  config_index,
                                              const uint8_t  device_address,
                                              const uint8_t  max_packet_size,
                                              const uint8_t  max_buffer_size,
                                              uint8_t *const buffer) {
  uint8_t result;
  uint8_t max_length;

  CHECK(usbtrn_get_config_descriptor((config_descriptor_t *)buffer, config_index, sizeof(config_descriptor_t), device_address,
                                     max_packet_size));

  max_length = ((config_descriptor_t *)buffer)->wTotalLength;
  if (max_length > max_buffer_size)
    max_length = max_buffer_size;

  return usbtrn_get_config_descriptor((config_descriptor_t *)buffer, config_index, max_length, device_address, max_packet_size);

done:
  return result;
}

const setup_packet_t usb_cmd_clear_endpoint_halt = {2, 1, {0, 0}, {255, 0}, 0}; //    ;byte 4 is the endpoint to be cleared

usb_error_t usbtrn_clear_endpoint_halt(const uint8_t endpoint_number, const uint8_t device_address, const uint8_t max_packet_size) {
  setup_packet_t cmd;
  cmd           = usb_cmd_clear_endpoint_halt;
  cmd.bIndex[0] = endpoint_number;

  return usb_control_transfer(&cmd, (uint8_t *)0, device_address, max_packet_size);
}

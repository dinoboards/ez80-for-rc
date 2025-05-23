#include "ch376.h"
#include <stdbool.h>

usb_error_t ch_get_status(void) {
  uint8_t ch_status;
  ch_command(CH_CMD_GET_STATUS);
  ch_status = CH376_DATA_PORT;

  if (ch_status >= USB_FILERR_MIN && ch_status <= USB_FILERR_MAX)
    return ch_status;

  if (ch_status == CH_CMD_RET_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_USB_INT_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_USB_INT_CONNECT)
    return USB_INT_CONNECT;

  if (ch_status == CH_USB_INT_DISK_READ)
    return USB_ERR_DISK_READ;

  if (ch_status == CH_USB_INT_DISK_WRITE)
    return USB_ERR_DISK_WRITE;

  if (ch_status == CH_USB_INT_DISCONNECT) {
    ch_cmd_set_usb_mode(5);
    return USB_ERR_NO_DEVICE;
  }

  if (ch_status == CH_USB_INT_BUF_OVER)
    return USB_ERR_DATA_ERROR;

  ch_status &= 0x2F;

  if (ch_status == 0x2A)
    return USB_ERR_NAK;

  if (ch_status == 0x2E)
    return USB_ERR_STALL;

  ch_status &= 0x23;

  if (ch_status == 0x20)
    return USB_ERR_TIMEOUT;

  if (ch_status == 0x23)
    return USB_TOKEN_OUT_OF_SYNC;

  return USB_ERR_UNEXPECTED_STATUS_FROM_HOST;
}

void ch_cmd_reset_all(void) { ch_command(CH_CMD_RESET_ALL); }

uint8_t ch_cmd_check_exist(void) {
  uint8_t complement;
  ch_command(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x55;
  complement      = CH376_DATA_PORT;
  return complement == 0x55;
}

uint8_t ch_probe(void) {
  uint8_t i = 5;
  do {
    if (ch_cmd_check_exist())
      return true;

    delay_medium();
  } while (--i != 0);

  return false;
}

usb_error_t ch_cmd_set_usb_mode(const uint8_t mode) {
  uint8_t result = 0;
  uint8_t count  = 127;

  CH376_COMMAND_PORT = CH_CMD_SET_USB_MODE;
  CH376_DATA_PORT    = mode;

  while (result != CH_CMD_RET_SUCCESS && result != CH_CMD_RET_ABORT && --count != 0) {
    result = CH376_DATA_PORT;
  }

  return (result == CH_CMD_RET_SUCCESS) ? USB_ERR_OK : USB_ERR_FAIL;
}

uint8_t ch_cmd_get_ic_version(void) {
  ch_command(CH_CMD_GET_IC_VER);
  return CH376_DATA_PORT & 0x1f;
}

usb_error_t ch_data_in_transfer(uint8_t *buffer, int16_t buffer_size, endpoint_param_t *const endpoint) {
  uint8_t count;
  uint8_t result;

  if (buffer_size == 0)
    return USB_ERR_OK;

  USB_MODULE_LEDS = 0x01;
  do {
    ch_issue_token_in(endpoint);

    CHECK(ch_long_wait_int_and_get_status());

    endpoint->toggle = !endpoint->toggle;

    count = ch_read_data(buffer);

    if (count == 0) {
      USB_MODULE_LEDS = 0x00;
      return USB_ERR_DATA_ERROR;
    }

    buffer += count;
    buffer_size -= count;
  } while (buffer_size > 0);

  USB_MODULE_LEDS = 0x00;
  return USB_ERR_OK;

done:
  USB_MODULE_LEDS = 0x00;
  return result;
}

usb_error_t ch_data_in_transfer_n(uint8_t *const buffer, uint8_t *const buffer_size, endpoint_param_t *const endpoint) {
  uint8_t     count;
  usb_error_t result;

  USB_MODULE_LEDS = 0x01;

  ch_issue_token_in(endpoint);

  CHECK(ch_long_wait_int_and_get_status());

  endpoint->toggle = !endpoint->toggle;

  count = ch_read_data(buffer);

  *buffer_size = count;

  USB_MODULE_LEDS = 0x00;

  return USB_ERR_OK;
done:
  USB_MODULE_LEDS = 0x00;
  return result;
}

usb_error_t ch_data_out_transfer(const uint8_t *buffer, int16_t buffer_length, endpoint_param_t *const endpoint) {
  usb_error_t   result;
  const uint8_t number          = endpoint->number;
  const uint8_t max_packet_size = endpoint->max_packet_size;

  USB_MODULE_LEDS = 0x02;

  while (buffer_length > 0) {
    const uint8_t size = max_packet_size < buffer_length ? max_packet_size : buffer_length;
    buffer             = ch_write_data(buffer, size);
    buffer_length -= size;
    ch_issue_token_out(endpoint);

    CHECK(ch_long_wait_int_and_get_status());

    endpoint->toggle = !endpoint->toggle;
  }

  USB_MODULE_LEDS = 0x00;
  return USB_ERR_OK;

done:
  USB_MODULE_LEDS = 0x00;
  return result;
}

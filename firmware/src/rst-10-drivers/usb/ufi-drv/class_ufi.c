#include "class_ufi.h"
#include "../base-drv/delay.h"
#include "../base-drv/protocol.h"
#include <stdlib.h>
#include <string.h>

const ufi_request_sense_command_T          _ufi_cmd_request_sense          = {0x03, 0, 0, 0, 18, {0, 0, 0, 0, 0, 0, 0}};
const ufi_read_format_capacities_command_t _ufi_cmd_read_format_capacities = {0x23, 0, {0, 0, 0, 0, 0}, {0, 12}, {0, 0, 0}};
const ufi_inquiry_command_t                _ufi_cmd_inquiry                = {0x12, 0, 0, 0, 0x24, {0, 0, 0, 0, 0, 0, 0}};
const ufi_format_command                   _ufi_cmd_format          = {0x04, 7 | 1 << 4, 0, {0, 0}, {0, 0}, {0, 0}, {0, 0, 0}};
const ufi_send_diagnostic_command          _ufi_cmd_send_diagnostic = {0x1D, 1 << 2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

uint8_t wait_for_device_ready(device_config_t *const storage_device, uint8_t timeout_counter) {
  usb_error_t                result;
  ufi_request_sense_response sense;

  do {
    memset(&sense, 0, sizeof(sense));
    result = ufi_test_unit_ready(storage_device, &sense);

    if ((result == USB_ERR_OK && (sense.sense_key & 15) == 0) || timeout_counter-- == 0)
      break;

    delay_medium();

  } while (true);

  return result | (sense.sense_key & 15);
}

usb_error_t ufi_test_unit_ready(device_config_t *const storage_device, ufi_request_sense_response const *response) {
  usb_error_t                   result;
  ufi_test_unit_ready_command_t ufi_cmd_request_test_unit_ready;
  ufi_request_sense_command_T   ufi_cmd_request_sense;

  memset(&ufi_cmd_request_test_unit_ready, 0, sizeof(ufi_test_unit_ready_command_t));

  usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_test_unit_ready, false, 0, NULL, NULL);

  ufi_cmd_request_sense = _ufi_cmd_request_sense;

  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                           (uint8_t *)response, NULL);
  RETURN_CHECK(result);
done:
  return result;
}

usb_error_t ufi_request_sense(device_config_t *const storage_device, ufi_request_sense_response const *response) {
  ufi_request_sense_command_T ufi_cmd_request_sense;
  usb_error_t                 result;

  ufi_cmd_request_sense = _ufi_cmd_request_sense;

  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                           (uint8_t *)response, NULL);

  RETURN_CHECK(result);
done:
  return result;
}

usb_error_t ufi_read_frmt_caps(device_config_t *const storage_device, ufi_format_capacities_response_t const *response) {
  usb_error_t                          result;
  ufi_read_format_capacities_command_t ufi_cmd_read_format_capacities;
  uint8_t                              available_length;
  uint8_t                              max_length;
  ufi_read_format_capacities_command_t cmd;

  ufi_cmd_read_format_capacities = _ufi_cmd_read_format_capacities;
  CHECK(usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_read_format_capacities, false, 12, (uint8_t *)response, NULL));

  available_length = response->capacity_list_length;

  max_length =
      available_length > sizeof(ufi_format_capacities_response_t) ? sizeof(ufi_format_capacities_response_t) : available_length;

  memcpy(&cmd, &ufi_cmd_read_format_capacities, sizeof(cmd));
  cmd.allocation_length[1] = max_length;

  return usb_execute_cbi(storage_device, (uint8_t *)&cmd, false, max_length, (uint8_t *)response, NULL);

done:
  return result;
}

usb_error_t ufi_inquiry(device_config_t *const storage_device, ufi_inquiry_response_t const *response) {
  ufi_inquiry_command_t ufi_cmd_inquiry;

  ufi_cmd_inquiry = _ufi_cmd_inquiry;

  return usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_inquiry, false, sizeof(ufi_inquiry_response_t), (uint8_t *)response,
                         NULL);
}

usb_error_t ufi_read_write_sector(device_config_t *const storage_device,
                                  const bool             send,
                                  const uint16_t         sector_number,
                                  const uint8_t          sector_count,
                                  uint8_t *const         buffer,
                                  uint8_t *const         sense_codes) {
  ufi_read_write_command_t cmd;

  memset(&cmd, 0, sizeof(cmd));
  cmd.operation_code     = send ? 0x2A : 0x28;
  cmd.lba[2]             = sector_number >> 8;
  cmd.lba[3]             = sector_number & 0xFF;
  cmd.transfer_length[1] = sector_count;

  return usb_execute_cbi(storage_device, (uint8_t *)&cmd, send, 512 * sector_count, (uint8_t *)buffer, sense_codes);
}

/**
 * Medium | Medium Type Code | Capacity | Tracks | Heads | Sectors/Track | Total Blocks | Block Length |
 * DD     | 1Eh              | 720 KB   | 80     | 2     | 9             | 1440 05A0h   | 512 0200h    |
 * HD     | 93h              | 1.25 MB  | 77     | 2     | 8             | 1232 04D0h   | 1024 0400h   |
 * HD     | 94h              | 1.44 MB  | 80     | 2     | 18            | 2880 0B40h   | 512 0200h    |
 */

usb_error_t ufi_format(device_config_t *const                        storage_device,
                       const uint8_t                                 side,
                       const uint8_t                                 track_number,
                       const ufi_format_capacity_descriptor_t *const format) {
  ufi_interrupt_status_t      sense_codes;
  ufi_format_command          cmd;
  ufi_format_parameter_list_t parameter_list;

  memset(&parameter_list, 0, sizeof(parameter_list));

  cmd = _ufi_cmd_format;

  cmd.track_number             = track_number;
  cmd.interleave[1]            = 0;
  cmd.parameter_list_length[1] = sizeof(parameter_list);

  parameter_list.defect_list_header.status =
      FMT_DEFECT_STATUS_FOV | FMT_DEFECT_STATUS_DCRT | FMT_DEFECT_STATUS_SINGLE_TRACK | (side & 1);

  parameter_list.defect_list_header.defect_list_length_msb = 0;
  parameter_list.defect_list_header.defect_list_length_lsb = 8;
  memcpy(&parameter_list.format_descriptor, (void *)format, sizeof(ufi_format_capacity_descriptor_t));

  return usb_execute_cbi(storage_device, (uint8_t *)&cmd, true, sizeof(parameter_list), (uint8_t *)&parameter_list,
                         (void *)&sense_codes);
}

usb_error_t ufi_send_diagnostics(device_config_t *const storage_device) {
  ufi_send_diagnostic_command ufi_cmd_send_diagnostic;

  ufi_cmd_send_diagnostic = _ufi_cmd_send_diagnostic;

  return usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_send_diagnostic, true, 0, NULL, NULL);
}

uint32_t convert_from_msb_first(const uint8_t *const buffer) {
  uint32_t       result;
  uint8_t       *p_output = ((uint8_t *)&result);
  const uint8_t *p_input  = buffer + 3;

  *p_output++ = *p_input--;
  *p_output++ = *p_input--;
  *p_output++ = *p_input--;
  *p_output   = *p_input--;

  return result;
}

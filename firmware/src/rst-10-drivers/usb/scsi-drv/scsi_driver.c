#include "scsi_driver.h"
#include "../base-drv/ch376.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/usb_state.h"
#include "class_scsi.h"
#include <string.h>

usb_error_t usb_scsi_init(const uint16_t dev_index) {
  uint8_t                        result;
  scsi_sense_result_t            response;
  device_config_storage_t *const dev     = (device_config_storage_t *)get_usb_device_config(dev_index);
  uint8_t                        counter = 3;

  critical_begin();
  while ((result = scsi_test(dev)) && --counter > 0)
    scsi_request_sense(dev, &response);
  critical_end();

  return result;
}

const static scsi_read_capacity_t scsi_packet_read_capacity = {0x25, 0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0}};

usb_error_t usb_scsi_read_capacity(const uint16_t dev_index, scsi_read_capacity_result_t *cap_result) {
  device_config_storage_t *const dev = (device_config_storage_t *)get_usb_device_config(dev_index);

  cbw_scsi_read_capacity_t cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.read_capacity = scsi_packet_read_capacity;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(scsi_read_capacity_t);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_read_capacity_result_t);

  return do_scsi_cmd(dev, &cbw_scsi.cbw, cap_result, false);
}

// scsi_packet_inquiry_t scsi_packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

// usb_error_t usb_scsi_inquiry(const uint16_t dev_index, scsi_inquiry_result_t *inq_result) {
//   device_config_storage_t *const dev = (device_config_storage_t *)get_usb_device_config(dev_index);

//   cbw_scsi_inquiry_t cbw_scsi;
//   cbw_scsi.cbw     = scsi_command_block_wrapper;
//   cbw_scsi.inquiry = scsi_packet_inquiry;

//   cbw_scsi.cbw.bCBWLUN                = 0;
//   cbw_scsi.cbw.bCBWCBLength           = sizeof(scsi_packet_inquiry_t);
//   cbw_scsi.cbw.dCBWDataTransferLength = 0x24;

//   return do_scsi_cmd(dev, &cbw_scsi.cbw, inq_result, false);
// }

usb_error_t usb_scsi_read(const uint16_t dev_index, uint8_t *const buffer) {
  uint8_t                        result;
  cbw_scsi_read_write_t          cbw = {{{0}}};
  device_config_storage_t *const dev = (device_config_storage_t *)get_usb_device_config(dev_index);

  cbw.cbw = scsi_command_block_wrapper;

  cbw.cbw.bCBWLUN                = 0;
  cbw.cbw.bCBWCBLength           = sizeof(scsi_packet_read_write_t);
  cbw.cbw.dCBWDataTransferLength = 512;

  cbw.scsi_cmd.operation_code  = 0x28; // read operation
  cbw.scsi_cmd.transfer_len[1] = 1;
  cbw.scsi_cmd.lba[0]          = dev->current_lba >> 24;
  cbw.scsi_cmd.lba[1]          = dev->current_lba >> 16;
  cbw.scsi_cmd.lba[2]          = dev->current_lba >> 8;
  cbw.scsi_cmd.lba[3]          = dev->current_lba;

  result = do_scsi_cmd(dev, &cbw.cbw, buffer, false);

  if (result == USB_ERR_OK)
    dev->current_lba++;

  return result;
}

usb_error_t usb_scsi_write(const uint16_t dev_index, uint8_t *const buffer) {
  uint8_t                        result;
  cbw_scsi_read_write_t          cbw = {{{0}}};
  device_config_storage_t *const dev = (device_config_storage_t *)get_usb_device_config(dev_index);

  cbw.cbw = scsi_command_block_wrapper;

  cbw.cbw.bCBWLUN                = 0;
  cbw.cbw.bCBWCBLength           = sizeof(scsi_packet_read_write_t);
  cbw.cbw.dCBWDataTransferLength = 512;

  cbw.scsi_cmd.operation_code  = 0x2A; // write operation
  cbw.scsi_cmd.transfer_len[1] = 1;
  cbw.scsi_cmd.lba[0]          = dev->current_lba >> 24;
  cbw.scsi_cmd.lba[1]          = dev->current_lba >> 16;
  cbw.scsi_cmd.lba[2]          = dev->current_lba >> 8;
  cbw.scsi_cmd.lba[3]          = dev->current_lba;

  result = do_scsi_cmd(dev, &cbw.cbw, buffer, true);

  if (result == USB_ERR_OK)
    dev->current_lba++;

  return result;
}

// usb_error_t scsi_eject(device_config_storage_t *const dev) {
//   cbw_scsi_eject_t cbw_scsi;
//   cbw_scsi.cbw = scsi_command_block_wrapper;

//   memset(&cbw_scsi.eject, 0, sizeof(scsi_packet_eject_t));

//   cbw_scsi.eject.operation_code = 0x1B;
//   cbw_scsi.eject.loej           = 1;

//   cbw_scsi.cbw.bCBWLUN                = 0;
//   cbw_scsi.cbw.bCBWCBLength           = sizeof(scsi_packet_eject_t);
//   cbw_scsi.cbw.dCBWDataTransferLength = 0;

//   return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
// }

#include "class_scsi.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/usb_state.h"
#include <string.h>

const scsi_command_block_wrapper_t scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0}, 0, 0, 0, 0};

uint16_t next_tag = 0;

usb_error_t do_scsi_cmd(device_config_storage_t *const      dev,
                        scsi_command_block_wrapper_t *const cbw,
                        void *const                         send_receive_buffer,
                        const bool                          send) {
  usb_error_t result;

  scsi_command_status_wrapper_t csw = {{{0}}};

  cbw->dCBWTag[0] = next_tag++;

  if (!send)
    cbw->bmCBWFlags = 0x80;

  critical_begin();

  CHECK(usb_data_out_transfer((uint8_t *)cbw, sizeof(scsi_command_block_wrapper_t) + 16, dev->address,
                              &dev->endpoints[ENDPOINT_BULK_OUT]));

  if (cbw->dCBWDataTransferLength != 0) {
    if (!send) {
      CHECK(usb_data_in_transfer(send_receive_buffer, (uint16_t)cbw->dCBWDataTransferLength, dev->address,
                                 &dev->endpoints[ENDPOINT_BULK_IN]));

    } else {
      CHECK(usb_data_out_transfer(send_receive_buffer, (uint16_t)cbw->dCBWDataTransferLength, dev->address,
                                  &dev->endpoints[ENDPOINT_BULK_OUT]));
    }
  }

  CHECK(usb_data_in_transfer((uint8_t *)&csw, sizeof(scsi_command_status_wrapper_t), dev->address,
                             &dev->endpoints[ENDPOINT_BULK_IN]));

  if (csw.bCSWStatus != 0 || csw.dCSWTag[0] != cbw->dCBWTag[0])
    result = USB_ERR_FAIL;
  else
    result = USB_ERR_OK;

done:
  critical_end();
  return result;
}

usb_error_t scsi_test(device_config_storage_t *const dev) {
  cbw_scsi_test_t cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;
  memset(&cbw_scsi.test, 0, sizeof(scsi_packet_test_t));

  cbw_scsi.cbw.bCBWCBLength           = sizeof(scsi_packet_test_t);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;

  return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
}

const scsi_packet_request_sense_t scsi_packet_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

usb_error_t scsi_request_sense(device_config_storage_t *const dev, scsi_sense_result_t *const sens_result) {
  cbw_scsi_request_sense_t cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.request_sense = scsi_packet_request_sense;

  cbw_scsi.cbw.bCBWCBLength           = sizeof(scsi_packet_request_sense_t);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_sense_result_t);

  return do_scsi_cmd(dev, &cbw_scsi.cbw, sens_result, false);
}

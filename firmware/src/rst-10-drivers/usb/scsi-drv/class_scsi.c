#include "class_scsi.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/usb_state.h"
#include <string.h>

_scsi_command_block_wrapper scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0}, 0, 0, 0, 0};

uint16_t next_tag = 0;

usb_error do_scsi_cmd(device_config_storage *const       dev,
                      _scsi_command_block_wrapper *const cbw,
                      void *const                        send_receive_buffer,
                      const bool                         send) {

  _scsi_command_status_wrapper csw = {{{0}}};

  cbw->dCBWTag[0] = next_tag++;

  if (!send)
    cbw->bmCBWFlags = 0x80;

  critical_begin();

  CHECK(usb_data_out_transfer((uint8_t *)cbw, sizeof(_scsi_command_block_wrapper) + 16, dev->address,
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

  CHECK(
      usb_data_in_transfer((uint8_t *)&csw, sizeof(_scsi_command_status_wrapper), dev->address, &dev->endpoints[ENDPOINT_BULK_IN]));

  if (csw.bCSWStatus != 0 && csw.dCSWTag[0] != cbw->dCBWTag[0])
    result = USB_ERR_FAIL;
  else
    result = USB_ERR_OK;

done:
  critical_end();
  return result;
}

usb_error scsi_test(device_config_storage *const dev) {
  cbw_scsi_test cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;
  memset(&cbw_scsi.test, 0, sizeof(_scsi_packet_test));

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_test);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;

  return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
}

usb_error scsi_request_sense(device_config_storage *const dev, scsi_sense_result *const sens_result) {
  _scsi_packet_request_sense scsi_packet_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};
  cbw_scsi_request_sense     cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.request_sense = scsi_packet_request_sense;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_request_sense);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_sense_result);

  return do_scsi_cmd(dev, &cbw_scsi.cbw, sens_result, false);
}

#include "usb_cbi.h"
#include "../base-drv/ch376.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/dev_transfers.h"
#include "../base-drv/protocol.h"

const setup_packet_t cbi2_adsc = {0x21, 0, {0, 0}, {255, 0}, 12}; // ;4th byte is interface number

// was no clear
usb_error_t usb_execute_cbi(device_config_t *const storage_device,
                            const uint8_t *const   cmd,
                            const bool             send,
                            const uint16_t         buffer_size,
                            uint8_t *const         buffer,
                            uint8_t *const         sense_codes) {

  uint8_t        result;
  setup_packet_t adsc;
  const uint8_t  interface_number = storage_device->interface_number;

  adsc           = cbi2_adsc;
  adsc.bIndex[0] = interface_number;

  critical_begin();

  result = usbdev_control_transfer(storage_device, &adsc, (uint8_t *const)cmd);

  if (result == USB_ERR_STALL) {
    if (sense_codes != NULL)
      usbdev_dat_in_trnsfer(storage_device, sense_codes, 2, ENDPOINT_INTERRUPT_IN);

    result = USB_ERR_STALL;
    goto done;
  }

  CHECK(result);

  if (send) {
    CHECK(usbdev_blk_out_trnsfer(storage_device, buffer, buffer_size));

  } else {
    CHECK(usbdev_dat_in_trnsfer(storage_device, buffer, buffer_size, ENDPOINT_BULK_IN));
  }

  if (sense_codes != NULL) {
    CHECK(usbdev_dat_in_trnsfer(storage_device, sense_codes, 2, ENDPOINT_INTERRUPT_IN));
  }

done:
  critical_end();

  return result;
}

#ifndef __SCSI_DRIVER__
#define __SCSI_DRIVER__

#include "../base-drv/ch376.h"
#include "class_scsi.h"
#include <stdint.h>

extern usb_error_t usb_scsi_init(const uint8_t dev_index);
extern usb_error_t usb_scsi_read_capacity(const uint8_t dev_index, scsi_read_capacity_result_t *result);
extern usb_error_t usb_scsi_read(const uint8_t dev_index, uint8_t *const buffer);
extern usb_error_t usb_scsi_write(const uint8_t dev_index, uint8_t *const buffer);
extern usb_error_t usb_scsi_seek(const uint8_t dev_index, const uint32_t lba);

#endif

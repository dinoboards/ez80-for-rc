#ifndef __USB_CBI_H__
#define __USB_CBI_H__

#include "../base-drv/ch376.h"
#include "../base-drv/dev_transfers.h"
#include <stdbool.h>
#include <stdint.h>

usb_error_t usb_execute_cbi(device_config_t *const storage_device,
                            const uint8_t *const   cmd,
                            const bool             send,
                            const uint16_t         buffer_size,
                            uint8_t *const         buffer,
                            uint8_t *const         asc);

#endif

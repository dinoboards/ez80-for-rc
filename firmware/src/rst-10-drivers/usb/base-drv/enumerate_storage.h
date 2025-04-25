#ifndef __USB_ENUMERATE_STORAGE
#define __USB_ENUMERATE_STORAGE

#include "dev_transfers.h"
#include "protocol.h"

extern void parse_endpoints(device_config_storage_t *const storage_dev, endpoint_descriptor_t const *pEndpoint);

#endif

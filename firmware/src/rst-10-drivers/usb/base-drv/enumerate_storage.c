#include "enumerate_storage.h"
#include "protocol.h"
#include <string.h>

void parse_endpoints(device_config_storage *const storage_dev, const endpoint_descriptor *pEndpoint) {
  uint8_t               x;
  endpoint_param       *ep;
  endpoint_param *const eps = storage_dev->endpoints;

  if (!(pEndpoint->bmAttributes & 0x02))
    return;

  x = calc_max_packet_sizex(pEndpoint->wMaxPacketSize);

  if (pEndpoint->bmAttributes & 0x01) { // 3 -> Interrupt
    if (!(pEndpoint->bEndpointAddress & 0x80))
      return;

    ep = &eps[ENDPOINT_INTERRUPT_IN];

  } else {
    ep = (pEndpoint->bEndpointAddress & 0x80) ? &eps[ENDPOINT_BULK_IN] : &eps[ENDPOINT_BULK_OUT];
  }

  ep->number           = pEndpoint->bEndpointAddress & 0x07;
  ep->toggle           = 0;
  ep->max_packet_sizex = x;
}

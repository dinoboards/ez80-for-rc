#include "../rst-28-vars.h"
#include <stdint.h>
#include <string.h>

uint8_t hbios_cio_install_uart0(uint8_t next_unit);

extern void *uart0_cio;

void hbios_cio_init() {
  uint8_t next_unit = 0;

  memset(hbios_vars->cio_drivers, 0, sizeof(hbios_vars->cio_drivers));

  next_unit = hbios_cio_install_uart0(next_unit);

  // report_hbios_drivers();
}

static const cio_fns_t cio_uart0_fns = {&uart0_cio, &uart0_cio, &uart0_cio, &uart0_cio, &uart0_cio, &uart0_cio, &uart0_cio};

uint8_t hbios_cio_install_uart0(uint8_t next_unit) {
  if (next_unit >= MAX_HBIOS_CIO_INSTANCES)
    return next_unit;

  hbios_vars->cio_drivers[next_unit].funcs    = &cio_uart0_fns;
  hbios_vars->cio_drivers[next_unit].instance = 1;

  return next_unit + 1;
}

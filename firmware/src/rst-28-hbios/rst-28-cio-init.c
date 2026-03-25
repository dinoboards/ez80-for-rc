#include "../rst-28-vars.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t hbios_cio_install_uart0(uint8_t next_unit);

extern void    *uart0_cio;
extern void    *uart0_ciodevice;
extern void    *uart0_cioquery;
extern uint24_t baud_rate;

void hbios_cio_init() {
  uint8_t next_unit = 0;

  memset(hbios_vars->cio_drivers, 0, sizeof(hbios_vars->cio_drivers));

  next_unit = hbios_cio_install_uart0(next_unit);

  hbios_vars->cio_count = next_unit;
}

static const cio_fns_t cio_uart0_fns = {&uart0_cio, &uart0_cio,      &uart0_cio,      &uart0_cio,
                                        &uart0_cio, &uart0_cioquery, &uart0_ciodevice};

uint8_t hbios_cio_install_uart0(uint8_t next_unit) {
  if (next_unit >= MAX_HBIOS_CIO_INSTANCES)
    return next_unit;

  hbios_vars->cio_drivers[next_unit].funcs    = &cio_uart0_fns;
  hbios_vars->cio_drivers[next_unit].instance = 1;

  printf("    UART0: MODE=%d,8,N,1\r\n", baud_rate);

  return next_unit + 1;
}

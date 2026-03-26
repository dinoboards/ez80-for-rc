#include "../rst-28-hbios/crt/vdu.h"
#include "../rst-28-vars.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <v99x8-super.h>

uint8_t hbios_cio_install_uart0(uint8_t next_unit);
uint8_t hbios_cio_install_super_hdmi(uint8_t next_unit);

extern void    *uart0_cioin;
extern void    *uart0_cioout;
extern void    *uart0_cioist;
extern void    *uart0_cioost;
extern void    *uart0_ciodevice;
extern void    *uart0_cioquery;
extern uint24_t baud_rate;

void hbios_cio_init() {
  uint8_t next_unit = 0;

  memset(hbios_vars->cio_drivers, 0, sizeof(hbios_vars->cio_drivers));

  next_unit = hbios_cio_install_super_hdmi(next_unit);
  next_unit = hbios_cio_install_uart0(next_unit);

  hbios_vars->cio_count = next_unit;
}

static const cio_fns_t cio_uart0_fns = {&uart0_cioin, &uart0_cioout,   &uart0_cioist,   &uart0_cioost,
                                        NULL,         &uart0_cioquery, &uart0_ciodevice};

uint8_t hbios_cio_install_uart0(uint8_t next_unit) {
  if (next_unit >= MAX_HBIOS_CIO_INSTANCES)
    return next_unit;

  hbios_vars->cio_drivers[next_unit].funcs    = &cio_uart0_fns;
  hbios_vars->cio_drivers[next_unit].instance = 1;

  printf("    UART0: MODE=%d,8,N,1\r\n", baud_rate);

  return next_unit + 1;
}

extern void *suphdmi_ciodevice;
extern void *suphdmi_cioout;

static const cio_fns_t cio_super_hdmi_fns = {&uart0_cioin, &suphdmi_cioout, &uart0_cioist,     &uart0_cioost,
                                             NULL,         &uart0_cioquery, &suphdmi_ciodevice};

uint8_t hbios_cio_install_super_hdmi(uint8_t next_unit) {
  if (next_unit >= MAX_HBIOS_CIO_INSTANCES)
    return next_unit;

  {
    uint8_t probe = vdp_init();

    if (probe != VDP_SUPER)
      return next_unit;

    vdu_init();
    vdu(22); // mode
    vdu(0);  // BBC Mode 0

    hbios_vars->cio_drivers[next_unit].funcs    = &cio_super_hdmi_fns;
    hbios_vars->cio_drivers[next_unit].instance = 1;
    hbios_vars->cio_count                       = next_unit;

    printf("SuperHDMI: IO=0x%02X..0x%02X 80x32\r\n", VDP_IO_DATA & 0xFF, VDP_IO_REGS & 0xFF);
  }

  return next_unit + 1;
}

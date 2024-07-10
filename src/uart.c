#include <eZ80.h>
#include <stdio.h>
#include "uart.h"
#include <uartdefs.h>
#include <defines.h>

extern long SysClkFreq;

void x_uart_init(void)
{
  unsigned short int i;
  unsigned short brg;
  brg = (SysClkFreq*1000)/(16 * UART_BPS);

  DI();

  PD_ALT2 = 0x03;
  PD_ALT1 = 0x00;
  PD_DDR = 0xEB;
  PD_DR = 0x00;

  UART_LCTL |= LCTL_DLAB; //enable register access
  UART_BRG_L = (brg & (unsigned short)0x00FF);
  UART_BRG_H = (brg & (unsigned short)0xFF00) >> (unsigned short)8;

  UART_LCTL &= ~LCTL_DLAB; //disable register access
  UART0_MCTL = 0x0;

  UART_FCTL = UART_FCTL_FIFOEN |UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4;

  // 0x07; // FIFO enabled, clear Tx and Rx FIFO, Receive Int at 1 in FIFO

// #define SetLCTL(d, p, s)     UART_LCTL = ((d-5)&3) | (((s-1)&1)<<2) | (p&3)
  SetLCTL(UART_DATA_BITS, UART_PARITY, UART_STOP_BITS);

  UART_IER = 1; // enable interrupts for receive?
  EI();

}


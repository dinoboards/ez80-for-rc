#include <eZ80.h>
#include <stdio.h>
#include "uart.h"

extern long SysClkFreq;

void x_uart_init(void)
{
  unsigned short int i;
  unsigned short brg;
  brg = (SysClkFreq*1000)/(16 * UART_BPS);

  PD_ALT2 = 0x03;
  PD_ALT1 = 0x00;
  PD_DDR = 0xEB;
  PD_DR = 0x00;

  UART_LCTL |= LCTL_DLAB;
  UART_BRG_L = (brg & (unsigned short)0x00FF);
  UART_BRG_H = (brg & (unsigned short)0xFF00) >> (unsigned short)8;
  UART_LCTL &= ~LCTL_DLAB;
  UART_FCTL = 0x07;	// eZ80F91 date codes 0611 and after requires disabling FIFO.
  SetLCTL(UART_DATA_BITS, UART_PARITY, UART_STOP_BITS);
}

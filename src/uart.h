#ifndef __UART
#define __UART

#define UART                 0
#define UART_BPS             115200
#define UART_DATA_BITS       8
#define UART_PARITY          0
#define UART_STOP_BITS       1

#define	UART_FCTL            UART0_FCTL
#define UART_RBR             UART0_RBR
#define UART_THR             UART0_THR
#define UART_BRG_L           UART0_BRG_L
#define UART_BRG_H           UART0_BRG_H
#define UART_LCTL            UART0_LCTL
#define UART_LSR             UART0_LSR
#define UART_IER             UART0_IER
#define UART_RBR             UART0_RBR
#define UART_IIR             UART0_IIR

#define LCTL_DLAB            (unsigned char)0x80
#define LSR_THRE             (unsigned char)0x20
#define LSR_DR               (unsigned char)0x01

#define SetLCTL(d, p, s)     UART_LCTL = ((d-5)&3) | (((s-1)&1)<<2) | (p&3)

extern void x_uart_init(void);

#endif

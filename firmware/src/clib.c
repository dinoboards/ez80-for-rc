#include <eZ80.h>
#include <uart.h>

#define LCTL_DLAB (unsigned char)0x80
#define LSR_THRE  (unsigned char)0x20
#define LSR_DR    (unsigned char)0x01

/*!
 * Receive a data byte from UART0.
 *
 * The getch() API reads a data byte from the default UART device. If there is no data in the UART device
 * the API blocks till it becomes available. When the FIFO is enabled this API returns the data byte
 * at the top of the FIFO.
 */
int getch(VOID) { return 0; }

/*!
 * Transmit a data byte on UART0.
 *
 * This function writes a data byte into the UARTx transmit buffer. In case of FIFO, the data byte
 * is put into the end of the FIFO.
 */
int putch(int ch) {
  while ((UART0_LSR & LSR_THRE) == (unsigned char)0)
    ;
  UART0_THR = ch;
  return (ch);
}

/*!
 * \brief Peek a character byte from default UART software transmit FIFO buffer.
 *
 * This function will simply peek into the receive FIFO buffer and returns
 * the same. That is, this function will return the next-out character present
 * in the software 'receive' FIFO buffer. However, the character peeked from
 * the FIFO, in this case, will not be read-off (cleared) from the FIFO. This
 * function returns UART_ERR_FAILURE if invoked in poll mode, or if
 * FIFO has an error in it. The function returns UART_ERR_RECEIVEFIFOEMPTY if receive FIFO buffer is empty. When it returns
 */
int peekc(VOID) { return 0; }

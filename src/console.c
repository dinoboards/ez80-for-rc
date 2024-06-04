#include <eZ80.h>
#include <stdio.h>
#include "uart.h"

#if defined(_ZSL_UART_USED)
#ifdef ZSL_DEVICE_UART1
/*** Note: As default, ZSL uses UART0 for functions: getch, putch and peekc.
 *         These functions were compiled with UART0 and were parts of ZSL library.
 *         When using UART1, it is required to overwrite functions getch, putch and
 *         peekc if they are used in this project.
 */

INT getch( VOID )
{
	CHAR ch ;
	UCHAR stat = UART_ERR_NONE ;
	UINT nbytes = 1 ;
	UART1_SPR = read_UART1( &ch, &nbytes ) ;
	stat = UART1_SPR ;
	nbytes = (UINT) ch ;
	nbytes = (nbytes & 0x0000FF) ;
	return (UART_ERR_NONE!=stat) ? EOF : nbytes ;

}//! end of

INT putch( INT ich )
{
    CHAR ch[ 2 ] ;
	CHAR cnt = 1 ;
	UCHAR stat = UART_ERR_NONE ;
	ch[ 0 ] = ich ;												//!< Copy the character byte to be transmitted.
	if( '\n' == ch[ 0 ] )										//!< See if this is a new line character.
	{
		ch[ 1 ] = '\r' ;										//!< Add a carriage-return to this character.
		cnt++ ;													//!< Accommodate this carriage-return character.
	}
	stat = write_UART1( ch, cnt ) ;								//!< Transmit this byte on UART1.
	return stat ;
}
#endif
#else
#if defined(_SIMULATE)
int putch(int ch)
{
	UART_THR = ch;
    return (ch);
}
#else
int putch(int ch)
{
    while ((UART_LSR & LSR_THRE) == (unsigned char)0);
    UART_THR = ch;
    return (ch);
}
#endif

int getch(void)
{
    while ((UART_LSR & LSR_DR) == (unsigned char)0);
    return (UART_RBR);
}
#endif

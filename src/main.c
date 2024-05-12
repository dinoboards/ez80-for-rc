/*****************************************************************************
 *  This program will exercise the eZ80Acclaim! UART.  The UART will be used
 *  to print a few messages and echo any character it receives from the
 *  Console port.  The target specific information is as follows:
 *
 *  Target
 *  ==========================================================================
 *    eZ80F91
 *    ----------------------------------------------
 *      Select Ethernet/Serial/USB in Settings->Debugger from Project menu.
 *      The program output will be sent to UART0.
 *
 *      UART Parameters (default):
 *        57600 bps, 8 bits per character, No parity, 1 stop bit, No flow ctrl
 *
 *    Simulator
 *    ----------------------------------------------
 *      Select Simulator in Settings->Debugger from Project menu. The program
 *      output will be displayed in the Debug tab on the output window.
 *
 *  Project Settings and Build Configurations
 *  ==========================================================================
 *    Debug
 *    ----------------------------------------------
 *      The settings in this configuration allow the program to run on
 *      the Simulator and on the development platform in RAM during a
 *      debug session.
 *
 *    Release
 *    ----------------------------------------------
 *      The settings in this configuration allow the program to reside
 *      and execute from internal Flash in the eZ80F91 on the mini module.
 *
 *  Example Output
 *  ==========================================================================
 *    ZiLOG Developers Studio
 *    i = 5
 *    d = 25
 *    f = 1.260000
 *    eZ80F91 5 25 1.260000
 *
 *   Change History:
 *   12/02/11 - Fixed UART when ZSL is selected
 *
 ****************************************************************************/
#include <eZ80.h>
#include <stdio.h>

#define UART                 0			// change to 1 when using UART 1
//#define UART_BPS             57600
#define UART_BPS             19200
#define UART_DATA_BITS       8
#define UART_PARITY          0
#define UART_STOP_BITS       1

#define DEVICE_NAME          "eZ80F92"

#if !defined(_ZSL_UART_USED)
#if (UART==1)
#define	UART_FCTL	     	 UART1_FCTL
#define UART_RBR             UART1_RBR
#define UART_THR             UART1_THR
#define UART_BRG_L           UART1_BRG_L
#define UART_BRG_H           UART1_BRG_H
#define UART_LCTL            UART1_LCTL
#define UART_LSR             UART1_LSR
#else
#define	UART_FCTL	     	 UART0_FCTL
#define UART_RBR             UART0_RBR
#define UART_THR             UART0_THR
#define UART_BRG_L           UART0_BRG_L
#define UART_BRG_H           UART0_BRG_H
#define UART_LCTL            UART0_LCTL
#define UART_LSR             UART0_LSR
#endif

#define LCTL_DLAB            (unsigned char)0x80
#define LSR_THRE             (unsigned char)0x20
#define LSR_DR               (unsigned char)0x01

#define SetLCTL(d, p, s)     UART_LCTL = ((d-5)&3) | (((s-1)&1)<<2) | (p&3)
#endif //!defined(_ZSL_UART_USED)

#define LF                   '\n'
#define CR                   '\r'

extern long SysClkFreq;
char device_name[] = DEVICE_NAME;

void uart_init(void)
{
#ifndef _SIMULATE
 #ifdef _ZSL_UART_USED
	ei();
   #ifdef ZSL_DEVICE_UART0
	//enable Max3222 driver
    PB_DR = 0x40;
    PB_ALT2 = 0x00;
    PB_ALT1 = 0x00;
    PB_DDR = 0xBF;
   #endif
 #else
    unsigned short int i;
    unsigned short brg;
    brg = SysClkFreq/(16 * UART_BPS);

  #if (UART==1)
    PC_ALT2 = 0x03;
    PC_ALT1 = 0x00;
    PC_DDR = 0xEB;
    PC_DR = 0x00;
  #else
    PD_ALT2 = 0x03;
    PD_ALT1 = 0x00;
    PD_DDR = 0xEB;
    PD_DR = 0x00;

	//enable Max3222 driver
    PB_DR = 0x40;
    PB_ALT2 = 0x00;
    PB_ALT1 = 0x00;
    PB_DDR = 0xBF;
  #endif

    UART_LCTL |= LCTL_DLAB;
    UART_BRG_L = (brg & (unsigned short)0x00FF);
    UART_BRG_H = (brg & (unsigned short)0xFF00) >> (unsigned short)8;
    UART_LCTL &= ~LCTL_DLAB;
    UART_FCTL = 0x07;	// eZ80F91 date codes 0611 and after requires disabling FIFO.
    SetLCTL(UART_DATA_BITS, UART_PARITY, UART_STOP_BITS);
 #endif
#endif
}

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
    if (ch == LF)
    {
        while ((UART_LSR & LSR_THRE) == (unsigned char)0);
        UART_THR = CR;
    }
    return (ch);
}
#endif

int getch(void)
{
    while ((UART_LSR & LSR_DR) == (unsigned char)0);
    return (UART_RBR);
}
#endif

void test(void)
{
    int a = 1;
    int b = 2;
    int c = 3;
    c = b + a + 2;
    c++;
    c++;
}


#define WS2812_CMD		(*(XSFR)0xFF30)
#define WS2812_DATA		(*(XSFR)0xFF31)

void ws2812b(void) {
  WS2812_CMD =4; //set count
  WS2812_DATA = 170; //170 pixels

  WS2812_CMD = 2; //select pixel
  WS2812_DATA = 0; //pixel 0

  WS2812_CMD = 3; //set color
  WS2812_DATA = 255; //red
  WS2812_DATA = 0; //green
  WS2812_DATA = 0; //blue
}

int main()
{
  unsigned int x = 0;
  int f = 0;
    int ch;
    int i = 0;
    static char zds[] = "ZiLOG Developers Studio";

    uart_init();
    test();

    ws2812b();

    //printf("\nUsing Uart %i\n", UART);
    //printf("-----------------------\n");
    //printf("%s\n", zds);
    //printf("-----------------------\n");
    //printf("i = %i\n", 5);
    //printf("d = %d\n", 25);
    //printf("f = %f\n", 1.26);
    //printf("%s %i %d %f\n", device_name, 5, 25, 1.26);
    //printf("-----------------------\n");

  // set a pin as an output??

//PB_DR
 PB_DDR = 0; // should be init to output (0)
// PB_ALT1
// PB_ALT2

    while (1)
    {
      if (x++ > 256)
      {
        x = 0;
        f = !f;
        PB_DR = f ? 1 : 0;
        putchar( 255 ) ;

      }

      //putchar( 'B' ) ;
      //putchar( 'C' ) ;

    }
    return 0;
}

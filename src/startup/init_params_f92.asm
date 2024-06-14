;*****************************************************************************
; init_params_f92.asm
;
; minimum eZ80F92 initialization
;*****************************************************************************
; Copyright (C) 2005 by ZiLOG, Inc.  All Rights Reserved.
;*****************************************************************************

        INCLUDE "ez80f92.inc"
        INCLUDE "..\config.inc"


	XREF	__stack
	XREF	__init_default_vectors
	XREF	__c_startup
	XREF	__cstartup
	XREF	_main

	XREF	__FLASH_CTL_INIT_PARAM
	XREF	__FLASH_ADDR_U_INIT_PARAM
	XREF	__RAM_CTL_INIT_PARAM
	XREF	__RAM_ADDR_U_INIT_PARAM

        XDEF	__init
        XDEF	_abort
        XDEF	__exit
        XDEF	_exit


;*****************************************************************************
; Startup code
        DEFINE	.STARTUP, SPACE = ROM
        SEGMENT	.STARTUP
        .ASSUME	ADL = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Minimum default initialization
__init:
	; disable internal peripheral interrupt sources
	;-- this will help during a RAM debug session --
	ld	a, %FF
	out0	(PB_DDR), a			; GPIO
	out0	(PC_DDR), a			;
	out0	(PD_DDR), a			;
	xor	a
	out0	(PB_ALT1), a			;
	out0	(PC_ALT1), a			;
	out0	(PD_ALT1), a			;
	out0	(PB_ALT2), a			;
	out0	(PC_ALT2), a			;
	out0	(PD_ALT2), a			;
	out0	(TMR0_CTL), a			; timers
	out0	(TMR1_CTL), a			;
	out0	(TMR2_CTL), a			;
	out0	(TMR3_CTL), a			;
	out0	(TMR4_CTL), a			;
	out0	(UART0_IER), a			; UARTs
	out0	(UART1_IER), a			;
	out0	(I2C_CTL), a			; I2C
	out0	(FLASH_IRQ), a			; Flash
	ld	a, %04
	out0	(SPI_CTL), a			; SPI
	in0	a, (RTC_CTRL)			; RTC, Writing to the RTC_CTRL register also
	and	a, %BE				; resets the RTC count prescaler allowing
	out0	(RTC_CTRL), a			; the RTC to be synchronized to another
                             			; time source.

	; set PB5 for RC2014 clock out
	; out frequency = 18.432Mhz / (DIV * (RR*2))
	;               = 18.432Mhz / (4 * 1 * 2)
	;               =  2.304Mhz
	;
	;               = 20Mhz / 8 = 2.5Mhz
	;               = 24MHz / 8 = 3Mhz

	; divide cpu clock by 4
	ld	a, 1
	out0	(TMR5_RR_L), a
	xor	a
	out0	(TMR5_RR_H), a
	LD	A, TMR_ENABLED | TMR_CONTINUOUS | TMR_RST_EN | TMR_CLK_DIV_4
	out0	(TMR5_CTL), a

	; SEND TIMER 5 TO PB5
	ld	a, 1 << 5 ; Px5 ALT
	out0	(PB_ALT2), a
	out0	(PB_DDR), a
	xor	a
	out0	(PB_ALT1), a
	out0	(PB_DR), a

	; CS0 and CS1 are disabled
	xor	a
	out0	(CS0_LBR), a
	out0	(CS0_UBR), a
	out0	(CS0_BMC), a
	out0	(CS0_CTL), a

	out0	(CS1_LBR), a
	out0	(CS1_UBR), a
	out0	(CS1_BMC), a
	out0	(CS1_CTL), a

	; CS2 is enabled for I/O @ $FFxx
	ld	a, %FF
	out0	(CS2_LBR), a
	out0	(CS2_UBR), a
	ld	a, BMX_BM_Z80 | BMX_AD_SEPERATE | IO_BUS_CYCLES
	out0	(CS2_BMC), a
	ld	a, CSX_WAIT_0 | CSX_TYPE_IO | CSX_ENABLED
	out0	(CS2_CTL), a

	; CS3 is enabled for memory @ $B9xxxx
	ld	a, %B9
	out0	(CS3_LBR), a
	out0	(CS3_UBR), a
	ld	a, BMX_BM_Z80 | BMX_AD_SEPERATE | MEM_BUS_CYCLES
	out0	(CS3_BMC), a
	ld	a, CSX_WAIT_0 | CSX_TYPE_MEM | CSX_ENABLED
	out0	(CS3_CTL), a

	; enable internal memory
	ld	a, __FLASH_ADDR_U_INIT_PARAM
	out0	(FLASH_ADDR_U), a
	ld	a, __FLASH_CTL_INIT_PARAM
	out0	(FLASH_CTRL), a

	ld	a, __RAM_ADDR_U_INIT_PARAM
	out0	(RAM_ADDR_U), a
	ld	a, __RAM_CTL_INIT_PARAM
	out0	(RAM_CTL), a

	; setup Stack Pointer
	ld	sp, __stack

	; in itialize the interrupt vector table
	call	__init_default_vectors

	; start application
	ld	a, __cstartup
	or	a, a
	jr	z, __no_cstartup
	call	__c_startup

__no_cstartup:
	;--------------------------------------------------
	; Initialize the peripheral devices

	XREF __open_periphdevice

	call	__open_periphdevice

	call	_main				; int main(int argc, char *argv[]))

__exit:
_exit:
_abort:
	;--------------------------------------------------
	; Close the peripheral devices

	XREF	__close_periphdevice

	call	__close_periphdevice

	jr	$				; if we return from main loop forever here


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_SysClkFreq:
	DL	CPU_CLK_FREQ/1000

	XDEF	_SysClkFreq

	END

;*****************************************************************************
; init_params_f92.asm
;
; minimum eZ80F92 initialization
;*****************************************************************************
; Copyright (C) 2005 by ZiLOG, Inc.  All Rights Reserved.
;*****************************************************************************

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
	XREF	__vector_table

	XREF	_system_sub_second_count

	XREF	_cs2_bus_cycles
	XREF	_cs3_bus_cycles

	XREF	FIRMWARE_RAM_RESERVED

;*****************************************************************************
; Startup code
        DEFINE	.STARTUP, SPACE = ROM
        SEGMENT	.STARTUP
        .ASSUME	ADL = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Minimum default initialization
__init:
	IM	2				; Interrtup mode 2
	LD	HL, __vector_table
	LD	A, __vector_table >> 8 & 0ffh
	LD	I, A				; Load interrtup vector base

	LD	A, %FF
	OUT0	(PB_DDR), A			; GPIO
	OUT0	(PC_DDR), A			;
	OUT0	(PD_DDR), A			;
	XOR	A
	OUT0	(PB_ALT1), A			;
	OUT0	(PC_ALT1), A			;
	OUT0	(PD_ALT1), A			;
	OUT0	(PB_ALT2), A			;
	OUT0	(PC_ALT2), A			;
	OUT0	(PD_ALT2), A			;
	OUT0	(TMR0_CTL), A			; timers
	OUT0	(TMR1_CTL), A			;
	OUT0	(TMR2_CTL), A			;
	OUT0	(TMR3_CTL), A			;
	OUT0	(TMR4_CTL), A			;
	OUT0	(UART0_IER), A			; UARTs
	OUT0	(UART1_IER), A			;
	OUT0	(I2C_CTL), A			; I2C
	OUT0	(FLASH_IRQ), A			; Flash
	LD	A, %04
	OUT0	(SPI_CTL), A			; SPI
	IN0	A, (RTC_CTRL)			; RTC, Writing to the RTC_CTRL register also
	AND	A, %BE				; resets the RTC count prescaler allowing
	OUT0	(RTC_CTRL), A			; the RTC to be synchronized to another
                             			; time source.

	; set PB5 for RC2014 clock out
	; out frequency = 18.432Mhz / (DIV * (RR*2))
	;               = 18.432Mhz / (4 * 1 * 2)
	;               =  2.304Mhz
	;
	;               = 20Mhz / 8 = 2.5Mhz
	;               = 24MHz / 8 = 3Mhz

	; divide cpu clock by 4
	LD	A, 1
	OUT0	(TMR5_RR_L), A
	XOR	A
	OUT0	(TMR5_RR_H), A
	LD	A, TMR_ENABLED | TMR_CONTINUOUS | TMR_RST_EN | TMR_CLK_DIV_4
	OUT0	(TMR5_CTL), A

	; CONFIGURE PB4 & PB5
	; SEND TIMER 5 TO PB5
	; GPIO MODE 7, ALT1 0, ALT2 1, DDR 1, DR 0
	;
	; SET PB4 (INT) TO INPUT & ENABLE INTERRUPT VECTOR
	; GPIO MODE 8, ALT1 1, ALT2 1, DDR 0, DR 0
	LD	A, P4_ALT1
	OUT0	(PB_ALT1), A

	LD	A, P5_ALT2 | P4_ALT2
	OUT0	(PB_ALT2), A

	LD	A, P5_DDR
	OUT0	(PB_DDR), A

	XOR	A
	OUT0	(PB_DR), A

	; CS0 and CS1 are disabled
	XOR	A
	OUT0	(CS0_LBR), A
	OUT0	(CS0_UBR), A
	OUT0	(CS0_BMC), A
	OUT0	(CS0_CTL), A

	OUT0	(CS1_LBR), A
	OUT0	(CS1_UBR), A
	OUT0	(CS1_BMC), A
	OUT0	(CS1_CTL), A

	; CS2 is enabled for I/O @ $FFxx
	LD	A, %FF
	OUT0	(CS2_LBR), A
	OUT0	(CS2_UBR), A

	LD	A, IO_BUS_CYCLES
	AND	%0F
	LD	(_cs2_bus_cycles), A
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS2_BMC), A
	LD	A, CSX_TYPE_IO | CSX_ENABLED
	OUT0	(CS2_CTL), A

	; CS3 is enabled for memory @ $03xxxx
	LD	A, Z80_ADDR_MBASE
	OUT0	(CS3_LBR), A
	OUT0	(CS3_UBR), A
	LD	A, MEM_BUS_CYCLES
	AND	%0F
	LD	(_cs3_bus_cycles), A
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS3_BMC), A
	LD	A, CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS3_CTL), A

	; enable internal memory
	LD	A, __FLASH_ADDR_U_INIT_PARAM
	OUT0	(FLASH_ADDR_U), A
	LD	A, __FLASH_CTL_INIT_PARAM
	OUT0	(FLASH_CTRL), A

	LD	A, __RAM_ADDR_U_INIT_PARAM
	OUT0	(RAM_ADDR_U), A
	LD	A, __RAM_CTL_INIT_PARAM
	OUT0	(RAM_CTL), A

	ld	sp, FIRMWARE_RAM_RESERVED

	JP	_main

	END

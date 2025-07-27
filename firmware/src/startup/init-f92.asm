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

	XREF	_mem_bus_mode_and_timing
	XREF	_mem0_bus_mode_and_timing
	XREF	_io_bus_mode_and_timing

	XREF	SPL_BASE

INIT_IO_BUS_CYCLES 		.EQU	BMX_BC_5
INIT_MEM_BUS_CYCLES 		.EQU	BMX_BC_2

;*****************************************************************************
; Startup code
        DEFINE	.STARTUP, SPACE = ROM
        SEGMENT	.STARTUP
        .ASSUME	ADL = 1

	xref	_reg_iy
	xref	_reg_spl
	xref	z80_flags
	xref	z80_invoke_iy
	xref	z80_regs
	xref	z80_set_int_state

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Minimum default initialization
__init:
	IM	2				; Interrupt mode 2 - the only valid mode for eZ80

	; determine if we are invoked due to illegal instruction
	; or due to power/reset
	; if mbase is z - then assume power on.

	; save register states into emulator storage (assumes we are about to switch to emulator)

	ld	(_reg_spl), sp
	ld	sp, _reg_iy+3

	; put alt registers into mem store
	; and place normal registers into alt
	exx
	push	iy
	push	ix
	push	hl		; regs' goes to store, regs goes to regs'
	push	de
	push	bc
	ex	af, af'
	push	af		; af' goes to store, af goes to af'

	ld	sp, (_reg_spl)
	inc	sp		; skip the trap return address
	inc	sp
	inc	sp

	ld	a, mb
	or	a
	jr	z, power_on_reset

illegal_instruction:
	; test to see if illegal instruction is $CB $30

	ld	iy, (_reg_spl)

	ld	a, (iy)
	cp	%02			; called from ADL=0 mode?
	jr	nz, power_on_reset	; no - so treat as normal illegal instruction and reboot

	ld	ix, (iy+1)		; load address of byte after illegal instruction byte

	ld.s	a, (ix-1)		; should be $CB
	cp	%CB
	jr	nz, power_on_reset
	ld.s	a, (ix)			; should be $30
	cp	%30
	jr	nz, power_on_reset

	; we have identified illegal instruction byte codes are:
	; $CB $30 - switch to emulator
	; we want iy to equal ix+1

	lea	iy, ix+1
	ld	ix, z80_regs
	ld	(ix+z80_flags), 0
	call	z80_set_int_state

	jp	z80_invoke_iy


power_on_reset:
	LD	HL, __vector_table
	LD	A, __vector_table >> 8 & 0ffh
	LD	I, A				; Load interrupt vector base

	LD	A, %FF
	OUT0	(PB_DDR), A			; GPIO
	OUT0	(PD_DDR), A			;

	LD	A, %FE				; SET PC0 as output, rest of PCs pins as input
	OUT0	(PC_DDR), A			;
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

	; SET ON BOARD LED TO TURN ON
	LD	A, 0
	OUT0	(PC_DR), A

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
	LD	A, IO_SEGMENT
	OUT0	(CS2_LBR), A
	OUT0	(CS2_UBR), A
	LD	A, INIT_IO_BUS_CYCLES & %0F | BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS2_BMC), A
	LD	A, CSX_TYPE_IO | CSX_ENABLED
	OUT0	(CS2_CTL), A

	; CS3 is enabled for memory @ $03xxxx
	LD	A, Z80_ADDR_MBASE
	ld	mb, a
	OUT0	(CS3_LBR), A
	OUT0	(CS3_UBR), A
	LD	A, INIT_MEM_BUS_CYCLES & %0F | BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS3_BMC), A
	LD	A, CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS3_CTL), A

	; CS0 is enabled for memory @ $200000 -> $3FFFFF
	LD	A, %20
	OUT0	(CS0_LBR), A
	LD	A, %3F
	OUT0	(CS0_UBR), A
	LD	A, INIT_MEM_BUS_CYCLES & %0F | BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS0_BMC), A
	LD	A, CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS0_CTL), A

	; CS1 is enabled for memory @ $400000 -> $5FFFFF
	LD	A, %40
	OUT0	(CS1_LBR), A
	LD	A, %5F
	OUT0	(CS1_UBR), A
	LD	A, INIT_MEM_BUS_CYCLES & %0F | BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS1_BMC), A
	LD	A, CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS1_CTL), A

	; enable internal memory
	LD	A, __FLASH_ADDR_U_INIT_PARAM
	OUT0	(FLASH_ADDR_U), A
	LD	A, __FLASH_CTL_INIT_PARAM
	OUT0	(FLASH_CTRL), A

	LD	A, __RAM_ADDR_U_INIT_PARAM
	OUT0	(RAM_ADDR_U), A
	LD	A, __RAM_CTL_INIT_PARAM
	OUT0	(RAM_CTL), A

	ld	sp, SPL_BASE

	JP	_main

	END

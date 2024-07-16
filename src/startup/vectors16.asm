;*****************************************************************************
; vectors16.asm
;
; eZ80's Reset, RST and first generation interrupt vector arrangement
;*****************************************************************************
; Copyright (C) 2005 by ZiLOG, Inc.  All Rights Reserved.
;*****************************************************************************

	XREF	__init
	XREF	__low_rom

	XDEF	_reset

;*****************************************************************************
; Reset and all RST nn's
;  1. diaable interrupts
;  2. clear mixed memory mode (MADL) flag
;  3. jump to initialization procedure with jp.lil to set ADL
	DEFINE	.RESET, SPACE = ROM
	SEGMENT	.RESET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	XREF	_rst_io
	XREF	_rst_rc2014_fnc
	XREF	_rst_rc2014_bank_switch
_reset:
_rst0:
	di
	stmix
	jp.lil	__init

	ORG	%08
_rst8:
	jp.lil	_rst_io

	org	%10
_rst10:
	jp.lil	_rst_rc2014_fnc

	org	%18
_rst18:
	jp.lil	_rst_rc2014_bank_switch

	org	%20
_rst20:
	di
	stmix
	jp.lil	__init
_rst28:
	di
	stmix
	jp.lil	__init
_rst30:
	di
	stmix
	jp.lil	__init
_rst38:
	di
	stmix
	jp.lil	__init
	ds %26
_nmi:
	jp.lil	__default_nmi_handler


;*****************************************************************************
; Startup code
	DEFINE .STARTUP, SPACE = ROM
	SEGMENT .STARTUP
	.ASSUME ADL=1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Default Non-Maskable Interrupt handler
__default_nmi_handler:
	RETN.L

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Default Maskable Interrupt handler
__default_mi_handler:
	EI
	RETI.L


delegate_isr:	; defer to the external ISR routine
	PUSH	HL
	LD	HL, 3
	ADD	HL, SP
	PUSH	AF

	LD	A, (HL)			; ADL MODE OF INTERRUPTED CODE (2 - Z80, 3 - ADL)
	CP	3
	JR	NZ, skip_24_reg_save

	PUSH	BC			; IF WE INTERRUPTED AN ADL ROUTINE,
	PUSH	DE			; WE NEED TO SAVE ALL 24-BIT REGISTERS
	PUSH	HL			; BEFORE JUMPING INTO Z80 CODE,
	PUSH	IX			; BECAUSE DESPITE IT ONLY HAVE ACCESS TO THE
	PUSH	IY			; LOWER 16 BIT OF REGISTERS, THE VARIOUS INSTRUCTIONS
	EXX				; CAN HAVE SIDE EFFECTS ON THE UPPER 8 BITS
	PUSH	BC			; AF REGISTER PAIR DOES NOT HAVE AN UPPER 8 BITS TO BE
	PUSH	DE			; IMPACTED, SO WE CAN SKIP IT
	PUSH	HL
	EXX

skip_24_reg_save:
	RST.S	%38		; delegate to ISR routine of external ROM

	JR	NZ, skip_24_reg_restore
	EXX
	POP	HL
	POP	DE
	POP	BC
	EXX
	POP	IY
	POP	IX
	POP	HL
	POP	DE
	POP	BC

skip_24_reg_restore:
	POP	AF
	POP	HL
	EI
	RETI.L			; return rom interrupt - back to ADL 0 or 1

;*****************************************************************************
; Interrupt Vector Table
;  - this segment must be aligned on a 256 byte boundary anywhere below
;    the 64K byte boundry
;  - each 2-byte entry is a 2-byte vector address
	DEFINE .IVECTS, SPACE = ROM, ALIGN = 100h
	SEGMENT .IVECTS

	PUBLIC	__vector_table
	EXTERN	_ms_60Hz_timer_counter_isr
	EXTERN _uart0_receive_isr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__vector_table:
	dw	__default_mi_handler		; 00H - unused
	dw	__default_mi_handler		; 02H - unused
	dw	__default_mi_handler		; 04H - unused
	dw	__default_mi_handler		; 06H - unused
	dw	__default_mi_handler		; 08H - Flash
	dw	__default_mi_handler		; 0AH - PRT 0
	dw	_ms_60Hz_timer_counter_isr	; 0CH - PRT 1
	dw	__default_mi_handler		; 0EH - PRT 2
	dw	__default_mi_handler		; 10H - PRT 3
	dw	__default_mi_handler		; 12H - PRT 4
	dw	__default_mi_handler		; 14H - PRT 5
	dw	__default_mi_handler		; 16H - RTC
	dw	_uart0_receive_isr		; 18H - UART 0
	dw	__default_mi_handler		; 1AH - UART 1
	dw	__default_mi_handler		; 1CH - I2C
	dw	__default_mi_handler		; 1EH - SPI
	dw	__default_mi_handler		; 20H - unused
	dw	__default_mi_handler		; 22H - unused
	dw	__default_mi_handler		; 24H - unused
	dw	__default_mi_handler		; 26H - unused
	dw	__default_mi_handler		; 28H - unused
	dw	__default_mi_handler		; 2AH - unused
	dw	__default_mi_handler		; 2CH - unused
	dw	__default_mi_handler		; 2EH - unused
	dw	__default_mi_handler		; 30H - Port B0
	dw	__default_mi_handler		; 32H - Port B1
	dw	__default_mi_handler		; 34H - Port B2
	dw	__default_mi_handler		; 36H - Port B3
	dw	delegate_isr			; 38H - Port B4
	dw	__default_mi_handler		; 3AH - Port B5
	dw	__default_mi_handler		; 3CH - Port B6
	dw	__default_mi_handler		; 3EH - Port B7
	dw	__default_mi_handler		; 40H - Port C0
	dw	__default_mi_handler		; 42H - Port C1
	dw	__default_mi_handler		; 44H - Port C2
	dw	__default_mi_handler		; 46H - Port C3
	dw	__default_mi_handler		; 48H - Port C4
	dw	__default_mi_handler		; 4AH - Port C5
	dw	__default_mi_handler		; 4CH - Port C6
	dw	__default_mi_handler		; 4EH - Port C7
	dw	__default_mi_handler		; 50H - Port D0
	dw	__default_mi_handler		; 52H - Port D1
	dw	__default_mi_handler		; 54H - Port D2
	dw	__default_mi_handler		; 56H - Port D3
	dw	__default_mi_handler		; 58H - Port D4
	dw	__default_mi_handler		; 5AH - Port D5
	dw	__default_mi_handler		; 5CH - Port D6
	dw	__default_mi_handler		; 5EH - Port D7

	END

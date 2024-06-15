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
	retn

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Default Maskable Interrupt handler
__default_mi_handler:
	ei
	reti

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__vector_table:
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	delegate_isr
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler
	dw	__default_mi_handler

	END

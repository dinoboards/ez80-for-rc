	include ".\config.inc"


	DEFINE	INTERNAL_HIRAM_ROM, SPACE = ROM
	SEGMENT	INTERNAL_HIRAM_ROM

	DEFINE	INTERNAL_HIRAM_RAM, SPACE = RAM
	; ORG=%02FF00
	SEGMENT	INTERNAL_HIRAM_RAM

	.assume adl=1

	SEGMENT	INTERNAL_HIRAM_ROM

	PUBLIC	FIRMWARE_RAM_RESERVED
FIRMWARE_RAM_RESERVED:
_firmware_ram_reseved:

	XREF	_rst_08_functions
	PUBLIC	firmware_rst_08_hook
firmware_rst_08_hook:
	JP	_rst_08_functions

	XREF	_rst_10_functions
	PUBLIC	firmware_rst_10_hook
firmware_rst_10_hook:
	JP	_rst_10_functions

	XREF	_system_timer_isr
	PUBLIC	_system_timer_isr_hook
_system_timer_isr_hook:
	JP	_system_timer_isr

	XREF	z80_emulator_isr
	PUBLIC	_marshall_isr_hook
_marshall_isr_hook:
	JP	z80_emulator_isr	; changes to _marshall_isr if version exchanged is invoked

	XREF	_uart0_receive_isr
	PUBLIC	_uart0_receive_isr_hook
_uart0_receive_isr_hook:
	JP	_uart0_receive_isr

	xref	_default_mi_handler
	PUBLIC	_default_mi_handler_hook
_default_mi_handler_hook:
	JP	_default_mi_handler

	.assume	adl=1

; ez80's alt version of BC, DE, HL contain the emulated z80's main registers
; the ez80's alt registers are stored in reg_aXX below:


	global	_reg_ade
	global	_reg_ahl
	global	_reg_ix
	global	_reg_iy
	global	_reg_spl
	global	_z80_flags
	global	z80_reg_aaf
	global	z80_reg_abc
	global	z80_reg_ade
	global	z80_reg_ahl
	global	z80_reg_ix
	global	z80_reg_iy
	global	z80_regs
	global	z80_flags

	DEFINE	INTERNAL_VARS_RAM, SPACE = RAM
	; ORG=%02FFE0
	SEGMENT	INTERNAL_VARS_RAM
_INTERNAL_VARS_RAM	equ	%02FFE0
; registers
z80_regs:
_reg_aaf:	ds	3
z80_reg_aaf	equ	0

_reg_abc:	ds	3
z80_reg_abc	equ	3

_reg_ade:	ds	3
z80_reg_ade	equ	6

_reg_ahl:	ds	3
z80_reg_ahl	equ	9

_reg_ix:	ds	3
z80_reg_ix	equ	12

_reg_iy:	ds	3
z80_reg_iy	equ	15

_reg_spl:	ds	3
z80_reg_spl	equ	18

_z80_flags:	db 	1		; bit 1 on -> DI, off -> EI, bit 0 on -> int-pending
z80_flags	equ	21		; true if maskable ints enabled

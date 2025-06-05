	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

; ez80's alt version of BC, DE, HL contain the emulated z80's main registers
; the ez80's alt registers are stored in reg_aXX below:

	section	BSS

	global	_reg_ade
	global	_reg_ahl
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


	include	"..\..\..\config.inc"
	include	"..\usb-constants.inc"

	section	CODE

	.assume	adl=1

	global	_ch_command
	XREF	_delay

_ch_command:
	ld	b, %ff

l_ch_command_00102:
	ld	a, +(_CH376_COMMAND_PORT/256)
	in	a, (_CH376_COMMAND_PORT&%FF)
	bit	4, a
	jr	nz, l_ch_command_00104
l_ch_command_00103:
	ld	iy, 0
	add	iy, sp
	ld	a, (iy+3)
	ld	bc, _CH376_COMMAND_PORT
	out	(bc), a
	ret

l_ch_command_00104:
	djnz	l_ch_command_00102
	jr	l_ch_command_00103

	public	_di_and_save
; uint24_t di_and_save();
_di_and_save:
	ld	a, i
	push	af
	pop	hl
	ret

	public	_restore_ei
; void restore_ei(uint24_t s)
_restore_ei:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy+3)
	push	hl
	pop	af
	jp	po, .skip
	ei
.skip
	ret

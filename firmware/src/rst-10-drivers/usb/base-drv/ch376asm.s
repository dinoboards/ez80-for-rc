
        INCLUDE "..\..\..\config.inc"
	INCLUDE "..\usb-constants.inc"

	SECTION CODE

	.assume adl=1

	GLOBAL	_ch_command

_ch_command:
	ld	iy, 0
	add	iy, sp
	ld	b, %ff

l_ch_command_00102:
	ld	a, +(_CH376_COMMAND_PORT / 256)
	in	a, (_CH376_COMMAND_PORT & %FF)
	bit	4, a
	jr	Z,l_ch_command_00104
	djnz	l_ch_command_00102

l_ch_command_00104:
	ld	a,(iy+3)
	ld	bc,_CH376_COMMAND_PORT
	out	(bc), a
	ret

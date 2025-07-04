	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

	xref	_z80_flags
	xref	ix_instr_table
	xref	iy_instr_table
	xref	z80_flags
	xref	z80_instr_misc_table
	xref	z80_instr_table
	xref	z80_regs
	xref	z80_int_request2

	section	INTERNAL_RAM_ROM

z80_int_request:
	ld	a, %02
	ld	(ix+z80_flags), a
	push.s	iy
z80_int_request2:
	ld	iy, %0038
	jr	z80_loop2

z80_loop:
	ld	a, (ix+z80_flags)
	bit	1, a			; ei enabled?
	jr	nz, z80_loop2

	bit	0, a			; int pending?
	jr	nz, z80_int_request

	global	z80_loop2
z80_loop2:
	; bit	2, a
	; jr	nz, z80_special

z80_loop3:
	z80_byte_jump	z80_instr_table

	; $CB ....
	global	cb_bit_instr
	global	z80_bit
	xref	z80_switch_to_native
z80_bit:
	ld.s	a, (iy)
	inc	iy
	cp	%31		;
	jp	z, z80_switch_to_native
	ld	(cb_bit_instr), a
	exx
	ex	af, af'
; 	jp	z80_bit2
; z80_bit2:
	db	%52	; .S prefix
	db	%CB	; bit operand
cb_bit_instr:
	db	%00
	ex	af, af'
	exx
	z80loop


	; $CB 31 switch to native
	global	z80_switch_to_native2
	global	switch_addr
	xref	_marshall_isr_hook
z80_switch_to_native2:
	restore_ei_s		; todo could switch_addr be corrupted by an int handler?

switch_addr	equ	$+2
	jp.s	%0000

	global	z80_callilmmn2
	global	z80_callilmmn_addr
z80_callilmmn2:
z80_callilmmn_addr	equ	$+2
	call.il	0
	ret


; MISC ED Instructions
	; $ED
	global	z80_misc
z80_misc:
	z80_byte_jump	z80_instr_misc_table


	; DD ....
	global	z80_ix
z80_ix:
	z80_byte_jump	ix_instr_table

	global	z80_ixbits2
	global	dd_bit_instr
z80_ixbits2:
	db	%52
	db	%DD	; bit operand
	db	%CB	; bit operand
dd_bit_instr:
	db	%00
	db	%06
	ex	af, af'
	exx
	ld	ix, z80_regs
	z80loop

	; FD ....
	global	z80_iy
z80_iy:
	z80_byte_jump	iy_instr_table

	global	z80_iybits2
	global	fd_bit_instr

z80_iybits2:
	db	%52
	db	%DD	; bit operand for ix+d
	db	%CB	; bit operand
fd_bit_instr:
	db	%00
	db	%06
	ex	af, af'
	exx
	ld	ix, z80_regs
	z80loop

	global	z80_emulator_isr
z80_emulator_isr:
	push	af
	ld	a, (_z80_flags)
	set	0, a
	ld	(_z80_flags), a
	pop	af
	RET.L			; WE SHOULD BE RETURNING INTO ADL MODE

	SECTION	code

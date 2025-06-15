	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1
	section	CODE

	xref	not_implemented
	xref	z80_callilmmn_addr
	xref	z80_callilmmn2
	xref	z80_flags
	xref	z80_load_all_registers
	xref	z80_save_all_registers
	xref	z80_set_int_state

	global	z80_ldee

	; $5B ld e, e aka lil suffix
z80_ldee:
	ld.s	a, (iy)
	cp	%CD		; call.lil
	jr	z, z80_callilmmn
	call	not_implemented
	z80loop

z80_callilmmn:
	; iy = {mb}:iy
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	a, mb
	ld	(iy+2), a
	pop	iy

	ld	hl, (iy+1)
	lea	iy, iy+4
	ld	(z80_callilmmn_addr), hl

	push	iy			; save return PC
	call	z80_load_all_registers

	call	z80_callilmmn2

	call	z80_save_all_registers
	pop	iy			; retrieve saved PC
	restore_ei_s
	call	z80_set_int_state
	z80loop

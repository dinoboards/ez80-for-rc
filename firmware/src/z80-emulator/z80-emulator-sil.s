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

	global	z80_lddd
	; $52 ld d, d aka .sil/il suffix - call.il/call.sil
z80_lddd:
.if	EN_EZ80_INSTR
	; iy = {mb}:iy
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	a, mb
	ld	(iy+2), a
	pop	iy

	ld	hl, (iy+1)
	ld	(z80_callilmmn_addr), hl

	ld.s	a, (iy)
	lea	iy, iy+4

	cp	%cd				; call.il
	jr	z, z80_callilmmn
	cp	%c4
	jr	z, z80_callilnzmmn
	cp	%cc
	jr	z, z80_callilzmmn
	cp	%d4
	jr	z, z80_callilncmmn
	cp	%dc
	jr	z, z80_callilcmmn
	cp	%e4
	jr	z, z80_callilpommn
	cp	%ec
	jr	z, z80_callilpemmn
	cp	%f4
	jr	z, z80_callilpmmn
	cp	%fc
	jr	z, z80_callilmmmn

	jp	not_implemented
.else
	z80loop
.endif

z80_callilmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	z80_callilmmn2

z80_calldone:
	call	z80_save_all_registers
	pop	iy			; retrieve saved PC
	call	z80_set_int_state
	z80loop

z80_callilnzmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	nz, z80_callilmmn2
	jr	z80_calldone

z80_callilzmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	z, z80_callilmmn2
	jr	z80_calldone

z80_callilncmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	nc, z80_callilmmn2
	jr	z80_calldone

z80_callilcmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	c, z80_callilmmn2
	jr	z80_calldone

z80_callilpommn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	po, z80_callilmmn2
	jr	z80_calldone

z80_callilpemmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	pe, z80_callilmmn2
	jr	z80_calldone

z80_callilpmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	p, z80_callilmmn2
	jr	z80_calldone

z80_callilmmmn:
	push	iy			; save return PC
	call	z80_load_all_registers

	call	m, z80_callilmmn2
	jr	z80_calldone

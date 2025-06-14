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
	di_and_save_s

	; invoke ei/nop just before call.il
	ld	a, (z80_ei_enabled)
	or	a
	ld	a, %FB	; EI
	jr	nz, __xei
	xor	a
__xei:
	ld	(z80_callilmmn2), a

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

	; set flag to indicate special int handling logic
	; note the current IY (return address) value  	RETURN_PTR
	; when int happens
	;   save all registers to spl
	;   set iy -> 38
	;   push sps RETURN_PTR (or maybe just push 0000)
	;   call z80_invoke_iy
	;   restore all registers from spl
	;   ret.l
	;
	; special handler on each loop
	;  check if iy == RETURN_PTR (or 0000)
	;  return from z80_invoke_iy

	push	iy			; save return PC
	set	2, (ix+z80_flags)
	call	z80_load_all_registers

	call	z80_callilmmn2

	di_and_save_s
	call	z80_save_all_registers
	res	2, (ix+z80_flags)
	pop	iy			; retrieve saved PC
	restore_ei_s
	call	z80_set_int_state
	z80loop

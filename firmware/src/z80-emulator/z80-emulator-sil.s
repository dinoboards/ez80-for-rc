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

	global	z80_lddd
	; $52 ld d, d aka .sil/il suffix - call.il/call.sil
z80_lddd:
	ld.s	a, (iy)
	cp	%CD		; call.sil
	jr	z, z80_callilmmn
	call	not_implemented
	z80loop

z80_callilmmn:
	DI_AND_SAVE
	ld.s	hl, (iy+1)	;todo fix this needs to load 24bit addr
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
	; special hanlder on each loop
	;  check if iy == RETURN_PTR (or 0000)
	;  return from z80_invoke_iy

	push	iy			; save return PC
	DI_AND_SAVE
	set	2, (ix+z80_flags)
	call	z80_load_all_registers
	RESTORE_EI
	call.il	z80_callilmmn2
	DI_AND_SAVE
	call	z80_save_all_registers
	res	2, (ix+z80_flags)
	RESTORE_EI
	pop	iy			; retrieve saved PC
	z80loop

	include "../config.inc"

	section	CODE

	.assume adl=1

	xref	_emulator_invoke
	xref	switch_addr
	xref	z80_save_all_registers
	xref	z80_invoke_iy
	xref	z80_load_all_registers

; call z80 function, using the z80 emulator
;
; BC, DE, HL passed through
; AF' is passed through as AF
; IX is passed through

; main registers, other than AF & IY, are passed as alt
; as such passing AF' and BC' to the sub-routine is not possible
;
; IY -> address within MBASE segment of function
;
_emulator_invoke:
	ex	af, af'
	call	z80_save_all_registers

	call	z80_invoke_iy

	call	z80_load_all_registers

	ret.l

	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1
	section	CODE

	xref	_marshall_isr
	xref	_marshall_isr_hook
	xref	not_implemented
	xref	z80_load_all_registers
	xref	z80_save_all_registers
	xref	z80_switch_to_native

	global	z80_ldcc

	; 49 ld c,c aka .lis/.l suffix
z80_ldcc:
	ld.s	a, (iy)
	cp	%D7		; RST.L %10
	jr	z, rst_l10
	cp	%CF		; RST.L %08
	jp	z, rst_l08
	cp	%C9		; RET.L
	jr	z, ret_l
	call	not_implemented
	z80loop

ret_l:	; we would have been called by RST.L %10 (EMULATOR_INVOKE) handler
	; so we just exit emulator and return
	; if call.sis is implemented though, this handler may need to be updated
	; TODO: do we not need to inc iy?????
	ret


rst_l08:
	inc	iy
	z80loop

rst_l10:
	inc	iy
	ex	af, af'
	exx
	cp	b
	jr	z, rst_l10_exch_ver
	or	a
	jr	nz, rst_l10_passthrough

	ex	af, af'
	ld	a, b
	cp	19				; SYSUTL_EMULSTAT_GET
	jr	z, rst_l10_emulstat_get
	ex	af, af'

rst_l10_passthrough:
	ex	af, af'
	exx

	push	iy
	call	z80_load_all_registers

	rst.l	%10

	call	z80_save_all_registers
	pop	iy

	z80loop

	; SYSUTL_VER_EXCHANGE also enables
	; switch to native execution
rst_l10_exch_ver:
	; change int handler from emulator version
	; to marshalling version
	ld	hl, _marshall_isr
	ld	(_marshall_isr_hook+1), hl

	ex	af, af'
	exx
	dec	iy
	jp	z80_switch_to_native

	; SYSUTL_EMULSTAT_GET
	; return status to indicate running in emulated environment
rst_l10_emulstat_get:
	ex	af, af'
	ld	a, 02			; emulated environment
	ex	af, af'
	exx
	z80loop

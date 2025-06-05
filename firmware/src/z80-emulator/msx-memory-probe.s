	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

	section	CODE

	global	probe_for_msx_system
; test and return if we are installed in a
; yellow msx platform - requiring the use of
; the z80 emulation mode
; return flags: Z if MSX system, NZ otherwise
probe_for_msx_system:

	; initialise PPI
	ld	bc, PPI_REGS
	ld	a, %82
	out	(bc), a

	ld	bc, GIO_REGS
	ld	a, %50
        out	(bc), a

	; set page 3 to slot 3
	ld	bc, PSL_STAT
	ld	a, %C0		; 11 00 00 00
	out	(bc), a

	; set sub slot to 3-2
	ld	hl, %FFFF
	ld.s	(hl), %80	; 10 00 00 00
	ld.s	a, (hl)
	; if we read back the compliment - then likely we have a sub-slot system
	; aka - msx system

	cp	%7F
	ret

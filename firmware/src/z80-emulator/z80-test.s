IFDEF ZEXALL

	include	"..\config.inc"

	.assume	adl=1

	SECTION CODE

	global	Z80test
	xref	z80_invoke
	xref	zexall
	xref	zexall_end
Z80test:

	xor	a
	ld	b,14
	ld	l, 1
	RST.L	%10

	ld	de, %200100
	ld	hl, zexall
	ld	bc, zexall_end-zexall
	ldir


	ld	a, %20
	ld	mb, a
	ld.sis	sp, %FFFF
	;call.sis	%0100
	ld	iy, %0100
	call	z80_invoke

done:
	nop
	nop
	nop
	jr	done
	; copy embedded test into external ram location (say 0x200000)
	; call to 0x20 0000
	; pause

	; embedded test needs to be change from cp/m to use firmware bios calls (rst.l for ch out)

ENDIF

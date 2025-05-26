IFDEF ZEXALL

	include	"..\config.inc"

	.assume	adl=1

	SECTION CODE

	global	Z80test
	xref	z80_invoke
	xref	zexall
	xref	zexall_end
Z80test:

	; set extended memory timing to 1WS
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
	call.sis	%0100
	; ld	iy, %0100
	; call	z80_invoke

done:
	nop
	nop
	nop
	jr	done

ENDIF

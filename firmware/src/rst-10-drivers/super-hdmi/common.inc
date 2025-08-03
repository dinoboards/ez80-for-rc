
CMD_PSET	equ	%50
CMD_LINE	equ	%70
CMD_LMMV	equ	%80
CMD_LMMM	equ	%90
CMD_LMMC	equ	%B0
CMD_HMMV	equ	%C0
CMD_HMMM	equ	%D0
CMD_HMMC	equ	%F0

SET_SLOW_IO_SPEED: macro
endmac
RESTORE_IO_SPEED: macro
endmac

DI_AND_SAVE: macro
	ld	a, i ; P = IEF2
	di
	push	af
endmac

RESTORE_EI: macro
	pop	af
	jp	po, $$skipEI
	ei
$$skipEI:
endmac

	XREF	_VDP_IO_REGS
	XREF	_VDP_IO_ADDR

	INCLUDE	"config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_main
	XREF	_uart0_init
	XREF	_bank_init_z2
	XREF	_rx_buffer_init
	xref	_init_clocks

_main:
	CALL	_init_clocks
	CALL	_bank_init_z2
	CALL	_rx_buffer_init
	CALL	_uart0_init

	;XREF	_spike
	;call	_spike

	LD	A, Z80_ADDR_MBASE		; set MBASE to $03
	LD	MB, A
	JP.SIS	0				; transfer to external Memory under Z80 Compatible mode

	SEGMENT BSS

_tmp:
	DS	3

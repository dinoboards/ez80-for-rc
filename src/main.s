	SECTION CODE

	.assume adl=1

	PUBLIC	_main
	XREF	_x_uart_init
	XREF	bank_init_z2
	XREF	_spike

_main:
	CALL	_x_uart_init
	CALL	bank_init_z2

	;call	_spike

	LD	A, %B9				; set MBASE to $B9
	LD	MB, A
	JP.SIS	0				; transfer to external Memory under Z80 Compatible mode

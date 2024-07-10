
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	XREF	uart_control

UART_ERR_FIFOBUFFERFULL	EQU	%0C		;!< The error code returned when the transmit FIFO buffer is full.

uart_control:
	POP	BC		; restore bc and hl
	POP	HL

	LD	A, B		; SUB FUNCTION CODE
	OR	A		; TEST SUB FUNCTION CODE
	JR	Z, uart_in	; B = 0, get ticks
	DEC	A
	JR	Z, uart_out	; B = 1
	DEC	A
	JR	Z, uart_ist	; B = 2
	DEC	A
	JR	Z, uart_ost	; B = 3
	DEC	A
	JR	Z, uart_initdev	; B = 4
	DEC	A
	JR	Z, uart_query	; B = 5
	DEC	A
	JR	Z, uart_device	; B = 6

	LD	A, %FF		; UNKNOWN UART FUNCTION
	RET.L

uart_in:
	; block and wait for char
	; return in E

uart_out:
uart_ist:
uart_ost:
uart_initdev:
uart_query:
uart_device:
	LD	A, %FF		; UNKNOWN UART FUNCTION
	RET.L

	PUBLIC	_uart0_receive_isr
	XREF	_rx_buffer_add_to
	XREF	_rx_buffer_add_to

_uart0_receive_isr:
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL
	IN0	A, (UART0_IIR);
	BIT	0, A
	JR	NZ, _uart0_receive_isr_end
	IN0	E, (UART0_RBR)

	CALL	_rx_buffer_add_to

_uart0_receive_isr_end:
	POP	HL
	POP	DE
	POP	BC
	POP	AF
	EI
	RETI.L

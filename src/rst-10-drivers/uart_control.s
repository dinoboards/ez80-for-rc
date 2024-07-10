
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	uart_control
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	XREF	_rx_buffer_get_length

UART_ERR_FIFOBUFFERFULL	EQU	%0C		;!< The error code returned when the transmit FIFO buffer is full.

uart_control:
	POP	BC		; restore bc and hl
	POP	HL

	LD	A, B		; SUB FUNCTION CODE
	OR	A		; TEST SUB FUNCTION CODE
	JR	Z, uart_in	; B = 0
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


	; block and wait for char
	; return in E
uart_in:
	CALL	_rx_buffer_empty
	OR	A
	JR	NZ, uart_in

	CALL	_rx_buffer_get
	XOR	A
	RET.L

	; send char in E
uart_out:
	IN0	A, (UART0_LSR)		; WAIT FOR TX READY
	AND	LSR_THRE
	JR	Z, uart_out

	OUT0	(UART0_THR), E		; SEND THE CHAR
	XOR	A
	RET.L

uart_ist:
	CALL	_rx_buffer_get_length	; RETURN THE NUMBER OF CHARS IN THE RX BUFFER IN A

	BIT	7, A			; IF A > 127, THEN RETURN 127 (NEGATIVE NUMBERS ARE ERROR CODES)
	RET.L	Z

	LD	A, 127
	RET.L

uart_ost:


uart_initdev:
uart_query:
uart_device:
	LD	A, %FF		; UNKNOWN UART FUNCTION
	RET.L

	PUBLIC	_uart0_init

BRG	EQU	(CPU_CLK_FREQ/(16 * UART_BPS))
BRG_L	EQU	(BRG & %FF)
BRG_H	EQU	((BRG & %FF00) >> 8)

_uart0_init:
	di
	LD	A, %3
	OUT0	(PD_ALT2),A

	XOR	A
	OUT0	(PD_ALT1),A
	OUT0	(PD_DR),A

	LD	A, %EB
	OUT0	(PD_DDR),A

	IN0	A, (UART0_LCTL)		; ENABLE REGISTER ACCESS
	SET	7, A			; SET DLAB BIT
	OUT0	(UART0_LCTL), A

	LD	A, BRG_L
	OUT0	(UART0_BRG_L), A

	LD	A, BRG_H
	OUT0	(UART0_BRG_H), A

	IN0	A, (UART0_LCTL)		; DISABLE REGISTER ACCESS
	RES	7, A			; CLEAR DLAB BIT
	OUT0	(UART0_LCTL), A

	XOR	A
	OUT0	(UART0_MCTL), A

	LD	A, UART_FCTL_FIFOEN |UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4
	OUT0	(UART0_FCTL), A

	LD	A, LCTL_8_BITS_1_STOP_BIT
	OUT0	(UART0_LCTL), A

	LD	A, 1
	OUT0	(UART0_IER), A		; ENABLE INTERRUPTS FOR RECEIVE EVENTS

	ei
	RET




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

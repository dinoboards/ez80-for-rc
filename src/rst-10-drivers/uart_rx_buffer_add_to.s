
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	XREF	_rx_buffer_full
	XREF	_rx_buf_next_in
	XREF	_rx_buf

UART_ERR_FIFOBUFFERFULL	EQU	%0C		;!< The error code returned when the transmit FIFO buffer is full.

	PUBLIC	_rx_buffer_add_to

_rx_buffer_add_to:
	CALL	_rx_buffer_full
	OR	A
	JR	Z, L_1

	LD	A, UART_ERR_FIFOBUFFERFULL
	RET

 L_1:
	LD	A, (_rx_buf_next_in)		; _rx_buf[rx_buf_next_in] = c
	UEXT	HL
	LD	L, A
	LD	BC, _rx_buf
	ADD	HL, BC
	LD	(HL), E

	INC	A
	LD	(_rx_buf_next_in), A		; _rx_buf_next_in++

	CP	A, RX_BUFFER_SIZE		; _rx_buf_next_in >= RX_BUFFER_SIZE ?
	JR	C, L_4

	XOR	A				; YES, _rx_buf_next_in = 0
	LD	(_rx_buf_next_in), A

 L_4:
	XOR	A
	RET

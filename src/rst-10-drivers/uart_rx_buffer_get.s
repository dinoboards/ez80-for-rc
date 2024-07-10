
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

UART_ERR_RECEIVEFIFOEMPTY	.EQU	%0F		;!< The error code returned when the software receive FIFO buffer is empty.

	XREF	_rx_buffer_empty
	XREF	_rx_buf
	XREF	_rx_buf_next_out

	PUBLIC	_rx_buffer_get

_rx_buffer_get:
	CALL	_rx_buffer_empty
	OR	A
	JR	Z, L_1

	LD	A, UART_ERR_RECEIVEFIFOEMPTY
	RET

 L_1:
	LD	A, (_rx_buf_next_out)
	UEXT	HL
	LD	L, A
	LD	BC, _rx_buf
	ADD	HL, BC			; HL = rx_buf + rx_buf_next_out

	LD	E, (HL)			; char to be returned!!

	INC	A
	LD	(_rx_buf_next_out), A	; rx_buf_next_out++;

	CP	RX_BUFFER_SIZE		; rx_buf_next_out >= RX_BUFFER_SIZE
	JR	C, L_4

	XOR	A			; yes, rx_buf_next_out = 0
	LD	(_rx_buf_next_out), A

L_4:
	XOR	A
	RET

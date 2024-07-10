
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_rx_buffer_get_length
	XREF	_rx_buf_next_in
	XREF	_rx_buf_next_out

_rx_buffer_get_length:
	LD	A, (_rx_buf_next_out)
	LD	HL, _rx_buf_next_in
	CP	A, (HL)				; rx_buf_next_in > rx_buf_next_out ?
	JR	Z, L_2
	JR	NC, L_1

	LD	C, A				; yes, return rx_buf_next_in - rx_buf_next_out
	LD	A, (HL)
	SUB	A, C
	RET

L_1:
	LD	C, A				; return rx_buf_next_in - rx_buf_next_out + RX_BUFFER_SIZE
	LD	A, (HL)
	SUB	A, C
	ADD	A, RX_BUFFER_SIZE
	RET

L_2:	XOR	A				; is empty
	RET

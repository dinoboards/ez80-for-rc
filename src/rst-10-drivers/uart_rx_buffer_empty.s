
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	XREF	_rx_buf_next_in
	XREF	_rx_buf_next_out

	PUBLIC	_rx_buffer_empty
_rx_buffer_empty:
	LD	A, (_rx_buf_next_in)
	LD	HL, _rx_buf_next_out
	CP	A, (HL)
	LD	A, 1
	RET	Z		; RETURN NON-ZERO IF EMPTY

	XOR	A		; RETURN ZERO IF EMPTY
	RET

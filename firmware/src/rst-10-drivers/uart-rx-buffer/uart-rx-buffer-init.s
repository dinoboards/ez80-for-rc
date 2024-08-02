
        INCLUDE "..\..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_rx_buffer_init
	XREF	_rx_buf_next_in
	XREF	_rx_buf_next_out

 _rx_buffer_init:
	XOR	A
	LD	(_rx_buf_next_in), A
	LD	(_rx_buf_next_out), A
	RET

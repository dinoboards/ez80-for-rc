
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	XREF	_rx_buf_next_in
	XREF	_rx_buf_next_out

	PUBLIC	_rx_buffer_full

	; DESTROYS REGISTERS AF AND B
_rx_buffer_full:
 	LD	A, (_rx_buf_next_in)
 	LD	B, A
 	INC	B					; B = _rx_buf_next_in + 1

 	LD	A, RX_BUFFER_SIZE
	CP	B					; B == RX_BUFFER_SIZE ?

 	JR	NZ, L_3
 	LD	B, 0					; YES, THEN ZERO E

L_3:
 	LD	A, (_rx_buf_next_out)
	CP	B					; B == _rx_buf_next_out ?
 	JR	NZ, L_4

 	LD	A, 1					; YES, RETURN 1 TO INDICATE FULL
 	RET
L_4:
 	XOR	A					; NO, RETURN 0 TO INDICATE NOT FULL
 	RET



        INCLUDE "..\config.inc"

	SECTION BSS

	.assume adl=1

	PUBLIC	_rx_buf
	PUBLIC	_rx_buf_next_in
	PUBLIC	_rx_buf_next_out
	PUBLIC	_baud_rate
	PUBLIC	_line_control

_rx_buf:
	DS	RX_BUFFER_SIZE

_rx_buf_next_in:
	DS	1

_rx_buf_next_out:
	DS	1

	SECTION	DATA
;
; line control bits:
;
; {0:1} = Parity    (00 -> NONE, 01 -> NONE, 10 -> ODD, 11 -> EVEN)
; {2}   = Stop Bits (0 -> 1, 1 -> 2)
; {3:4} = Data Bits (00 -> 5, 01 -> 6, 10 -> 7, 11 -> 8)
; {5:5} = Hardware Flow Control CTS (0 -> OFF, 1 -> ON)
;
_line_control:
	DB	3 << 3 ; 8 BITS, 1 STOP BIT, NO PARITY, CTS FLOW CONTROL DISABLED

_baud_rate:
	DW24	UART_BPS

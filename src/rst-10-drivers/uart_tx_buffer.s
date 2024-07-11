
        INCLUDE "..\config.inc"

	SECTION BSS

	.assume adl=1

	PUBLIC	_rx_buf
	PUBLIC	_rx_buf_next_in
	PUBLIC	_rx_buf_next_out
	PUBLIC	_cts_flow_control

_rx_buf:
	DS	RX_BUFFER_SIZE

_rx_buf_next_in:
	DS	1

_rx_buf_next_out:
	DS	1

_cts_flow_control:
	DS	1	; IF 1, THEN  ONLY TRANSMITT IF CTS IS READ AS LOW

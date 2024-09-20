
        INCLUDE "..\config.inc"

	XREF	__lmulu
	XREF	_baud_rate
	XREF	_cpu_freq_calculated
	XREF	_line_control
	XREF	_rx_buf_next_in
	XREF	_rx_buf_next_out
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	XREF	_rx_buffer_get_length
	XREF	_get_brg

	SECTION INTERNAL_RAM_ROM

	.assume adl=1

	PUBLIC	_uart_dispatch

UART_ERR_FIFOBUFFERFULL	EQU	%0C			; THE ERROR CODE RETURNED WHEN THE TRANSMIT FIFO BUFFER IS FULL.

_uart_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, uart_in				; B = 0
	DEC	A
	JR	Z, uart_out				; B = 1
	DEC	A
	JR	Z, uart_ist				; B = 2
	DEC	A
	JR	Z, uart_ost				; B = 3
	DEC	A
	JR	Z, uart_config				; B = 4
	DEC	A
	JR	Z, uart_query				; B = 5
	DEC	A
	JR	Z, uart_reset				; B = 6

	LD	A, %FF					; UNKNOWN UART FUNCTION
	RET.L
;
; Function B = 00 -- Character Input (UART_IN)
;  Output E = Character
;  Output A = Status
;
; Read and return a Character (E).  If no character(s) are available in the unit's input buffer, this function will
; wait indefinitely.  The returned Status (A) is a standard  HBIOS result code.
;
uart_in:
	CALL	_rx_buffer_empty
	OR	A
	JR	NZ, uart_in

	CALL	_rx_buffer_get_length
	CP	RX_BUFFER_LOW				; BUFFER_SIZE == RX_BUFFER_LOW ?
	JR	NZ, uart_in_end				; NO, DO NOTHING

	DI
	IN0	A, (UART0_MCTL)				; YES, RESUME RECEIVING BYTES
	OR	MCTL_RTS
	OUT0	(UART0_MCTL), A
	EI

uart_in_end:
	CALL	_rx_buffer_get

	XOR	A
	RET.L
;
; Function B = 01 -- Character Output (UART_OUT)
;  Input E = Character to send
;  Output A = Status
;
; Send a Character (E). If there is no space available in the unit's output buffer, the function will wait
; indefinitely.  The returned Status (A) is a standard HBIOS result code.
; Will block if CTS flow control enabled, and receiver does not assert (low) our CTS input.
;
uart_out:
	IN0	A, (UART0_LSR)				; WAIT FOR TX READY
	AND	LSR_THRE
	JR	Z, uart_out

	LD	A, (_line_control)			; IS CTS FLOW CONTROL ENABLED
	BIT	5, A
	JR	Z, uart_out_char			; NO, SEND THE CHAR NOW

uart_wait_for_cts_low:
	IN0	A, (UART0_MSR)				; YES, WAIT FOR CTS TO GO LOW
	AND	MSR_CTS
	JR	Z, uart_wait_for_cts_low

uart_out_char:
	OUT0	(UART0_THR), E				; SEND THE CHAR
	XOR	A
	RET.L
;
; Return the count of Characters Pending (A) in the input buffer.
;  Output A = Status/Characters Pending
;
; The value returned in register A is used as both a Status (A) code and the return value. Negative values
; (bit 7 set) indicate a standard HBIOS result (error) code.  Otherwise, the return value represents the number
; of characters in the input buffer.
;
uart_ist:
	CALL	_rx_buffer_get_length			; RETURN THE NUMBER OF CHARS IN THE RX BUFFER IN A

	BIT	7, A					; IF A > 127, THEN RETURN 127 (NEGATIVE NUMBERS ARE ERROR CODES)
	JR	Z, uart_ist_postive

	LD	A, 127
uart_ist_postive:
	OR	A
	RET.L
;
; Function B = 03 -- Character Output Status (UART_OST)
;   Output A = Transmitter Status
;
; Return the status of the output FIFO.  0 means the output FIFO is full and no more characters can be sent. 1 means
; the output FIFO is not full and at least one character can be sent.
; Negative values (bit 7 set) indicate a standard HBIOS result (error) code.
;
; TODO: if CTS flow control enabled, return 0 if CTS is high.
;
uart_ost:
	IN0	A, (UART0_LSR)
	AND	LSR_THRE
	RET.L	Z
	LD	A, 1
	RET.L
;
; Function B = 04 -- Configure UART Device (UART_CONFIG)
;   Input:
;     HL{23:0} = New desired baud rate
;     D{0:1} = Parity    (00 -> NONE, 01 -> NONE, 10 -> ODD, 11 -> EVEN)
;     D{2}   = Stop Bits (0 -> 1, 1 -> 2)
;     D{3:4} = Data Bits (00 -> 5, 01 -> 6, 10 -> 7, 11 -> 8)
;     D{5:5} = Hardware Flow Control CTS (0 -> OFF, 1 -> ON)
;   Output
;     A = Status
;
; Configure the UART device with the new desired baud rate, parity, stop bits, and data bits.  The returned Status (A)
; is a standard HBIOS result code.
; If CTS flow control is enabled, transmission will pause while the CTS signal is held high.
;
; TODO: add ability to enable/disable RTS flow control. (if disabled - ensure RTS is set to high impedance)
;
	XREF	__ldivu
	XREF	__itol
uart_config:
	LD	(_baud_rate), HL

	PUSH	DE

	IN0	A, (UART0_LCTL)				; ENABLE REGISTER ACCESS
	SET	7, A					; SET DLAB BIT
	OUT0	(UART0_LCTL), A

	CALL	_get_brg
	OUT0	(UART0_BRG_L), L
	OUT0	(UART0_BRG_H), H

	RES	7, A					; DISABLE REGISTER ACCESS
	OUT0	(UART0_LCTL), A

	POP	DE
	LD	A, D
	AND	3
	INC	A
	LD	B, A

	LD	A, D					; MOVE DESIRED BIT LENGTH TO A
	LD	(_line_control), A			; SAVE NEW LINE CONTROL CONFIG
	RRCA
	RRCA
	RRCA
	AND	3

	DEC	B					; E{0:1} == 0
	JR	Z, uart_config_assign_stop_bits

	DEC	B					; E{0:1} == 1
	JR	Z, uart_config_assign_stop_bits

	DEC	B					; E{0:1} == 2
	JR	NZ, uart_config_even_parity
	OR	LCTL_ODD_PARITY				; CONFIGURE ODD PARITY
	JR	uart_config_assign_stop_bits

uart_config_even_parity:
	OR	LCTL_EVEN_PARITY

uart_config_assign_stop_bits:
	BIT	2, D
	JR	Z, uart_config_assign_line		; 1 STOP BIT IS THE DEFAULT

	OR	LCTL_2_STOP_BITS			; ENABLE 2 STOP BITS

uart_config_assign_line:
	OUT0	(UART0_LCTL), A

	XOR	A
	RET.L
;
; Function B = 05 -- GET UART Device Configure (UART_QUERY)
;   Output:
;     HL{23:0} = New desired baud rate
;     E = H{23:16}
;     D{0:1} = Parity    (00 -> NONE, 01 -> NONE, 10 -> ODD, 11 -> EVEN)
;     D{2}   = Stop Bits (0 -> 1, 1 -> 2)
;     D{3:4} = Data Bits (00 -> 5, 01 -> 6, 10 -> 7, 11 -> 8)
;     D{5:5} = Hardware Flow Control CTS (0 -> OFF, 1 -> ON)
;     A = Status
;
; Retreive the UART device's current configuration.  The returned Status (A) is a standard HBIOS result code.
;
uart_query:
	LD	HL, (_baud_rate)
	LD	A, (_baud_rate+2)
	LD	E, A
	LD	A, (_line_control)
	LD	D, A

uart_query_end:
	XOR	A
	RET.L
;
; Function B = 06 -- UART flush all buffers (UART_RESET)
;
; Output
;   A = Status
;
uart_reset:
	DI

	LD	A, UART_FCTL_FIFOEN | UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4
	OUT0	(UART0_FCTL), A

	XOR	A
	LD	(_rx_buf_next_in), A
	LD	(_rx_buf_next_out), A

	EI
	XOR	A
	RET.L

	PUBLIC	_uart0_init

_uart0_init:
	di
	LD	A, %0F					; 0000 0011
	OUT0	(PD_ALT2),A

	LD	A, %F0
	OUT0	(PD_ALT1),A

	XOR	A					; 0000 0000
	OUT0	(PD_DR),A

	LD	A, %0F					; 1110 1011
	OUT0	(PD_DDR),A

	IN0	A, (UART0_LCTL)				; ENABLE REGISTER ACCESS
	SET	7, A					; SET DLAB BIT
	OUT0	(UART0_LCTL), A

	LD	HL, UART_BPS
	LD	(_baud_rate), HL

	CALL	_get_brg
	OUT0	(UART0_BRG_L), L
	OUT0	(UART0_BRG_H), H

	IN0	A, (UART0_LCTL)				; DISABLE REGISTER ACCESS
	RES	7, A					; CLEAR DLAB BIT
	OUT0	(UART0_LCTL), A

	LD	A, MCTL_RTS				; H/W FLOW CONTROL - ALLOW RECEIVING BYTES
	OUT0	(UART0_MCTL), A

	LD	A, UART_FCTL_FIFOEN | UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4
	OUT0	(UART0_FCTL), A

	LD	A, LCTL_8_BITS | LCTL_1_STOP_BIT
	OUT0	(UART0_LCTL), A

	LD	A, 3 << 3                               ; 8 BITS, 1 STOP BIT, NO PARITY, CTS FLOW CONTROL DISABLED
	LD	(_line_control), A

	LD	A, 1
	OUT0	(UART0_IER), A				; ENABLE INTERRUPTS FOR RECEIVE EVENTS

	EI
	RET

	PUBLIC	_uart0_receive_isr
	XREF	_rx_buffer_add_to
	XREF	_rx_buffer_add_to

_uart0_receive_isr:
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL

	LD	B, 4

_uart0_receive_isr_loop
	IN0	A, (UART0_IIR)
	BIT	0, A
	JR	NZ, _uart0_receive_isr_end
	IN0	E, (UART0_RBR)

	PUSH	BC
	CALL	_rx_buffer_add_to
	CALL	_rx_buffer_get_length
	POP	BC

	CP	RX_BUFFER_HIGH				; RX_BUFFER_FULL = BUFFER_SIZE ?
	JR	NZ, _uart0_receive_isr_try_next		; NO, DO NOTHING

	IN0	A, (UART0_MCTL)				; YES, STOP RECEIVING BYTES
	AND	~MCTL_RTS
	OUT0	(UART0_MCTL), A

_uart0_receive_isr_try_next:
	DJNZ	_uart0_receive_isr_loop

_uart0_receive_isr_end:
	POP	HL
	POP	DE
	POP	BC
	POP	AF
	EI
	RETI.L

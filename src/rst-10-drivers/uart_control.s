
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	uart_control
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	XREF	_rx_buffer_get_length
	XREF	_cts_flow_control

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
	JR	Z, uart_config	; B = 4
	DEC	A
	JR	Z, uart_query	; B = 5

	LD	A, %FF		; UNKNOWN UART FUNCTION
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
	JR	nZ, uart_in

	CALL	_rx_buffer_get_length
	CP	RX_BUFFER_LOW			; buffer_size == RX_BUFFER_LOW ?
	JR	NZ, uart_in_end			; no, do nothing

	DI
	IN0	A, (UART0_MCTL)			; yes, resume receiving bytes
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
	IN0	A, (UART0_LSR)		; WAIT FOR TX READY
	AND	LSR_THRE
	JR	Z, uart_out

	LD	A, (_cts_flow_control)	; IS CTS FLOW CONTROL ENABLED
	OR	A
	JR	Z, uart_out_char	; NO, SEND THE CHAR NOW

uart_wait_for_cts_low:
	IN0	A, (UART0_MSR)		; YES, WAIT FOR CTS TO GO LOW
	AND	MSR_CTS
	JR	Z, uart_wait_for_cts_low

uart_out_char:
	OUT0	(UART0_THR), E		; SEND THE CHAR
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
	CALL	_rx_buffer_get_length	; RETURN THE NUMBER OF CHARS IN THE RX BUFFER IN A

	BIT	7, A			; IF A > 127, THEN RETURN 127 (NEGATIVE NUMBERS ARE ERROR CODES)
	RET.L	Z

	LD	A, 127
	RET.L

;
; Function B = 03 -- Character Output Status (UART_OST)
;   Output A = Transmitter Status
;
; Return the status of the output FIFO.  0 means the output FIFO is full and no more characters can be sent. 1 means
; the output FIFO is not full and at least one character can be sent.
; Negative values (bit 7 set) indicate a standard HBIOS result (error) code.
;
uart_ost:
	IN0	A, (UART0_LSR)
	AND	LSR_THRE
	RET.L	Z
	LD	A, 1
	RET.L

;
; Function B = 04 -- Configure UART Device (UART_CONFIG)
;   Input HL{23:0} = New desired baud rate
;   DE{0:1} = Parity    (00 -> NONE, 01 -> NONE, 10 -> ODD, 11 -> EVEN)
;   DE{2}   = Stop Bits (0 -> 1, 1 -> 2)
;   DE{3:4} = Data Bits (00 -> 5, 01 -> 6, 10 -> 7, 11 -> 8)
;   DE{5:5} = Hardware Flow Control CTS (0 -> OFF, 1 -> ON)
;   Output A = Status
;
; Configure the UART device with the new desired baud rate, parity, stop bits, and data bits.  The returned Status (A)
; is a standard HBIOS result code.
; If CTS flow control is enabled, transmission will pause while the CTS signal is held high.
;
	XREF	__ldivu
	XREF	__itol
uart_config:
	PUSH	DE
	XOR	A					; A:HL = HL * 16
	ADD	HL,HL
	RLA
	ADD	HL,HL
	RLA
	ADD	HL,HL
	RLA
	ADD	HL,HL
	RLA

	LD	BC, HL

	LD	HL, CPU_CLK_FREQ & %FFFFFF
	LD	E, CPU_CLK_FREQ >> 24
	CALL	__ldivu					; HL = E:HL / A:BC
							; HL = BRG

	IN0	A, (UART0_LCTL)				; ENABLE REGISTER ACCESS
	SET	7, A					; SET DLAB BIT
	OUT0	(UART0_LCTL), A

	OUT0	(UART0_BRG_L), L
	OUT0	(UART0_BRG_H), H

	RES	7, A					; DISABLE REGISTER ACCESS
	OUT0	(UART0_LCTL), A

	POP	DE
	LD	A, E
	AND	3
	INC	A
	LD	B, A

	LD	A, E					; MOVE DESIRED BIT LENGTH TO A
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
	BIT	2, E
	JR	Z, uart_config_assign_line		; 1 STOP BIT IS THE DEFAULT

	OR	LCTL_2_STOP_BITS			; ENABLE 2 STOP BITS

uart_config_assign_line:
	OUT0	(UART0_LCTL), A

	BIT 	5, E
	JR	NZ, uart_config_enable_flow_control

	XOR	A
	LD	(_cts_flow_control), A			; DISABLE CTS FLOW CONTROL
	RET.L

uart_config_enable_flow_control:
	LD	A, 1
	LD	(_cts_flow_control), A			; ENABLE CTS FLOW CONTROL
	XOR	A
	RET.L

uart_query:
	LD	A, %FF					; UNKNOWN UART FUNCTION
	RET.L

	PUBLIC	_uart0_init

BRG	EQU	(CPU_CLK_FREQ/(16 * UART_BPS))
BRG_L	EQU	(BRG & %FF)
BRG_H	EQU	((BRG & %FF00) >> 8)

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

	LD	A, BRG_L
	OUT0	(UART0_BRG_L), A

	LD	A, BRG_H
	OUT0	(UART0_BRG_H), A

	IN0	A, (UART0_LCTL)				; DISABLE REGISTER ACCESS
	RES	7, A					; CLEAR DLAB BIT
	OUT0	(UART0_LCTL), A

	LD	A, MCTL_RTS				; H/W FLOW CONTROL - ALLOW RECEIVING BYTES
	OUT0	(UART0_MCTL), A

	LD	A, UART_FCTL_FIFOEN | UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4
	OUT0	(UART0_FCTL), A

	LD	A, LCTL_8_BITS | LCTL_1_STOP_BIT
	OUT0	(UART0_LCTL), A

	LD	A, 1
	OUT0	(UART0_IER), A				; ENABLE INTERRUPTS FOR RECEIVE EVENTS

	XOR	A					; BY DEFAULT, CTS FLOW CONTROL (TRANSMISSION) IS DISABLED
	LD	(_cts_flow_control), A
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

	CP	RX_BUFFER_HIGH				; RX_BUFFER_FULL = buffer_size ?
	JR	NZ, _uart0_receive_isr_try_next		; no, do nothing

	IN0	A, (UART0_MCTL)				; yes, stop receiving bytes
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

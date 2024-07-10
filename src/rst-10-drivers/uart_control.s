
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	uart_control
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	XREF	_rx_buffer_get_length

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
	JR	Z, uart_initdev	; B = 4
	DEC	A
	JR	Z, uart_query	; B = 5
	DEC	A
	JR	Z, uart_device	; B = 6

	LD	A, %FF		; UNKNOWN UART FUNCTION
	RET.L
;
; Function B = 00 -- Character Input (CIOIN)
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

	CALL	_rx_buffer_get
	XOR	A
	RET.L
;
; Function B = 01 -- Character Output (CIOOUT)
;  Input E = Character to send
;  Output A = Status
;
; Send a Character (E). If there is no space available in the unit's output buffer, the function will wait
; indefinitely.  The returned Status (A) is a standard HBIOS result code.
;
uart_out:
	IN0	A, (UART0_LSR)		; WAIT FOR TX READY
	AND	LSR_THRE
	JR	Z, uart_out

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
; Function B = 03 -- Character Output Status (CIOOST)
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

uart_initdev:
uart_query:
uart_device:
	LD	A, %FF		; UNKNOWN UART FUNCTION
	RET.L

	PUBLIC	_uart0_init

BRG	EQU	(CPU_CLK_FREQ/(16 * UART_BPS))
BRG_L	EQU	(BRG & %FF)
BRG_H	EQU	((BRG & %FF00) >> 8)

_uart0_init:
	di
	LD	A, %3
	OUT0	(PD_ALT2),A

	XOR	A
	OUT0	(PD_ALT1),A
	OUT0	(PD_DR),A

	LD	A, %EB
	OUT0	(PD_DDR),A

	IN0	A, (UART0_LCTL)		; ENABLE REGISTER ACCESS
	SET	7, A			; SET DLAB BIT
	OUT0	(UART0_LCTL), A

	LD	A, BRG_L
	OUT0	(UART0_BRG_L), A

	LD	A, BRG_H
	OUT0	(UART0_BRG_H), A

	IN0	A, (UART0_LCTL)		; DISABLE REGISTER ACCESS
	RES	7, A			; CLEAR DLAB BIT
	OUT0	(UART0_LCTL), A

	XOR	A
	OUT0	(UART0_MCTL), A

	LD	A, UART_FCTL_FIFOEN |UART_FCTL_CLRRxF | UART_FCTL_CLRTxF | UART_FCTL_TRIG_4
	OUT0	(UART0_FCTL), A

	LD	A, LCTL_8_BITS_1_STOP_BIT
	OUT0	(UART0_LCTL), A

	LD	A, 1
	OUT0	(UART0_IER), A		; ENABLE INTERRUPTS FOR RECEIVE EVENTS

	ei
	RET




	PUBLIC	_uart0_receive_isr
	XREF	_rx_buffer_add_to
	XREF	_rx_buffer_add_to

_uart0_receive_isr:
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL
	IN0	A, (UART0_IIR);
	BIT	0, A
	JR	NZ, _uart0_receive_isr_end
	IN0	E, (UART0_RBR)

	CALL	_rx_buffer_add_to

_uart0_receive_isr_end:
	POP	HL
	POP	DE
	POP	BC
	POP	AF
	EI
	RETI.L

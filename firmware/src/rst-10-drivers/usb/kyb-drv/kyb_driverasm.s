
        INCLUDE "..\..\..\config.inc"

	SECTION CODE

	.assume adl=1

	XREF	_alt_read_index
	; XREF	_usb_kyb_buf_size
	XREF	_reports

	GLOBAL	_usb_kyb_install_timer_tick
	XREF	_system_timer_isr
	XREF	_system_timer_isr_hook
	XREF	_usb_kyb_tick

_usb_kyb_install_timer_tick:
	LD	HL, (_system_timer_isr_hook+1)
	LD	(timer_isr_hook_next+1), HL
	LD	HL, timer_isr_hook
	LD	(_system_timer_isr_hook+1), HL
	RET

timer_isr_hook:
	PUSH	IX
	PUSH	IY
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	CALL	_usb_kyb_tick
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IY
	POP	IX
	JP	timer_isr_hook_next

	SECTION DATA

timer_isr_hook_next:
	JP	_system_timer_isr

	SECTION CODE

	GLOBAL	_report_diff
	XREF	_report
	XREF	_previous

; uint8_t report_diff();
_report_diff:
	LD	DE, _report
	LD	HL, _previous
	LD	B, 8 				; //sizeof(keyboard_report_t)

.rptdf_loop:
	LD	A, (DE)
	INC	DE
	CP	(HL)
	INC	HL
	JR	NZ, .rptdf_diff
	DJNZ	.rptdf_loop

	XOR	A
	RET

.rptdf_diff:
	LD	A, 1
	RET
;
; Inputs:
;   None
;
; Outputs:
;   A: Status / Codes Pending
;   B: Number of buffered usb reports
;   A': USB Report Modifier Key State (valid if B > 0)
;   B', C', D', E', H', L': USB Report's 6 key codes (valid only if B > 0)
;
; Return a count of the number of key Codes Pending (A) in the keyboard buffer.
; If it is not possible to determine the actual number in the buffer, it is
; acceptable to return 1 to indicate there are key codes available to read and
; 0 if there are none available.
; The value returned in register A is used as both a Status (A) code and the
; return value. Negative values (bit 7 set) indicate a standard HBIOS result
; (error) code. Otherwise, the return value represents the number of key codes
; pending.
;
; USB Keyboard Extension:
; Returns the current USB HID keyboard report data.
; Register B contains the number of buffered reports available:
;   B = 0: No reports available
;   B > 0: At least one report available (will be consumed after reading)
; When a report is available (B > 0):
;   A': Contains modifier key states
;   B',C',D',E',H',L': Contains up to 6 concurrent key codes
; See USB HID Usage Tables specification for key codes

; _usb_kyb_report:
; 	exx
; 	ld	hl, (_alt_read_index)
; 	ld	h, %00
; 	add	hl, hl
; 	add	hl, hl
; 	add	hl, hl
; 	ld	bc,_reports
; 	add	hl, bc
; 	push	hl			; address of potential que'd next usb report

; 	call	_usb_kyb_buf_size
; 	ld	a, l
; 	ld	b, h
; 	ex	af, af'
; 	ld	a, b
; 	or	a
; 	pop	iy			; retrieve the next que'd usb_report address
; 	jr	z, no_queued_reports

; 	ld	a, (iy)
; 	ex	af, af'
; 	exx
; 	ld	b, (iy+2)
; 	ld	c, (iy+3)
; 	ld	d, (iy+4)
; 	ld	e, (iy+5)
; 	ld	h, (iy+6)
; 	ld	l, (iy+7)
; 	exx
; 	ret

; no_queued_reports:
; 	ex	af, af'
; 	exx
; 	ld	bc, 0
; 	ld	d, b
; 	ld	e, b
; 	ld	l, b
; 	ld	h, b
; 	exx
; 	ret



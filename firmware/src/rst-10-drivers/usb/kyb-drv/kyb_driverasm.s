
        INCLUDE "..\..\..\config.inc"
	INCLUDE "..\usb-constants.inc"

	SECTION CODE

	.assume adl=1

	XREF	_kyb_rpt_read_index
	XREF	_kyb_reports

	GLOBAL	_report_diff
	XREF	_kyb_report
	XREF	_kyb_previous

_report_diff:
	LD	DE, _kyb_report
	LD	HL, _kyb_previous
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

	SECTION CODE

; uint8_t usb_kyb_report(keyboard_report_t*)

	XREF	_kyb_rpt_read_index
	XREF	_kyb_reports
	XREF	_usb_kyb_rpt_que_size
	GLOBAL	_usb_kyb_report

_usb_kyb_report:
	push	hl			; keyboard_report_t*

	xor	a
	sbc	hl, hl
	ld	a, (_kyb_rpt_read_index)
	ld	l, a
	add	hl, hl
	add	hl, hl
	add	hl, hl
	ld	bc, _kyb_reports
	add	hl, bc
	push	hl			 ; address of potential que'd next usb report

	call	_usb_kyb_rpt_que_size
	or	a
	pop	hl			 ; retrieve the next que'd usb_report address
	jr	z, .no_queued_reports

	ld	bc, 8
	pop	de			; keyboard_report_t*
	ldir
	ret.l

.no_queued_reports:
	pop	hl
	ret.l

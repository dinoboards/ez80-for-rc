;
; CRT INTERFACE
;
; Provides a VDU, Keyboard (USB)
;
	include "..\config.inc"

	SECTION CODE

	XREF	_vm_vdp_init
	XREF	_vdu_init

	.assume adl=1
;
; CRT DISPATCH
; Dispatcher for the RST %10 trap functions
;
; Inputs:
;   B      = CRT sub function index
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;  Other registers as per sub-functions
;
	; must be called with RST.L
	PUBLIC	_crt_dispatch
_crt_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, crt_vdu				; B = 0, OUTPUT TO VDU STREAM
	DEC	A
	JR	Z, crt_ansi				; *B = 1, OUTPUT TO ANSI STREAM
	DEC	A
	JR	Z, crt_key_event			; *B = 2, returns next qued keyboard event payload (aka usb_kyb_event or uart in)
	DEC	A
	JR	Z, crt_vdp_fn				; *B = 3, INVOKE SPECIFIC vdp_... library functions, fn id in C, values on stack.  must not be invoked with rst.l

; *Not currently implemented - under consideration only for implementation


	; 255 V99X8/Super PROBE
	LD	A, B
	CP	255
	JR	Z, crt_v99x8_probe

	LD	A, 1					; UNKNOWN SUB FUnCTION
	OR	A
	RET.L

	; C is char to be sent
	; spare h, l, d, e
	XREF	_vdu
	PUBLIC	crt_vdu
crt_vdu:
	PUSH	BC
	CALL	_vdu
	LD	A, 9					; restore A and BC
	POP	BC
	RET.L

; // example of sending to vdu stream
; 	;VDU 24,x1;y1;x2;y2

; 	ld	a, 8
; 	ld	b, 0
; 	ld	c, 24
; 	rst	%10		; restore a and b to aid recall

; 	ld	c, low x1
; 	rst	%10

; 	ld	c, high x1
; 	rst	%10


crt_ansi:
crt_key_event:
crt_vdp_fn:
	RET.L

crt_v99x8_probe:
	CALL	_vm_vdp_init
	PUSH	AF
	CP	4
	CALL	Z, _vdu_init
	POP	AF
	RET.L

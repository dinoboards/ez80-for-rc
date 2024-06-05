
	SECTION CODE

	.assume adl=1

HOOK_CNT	EQU	4

	PUBLIC	_rst_rc2014_fnc
_rst_rc2014_fnc:
	PUSH	HL
	PUSH	BC

	CP	HOOK_CNT
	JR	NC, rst_rc2014_fnc_resume

	LD	BC, %000000
	SLA	A				; MULT BY 4
	SLA	A
	LD	C, A

	LD	HL, rc_functions		; ADD IT TO THE JUMP TABLE
	ADD	HL, BC

	JP	(HL)				; INVOKE THE HANDLER

rst_rc2014_fnc_resume:
	POP	BC
	POP	HL
	RET.L


rc_functions:
	JP	rcfn_platform_init
	JP	_fn_1
	JP	_fn_2
	JP	_fn_3

_fn_1:
_fn_2:
_fn_3:

; A = 0
; HL{15:0} POINTS TO A TABLE OF VARIOUS CONFIGURATION VALUES - EG IO PORT ADDRESSES FOR H/W SUCH AS MEMORY BANKING CONTROL
rcfn_platform_init:
	POP	BC
	POP	HL
	RET.L


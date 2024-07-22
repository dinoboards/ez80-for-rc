	include "config.inc"

	XREF	_system_utils_dispatch
	XREF	_rtc_dispatch
	XREF	_system_timer_dispatch
	XREF	uart_control

	SECTION CODE

	.assume adl=1

HOOK_CNT	EQU	4

; A = function index
; A = 0 -> UTIL-FUNC, B-> SUB FUNCTION
; A = 1 -> RTC-FUNC, B-> RTC SUB-FUNCTION
; A = 2 -> SYSTMR, B-> SUB-FUNCTION
; A = 3 -> UART-FUNC, B-> SUB-FUNCTION

	PUBLIC	_rst_10_functions
_rst_10_functions:
	PUSH	HL
	PUSH	BC

	CP	HOOK_CNT
	JR	NC, rst_rc2014_fnc_resume

	LD	BC, %000000
	SLA	A					; MULT BY 4
	SLA	A
	LD	C, A

	LD	HL, rc_functions			; ADD IT TO THE JUMP TABLE
	ADD	HL, BC

	JP	(HL)					; INVOKE THE HANDLER

rst_rc2014_fnc_resume:
	POP	BC
	POP	HL
	RET.L


rc_functions:
	JP	_system_utils_dispatch
	JP	_rtc_dispatch
	JP	_system_timer_dispatch
	JP	uart_control

_fn_1:
_fn_2:
_fn_3:

; A = 0
; HL{15:0} POINTS TO A TABLE OF VARIOUS CONFIGURATION VALUES - EG IO PORT ADDRESSES FOR H/W SUCH AS MEMORY BANKING CONTROL
rcfn_platform_init:
	POP	BC
	POP	HL
	RET.L


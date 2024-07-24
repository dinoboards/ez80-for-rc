	include "config.inc"

	XREF	_system_utils_dispatch
	XREF	_rtc_dispatch
	XREF	_system_timer_dispatch
	XREF	_uart_dispatch

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
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, _system_utils_dispatch		; A = 0, SYSUTL_xxx functions
	DEC	A
	JR	Z, _rtc_dispatch			; A = 1, RTC_xxx functions
	DEC	A
	JR	Z, _system_timer_dispatch		; A = 2, SYSTMR_xxx functions
	DEC	A
	JR	Z, _uart_dispatch			; A = 3, UART_xxx functions

	LD	A, %FF					; UNKNOWN UART FUNCTION
	RET.L

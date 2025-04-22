	include "config.inc"

	XREF	_system_utils_dispatch
	XREF	_rtc_dispatch
	XREF	_system_timer_dispatch
	XREF	_uart_dispatch
	XREF	_i2c_dispatch
	XREF	_usb_dispatch

	SECTION INTERNAL_RAM_ROM

	.assume adl=1

HOOK_CNT	EQU	4

; A = function index
; A = 0 -> UTIL-FUNC, B-> SUB FUNCTION
; A = 1 -> RTC-FUNC, B-> RTC SUB-FUNCTION
; A = 2 -> SYSTMR, B-> SUB-FUNCTION
; A = 3 -> UART-FUNC, B-> SUB-FUNCTION
; A = 4 -> I2C-FUNC, B-> SUB-FUNCTION
; A = 5 -> SPI-FUNC, B-> SUB-FUNCTION
; A = 6 -> RESERVED, B-> SUB-FUNCTION
; A = 7 -> ROM-FLASHING, B-> SUB-FUNCTION

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
	DEC	A
	JR	Z, _i2c_dispatch			; A = 4, I2C_xxx functions
	DEC	A
	JR	Z, _spi_dispatch			; A = 5, SPI_xxx functions
	DEC	A
	JR	Z, _usb_dispatch			; A = 6, RESERVED functions
	DEC	A
	JR	Z, _reserved_dispatch			; A = 7, RESERVED functions

_spi_dispatch:
_reserved_dispatch:
	LD	A, %FF					; UNKNOWN UART FUNCTION
	RET.L

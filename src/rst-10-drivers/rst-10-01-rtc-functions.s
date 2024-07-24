;
; RTC INTERFACE
;
; Access with RST.L %10 (A=1, B=sub function code)
;
; The RTC provides functions to access the Real Time Clock hardware.
;
; Function provide ability to set/get the current time, set/get alarms,
;
; The RTC is driven by a 32KHz crystal.
;
; Use the RTC_QUERY to determine if the RTC is powered and enabled.
;
	include "..\config.inc"

	XREF	_rtc_enabled

	SECTION CODE

	.assume adl=1
;
; RTC DISPATCH
; Dispatcher for the RST.L %10 trap functions
;
; Inputs:
;   B      = RTC sub function index
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;  Other registers as per sub-functions
;
	PUBLIC	_rtc_dispatch
_rtc_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, rtc_query				; B = 0, RTC_QUERY
	DEC	A
	JR	Z, rtc_time_get				; B = 1, RTC_TIME_GET
	DEC	A
	JR	Z, rtc_time_set				; B = 2, RTC_TIME_SET
	DEC	A
	JR	Z, rtc_alarm_get			; B = 3, RTC_ALARM_GET
	DEC	A
	JR	Z, rtc_alarm_set			; B = 4, RTC_ALARM_SET

	LD	A, 1					; UNKNOWN SUB FUCTION
	OR	A
	RET.L
;
; Date Time Structure
; -------------------
; CENTURY	DS	1
; YEAR		DS	1
; MONTH		DS	1
; DAY		DS	1
; HOUR		DS	1
; MIN		DS	1
; SECCOND	DS	1
;
;
; Function B = 0 -- RTC_INIT
; Initialize the RTC hardware
;
; Outputs:
; A	 = 0 -> Success, ERR_NOHW -> RTC hardware not present
;
rtc_query:
	LD	A, (_rtc_enabled)			; NZ IF RTC IS ENABLED
	OR	A
	JR	NZ, rtc_query_ok

	LD	A, ERR_NOHW
	OR	A
	RET.L

rtc_query_ok:
	XOR	A					; SUCCESS
	RET.L
;
; Function B = 3 -- RTC_ALARM_GET
; Not implemented yet
;
rtc_alarm_get:
;
; Function B = 4 -- RTC_ALARM_SET
; Not implemented yet
;
rtc_alarm_set:
	LD	A, 1					; NOT IMPLEMTNED YET
	OR	A
	RET.L
;
; Function B = 1 -- RTC_TIME_GET
; Retrieve the current RTC time
;
; Inputs:
;  MBASE:HL	= Address of Date Time Struct to receive the current date/time values
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;
rtc_time_get:
	IN0	A, (RTC_CEN)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_YR)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_MON)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_DOM)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_HRS)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_MIN)
	LD.S	(HL), A
	INC	HL

	IN0	A, (RTC_SEC)
	LD.S	(HL), A

	XOR	A					; SUCCESS
	RET.L
;
; Function B = 2 -- RTC_TIME_SET
;
; Inputs:
;  MBASE:HL	= Address of Date Time Struct containing the new date/time values
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;
rtc_time_set:
	LD	A, RTC_INT_DISABLE | RTC_BCD_ENABLE | RTC_CLK_32KHZ | RTC_UNLOCK
	OUT0	(RTC_CTRL), A

	LD.S	A, (HL)
	OUT0	(RTC_CEN), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_YR), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_MON), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_DOM), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_HRS), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_MIN), A
	INC	HL

	LD.S	A, (HL)
	OUT0	(RTC_SEC), A

	LD	A, RTC_INT_DISABLE | RTC_BCD_ENABLE | RTC_CLK_32KHZ | RTC_LOCK
	OUT0	(RTC_CTRL), A

	XOR	A					; SUCCESS
	RET.L

	include "config.inc"

	XREF	timer_tick_control

	SECTION CODE

	.assume adl=1

HOOK_CNT	EQU	4

; A = function index
; A = 0 -> SYSTEM INITIALISE, HL-> PLATFORM DESCRIPTION TABLE
; A = 1 -> RTC-FUNC, B-> RTC SUB-FUNCTION
; A = 2 -> 60HZ-TIMER-FUNC, B-> SUB-FUNCTION

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
	JP	rtc_control
	JP	timer_tick_control
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


; B = RTC SUB FUNCTION INDEX (STARTING FROM 0)
; RETURNS:
;  A -> ZERO SUCCESS
;  A -> NON-ZERO ERROR CODE

; RTCINIT	EQU	0	; GET TIME
; RTCGETTIM	EQU	1	; GET TIME
; RTCSETTIM	EQU	2	; SET TIME
; RTCGETBYT	EQU	3	; GET NVRAM BYTE BY INDEX
; RTCSETBYT	EQU	4	; SET NVRAM BYTE BY INDEX
; RTCGETBLK	EQU	5	; GET NVRAM DATA BLOCK
; RTCSETBLK	EQU	6	; SET NVRAM DATA BLOCK
; RTCGETALM	EQU	7	; GET ALARM
; RTCSETALM	EQU	8	; SET ALARM
; RTCDEVICE	EQU	9	; RTC DEVICE INFO REPORT

rtc_control:
	POP	BC		; restore bc and hl
	POP	HL

	LD	A, B		; SUB FUNCTION CODE
	OR	A		; TEST SUB FUNCTION CODE
	JR	Z, rtc_init
	DEC	A
	JR	Z, rtc_get_time
	DEC	A
	JR	Z, rtc_set_time
	DEC	A
	JR	Z, rtc_get_byte
	DEC	A
	JR	Z, rtc_set_byte
	DEC	A
	JR	Z, rtc_get_block
	DEC	A
	JR	Z, rtc_set_block
	DEC	A
	JR	Z, rtc_get_alarm
	DEC	A
	JR	Z, rtc_set_alarm


	LD	A, 1		; UNKNOWN SUB FUCTION
	OR	A
	RET.L


rtc_init:
	XOR	A		; SUCCESS
	RET.L

rtc_get_byte:
rtc_set_byte:
rtc_get_block:
rtc_set_block:
rtc_get_alarm:
rtc_set_alarm:
	LD	A, 1		; NOT IMPLEMTNED YET
	OR	A
	RET.L

	; hl[15:0] -> address[15:0] to write bcd date/time values
	; mbase -> address[23:16]
	; structure:
	; CENTURY	DS	1
	; YEAR		D	1
	; MONTH		D	1
	; DAY		D	1
	; HOUR		D	1
	; MIN		D	1
	; SECCOND	D	1
rtc_get_time:
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

	XOR	A		; SUCCESS
	RET.L

rtc_set_time:
	LD	A, RTC_ALARM_DISABLE | RTC_INT_DISABLE | RTC_BCD_ENABLE | RTC_CLK_32KHZ | RTC_SLP_WAKE_DISABLE | RTC_UNLOCK
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

	LD	A, RTC_ALARM_DISABLE | RTC_INT_DISABLE | RTC_BCD_ENABLE | RTC_CLK_32KHZ | RTC_SLP_WAKE_DISABLE | RTC_LOCK
	OUT0	(RTC_CTRL), A

	XOR	A		; SUCCESS
	RET.L

	include "..\config.inc"

	XREF	timer_tick_control

	SECTION CODE

	.assume adl=1

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

	PUBLIC	rtc_control
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

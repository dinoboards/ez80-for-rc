
        INCLUDE "..\config.inc"

	XREF	__idivu
	XREF	__iremu
	XREF	__imul_b
	XREF	_system_ticks
	XREF	_ticks_frequency
	XREF	_rtc_enabled
	XREF	_get_ticks_for_rtcclk
	XREF	_get_ticks_for_sysclk
	XREF	_configure_tmr1_to_rtc
	XREF	_configure_tmr1_to_sysclk

	SEGMENT	CODE

	.ASSUME ADL=1

	PUBLIC	_system_timer_isr

_system_timer_isr:
	PUSH	AF
	PUSH	HL

	IN0    A, (TMR1_CTL)				; CLEAR INTERRUPT SIGNAL

	LD	HL, (_system_ticks)			; POINT TO TICK COUNTER
	INC	HL
	LD	(_system_ticks), HL

	POP	HL
	POP	AF
	EI
	RETI.L

	PUBLIC	timer_tick_control
timer_tick_control:
	POP	BC					; RESTORE BC AND HL
	POP	HL

	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, tm_tick_get				; B = 0, GET TICKS
	DEC	A
	JR	Z, tmr_secs_get				; B = 1, GET SECS
	DEC	A
	JR	Z, tmr_tick_set				; B = 2, SET TICKS
	DEC	A
	JR	Z, tmr_secs_set				; B = 3, SET SECS
	DEC	A
	JR	Z, tmr_freq_get				; B = 4, GET FREQ
	DEC	A
	JR	Z, tmr_freq_set				; B = 5, SET FREQ

	XOR	A					; SUCCESS
	RET.L

;
; GET TIMER TICKS
;   RETURNS:
;     HL{23:0}:		TIMER VALUE (24 BIT)
;     E:HL{15:0}:	TIMER VALUE (24 BIT)
;     D:		0
;     C:		50 or 60 (_ticks_frequency)
;
tm_tick_get:
	DI
	LD	HL, (_system_ticks)
	LD	A, (_system_ticks+2)
	LD	D, 0
	LD	E, A
	EI
	LD	A, (_ticks_frequency)
	LD	C, A
	XOR	A					; SUCCESS
	RET.L
;
; GET TIMER SECONDS
;   RETURNS:
;     HL{23:0}:		TIMER VALUE (24 BIT)
;     C: 		NUM TICKS WITHIN CURRENT SECOND
;
tmr_secs_get:
	LD	HL, (_system_ticks)
	LD	BC, 0
	LD	A, (_ticks_frequency)
	LD	C, A
	PUSH	BC
	CALL	__idivu

	POP	BC					; RESTORE _ticks_frequency
	PUSH	HL					; SAVE SECONDS = _system_ticks/_ticks_frequency

	LD	HL,(_system_ticks)
	CALL	__iremu
	LD	C, L					; SECOND FRACTION INTO C

	POP	HL					; RESTORE SECONDS

	XOR	A					; SUCCESS
	RET.L
;
; SET TIMER
;   ON ENTRY:
;     HL{23:0}: TIMER VALUE (24 BIT)
;
tmr_tick_set:
	LD	(_system_ticks), HL

	XOR	A					; SUCCESS
	RET.L
;
; SET SECS
;   ON ENTRY:
;     HL{23:0}: SECONDS VALUE (24 BIT)
;
tmr_secs_set:
	LD	A, (_ticks_frequency)
	CALL	__imul_b

	LD	(_system_ticks), HL

	XOR	A					; SUCCESS
	RET.L
;
; GET TIMER FREQUENCY
;   RETURNS:
;
tmr_freq_get:
	LD	A, (_ticks_frequency)
	LD	C, A
	RET.L
;
; SET TIMER FREQUENCY
;  ON ENTRY:
;   	C: TIMER FREQUENCY
;
tmr_freq_set:
	LD	A, C
	LD	(_ticks_frequency), A

	LD	A, (_rtc_enabled)
	OR	A
	JR	Z, tmr_freq_set_for_cpuclk

tmr_freq_set_for_rtc:
	CALL	_get_ticks_for_rtcclk
	CALL	_configure_tmr1_to_rtc
	JR	tmr_freq_set_exit

tmr_freq_set_for_cpuclk:
	CALL	_get_ticks_for_sysclk
	CALL	_configure_tmr1_to_sysclk

tmr_freq_set_exit
	LD	HL, 0
	LD	(_system_ticks), HL
	XOR	A					; SUCCESS
	RET.L

	END

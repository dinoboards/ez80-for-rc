
        INCLUDE "..\config.inc"

	SEGMENT	CODE

	.ASSUME ADL=1

	PUBLIC	_ms_60Hz_timer_counter_isr

_ms_60Hz_timer_counter_isr:
	PUSH	AF
	PUSH	HL

	IN0    A, (TMR1_CTL)				; CLEAR INTERRUPT SIGNAL

	LD	HL, (_system_ticks)			; POINT TO TICK COUNTER
	INC	HL
	LD	(_system_ticks), HL

	LD	HL, _system_sub_second_count		; POINT TO SECONDS TICK COUNTER
	DEC	(HL)					; COUNTDOWN ONE SECOND OF TICKS
	JR	NZ, HB_TICK1				; NOT DONE, SKIP AHEAD
	LD	A, TICKFREQ				; TICKS PER SECOND
	LD	(HL), A					; RESET COUNTDOWN REGISTER
	LD	HL, (_system_second_ticks)		; POINT TO SECONDS COUNTER
	INC	HL					; INCREMENT AND RETURN
	LD	(_system_second_ticks), HL

HB_TICK1:
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

	XOR	A					; SUCCESS
	RET.L


; GET TIMER TICKS
;   RETURNS:
;     HL{23:0}:		TIMER VALUE (24 BIT)
;     E:HL{15:0}:	TIMER VALUE (24 BIT)
;     D:		0
;     C:		60 (TICKFREQ)
;
tm_tick_get:
	DI
	LD	HL, (_system_ticks)
	LD	A, (_system_ticks+2)
	LD	D, 0
	LD	E, A
	EI
	LD	C, TICKFREQ
	XOR	A					; SUCCESS
	RET.L
;
; GET TIMER SECONDS
;   RETURNS:
;     HL{23:0}:		TIMER VALUE (24 BIT)
;     E:HL{15:0}:	TIMER VALUE (24 BIT)
;     D:		0
;     C: 		NUM TICKS WITHIN CURRENT SECOND
;
tmr_secs_get:
	LD	D, 0
	DI
	LD	HL, (_system_second_ticks)
	LD	A, (_system_second_ticks+2)
	LD	E, A
	LD	A, (_system_sub_second_count)
	EI
	NEG						; CONVERT DOWNCOUNTER TO UPCOUNTER
	ADD	A, TICKFREQ
	LD	C, A
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
	LD	(_system_second_ticks), HL

	XOR	A					; SUCCESS
	RET.L


	SEGMENT	BSS

	PUBLIC	_system_sub_second_count
	PUBLIC	_system_ticks
	PUBLIC	_system_second_ticks

_system_ticks:
	DS	3

_system_sub_second_count:
	DS	1					; TICK COUNTER FOR FRACTIONAL SECONDS

_system_second_ticks:
	DS	3

	END

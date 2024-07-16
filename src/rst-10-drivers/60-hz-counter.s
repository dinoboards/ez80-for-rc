
        INCLUDE "..\config.inc"

	SEGMENT	CODE

	.ASSUME ADL=1

	PUBLIC	_ms_60Hz_timer_counter_isr

_ms_60Hz_timer_counter_isr:
	PUSH	AF
	PUSH	HL

	IN0    A, (TMR1_CTL)				; CLEAR INTERRUPT SIGNAL

	LD	HL, _system_ticks			; POINT TO TICK COUNTER
	CALL	INC32HL
	LD	HL, _system_sub_second_count		; POINT TO SECONDS TICK COUNTER
	DEC	(HL)					; COUNTDOWN ONE SECOND OF TICKS
	JR	NZ, HB_TICK1				; NOT DONE, SKIP AHEAD
	LD	A, TICKFREQ				; TICKS PER SECOND
	LD	(HL), A					; RESET COUNTDOWN REGISTER
	LD	HL, _system_second_ticks		; POINT TO SECONDS COUNTER
	CALL	INC32HL					; INCREMENT AND RETURN

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
;     DE{15:0}:HL{15:0}: TIMER VALUE (32 BIT)
;     C TICKFREQ (60)
;
tm_tick_get:
	LD	HL, _system_ticks
	DI
	CALL	LD32
	EI
	LD	C, TICKFREQ
	XOR	A					; SUCCESS
	RET.L
;
; GET TIMER SECONDS
;   RETURNS:
;     DE:HL: SECONDS VALUE (32 BIT)
;     C: NUM TICKS WITHIN CURRENT SECOND
;
tmr_secs_get:
	LD	HL, _system_second_ticks
	DI
	CALL	LD32
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
;     DE{15:0}:HL{15:0}: TIMER VALUE (32 BIT)
;
tmr_tick_set:
	LD	BC, _system_ticks
	DI
	CALL	ST32
	EI
	XOR	A					; SUCCESS
	RET.L

;
; SET SECS
;   ON ENTRY:
;     DE:HL: SECONDS VALUE (32 BIT)
;
tmr_secs_set:
	LD	BC, _system_second_ticks
	DI
	CALL	ST32
	EI
	XOR	A
	RET.L
;
; INC32 (HL)
; INCREMENT 32 BIT BINARY AT ADDRESS
;
INC32HL:
	INC	(HL)
	RET	NZ
	INC	HL
	INC	(HL)
	RET	NZ
	INC	HL
	INC	(HL)
	RET	NZ
	INC	HL
	INC	(HL)
	RET

; LOAD OR STORE DE:HL
;
LD32:
	; LD DE:HL, (HL)
	PUSH	AF
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	LD	H, (HL)
	LD	L, A
	POP	AF
	EX	DE, HL
	RET

ST32:
	; LD (BC),DE{15:0}:HL{15:0}
	PUSH	AF
	LD	A,L
	LD	(BC),A
	INC	BC
	LD	A,H
	LD	(BC),A
	INC	BC
	LD	A,E
	LD	(BC),A
	INC	BC
	LD	A,D
	LD	(BC),A
	POP	AF
	RET

	SEGMENT	BSS

	PUBLIC	_system_sub_second_count
	PUBLIC	_system_ticks

_system_ticks:
	DS	4

_system_sub_second_count:
	DS	1				; TICK COUNTER FOR FRACTIONAL SECONDS

_system_second_ticks:
	DS	4

	END

;
; SYSTEM TIMER INTERFACE
;
; Access with RST.L %10 (A=2, B=sub function code)
;
; The System Timer track a counter that is incremented at a fixed rate.
; The rate can be selected, and is typically 50 Hz or 60 Hz.
;
; The timer is driven by the RTC if it is enabled, otherwise it is driven
; by the CPU clock.
;
; The interface provides function to access and reset the tick counter,
; as well as access a calibrated seconds value.
;
; The tick count is a 24 bit number, and will wrap to 0 after 2^24 ticks.
;
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
	PUBLIC	_system_timer_dispatch
;
; SYSTEM TIMER ISR
;
; This ISR is called at the tick frequency, and increments the 24 bit tick counter.
;
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
;
; SYSTEM TIMER DISPATCH
;
; Dispatcher for the RST.L %10 trap functions
; Inputs:
;   B = TIMER SUB FUNCTION INDEX
; Outputs:
;   A = 0 -> SUCCESS, NON-ZERO -> ERROR
;   Other registers as per sub-functions
;
_system_timer_dispatch:
	POP	BC					; RESTORE BC AND HL
	POP	HL

	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, tm_tick_get				; B = 0, SYSTMR_GET_TICKS
	DEC	A
	JR	Z, tmr_secs_get				; B = 1, SYSTMR_GET_SECONDS
	DEC	A
	JR	Z, tmr_tick_set				; B = 2, SYSTMR_SET_TICKS
	DEC	A
	JR	Z, tmr_secs_set				; B = 3, SYSTMR_SET_SECONDS
	DEC	A
	JR	Z, tmr_freq_get				; B = 4, SYSTMR_GET_FREQTICK
	DEC	A
	JR	Z, tmr_freq_set				; B = 5, SYSTMR_SET_FREQTICK

	LD	A, %FF					; FAILURE - UNKONWN SUB FUNCTION
	RET.L
;
; Function B = 0 -- SYSTMR_GET_TICKS
; Retrieve the current 24 bit tick count.
;
; Output:
;   HL{23:0}	= TIMER VALUE (24 BIT)
;   E:HL{15:0}	= TIMER VALUE (24 BIT)
;   D		= 0
;   C		= tick frequency (typically 50 or 60)
;
tm_tick_get:
	DI
	LD	HL, (_system_ticks)
	LD	A, (_system_ticks+2)
	EI
	LD	D, 0
	LD	E, A
	LD	A, (_ticks_frequency)
	LD	C, A
	XOR	A					; SUCCESS
	RET.L
;
; Function B = 1 -- SYSTMR_GET_SECONDS
; Retrieve the current 24 bit number of seconds counted.
;
;   RETURNS:
;     HL{23:0}	= Number of seconds counted
;     C 	= Number of ticks within the current second
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
;  Set the on board system clock to track the desired frequency, typically
;  this will be 50Hz or 60Hz.
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

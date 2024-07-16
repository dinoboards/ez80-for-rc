        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_init_clocks
	XREF	_system_ticks
	XREF	_system_sub_second_count
	XREF	_system_second_ticks
	XREF	_assign_cpu_frequency
	XREF	_rtc_enabled

RTC_CLOCK_RATE EQU 32768

COUNT_FOR_60HZ_RTC EQU RTC_CLOCK_RATE / 60

COUNT_FOR_60HZ_SYSCLK EQU (CPU_CLK_FREQ/16)/60

_init_clocks:
	LD	A, TICKFREQ
	LD	(_system_sub_second_count), A

	LD	HL, 1
	CALL	configure_tmr1_to_rtc			; CONFIGURE TM1 FOR RTC CLOCK AS FULL SPEED

	EI						; SEE IF THE TIMER ISR IS TRIGGERED
	LD	HL, _system_ticks			; BY NOTING IF _system_ticks CHANGES
	LD	A, (HL)
	LD	B, 0
wait:
	NOP
	DJNZ	wait
	DI

	CP	(HL)
	JR	Z, no_rtc

	LD	HL, COUNT_FOR_60HZ_RTC
	CALL	configure_tmr1_to_rtc

	CALL	measure_cpu_freq

	LD	A, TICKFREQ				; RESET TICK COUNTERS
	LD	(_system_sub_second_count), A
	LD	HL, 0
	LD	(_system_ticks), HL
	LD	(_system_second_ticks), HL

	CPL						; RETURN NZ FOR RTC PRESENT
	LD	(_rtc_enabled), A
	RET

no_rtc:
	LD	HL, COUNT_FOR_60HZ_SYSCLK
	CALL	configure_tmr1_to_sysclk

	XOR	A					; RETURN 0 FOR NO RTC
	LD	(_rtc_enabled), A
	RET

configure_tmr1_to_rtc:
	LD	A, TMR0_IN_SYSCLK | TMR1_IN_RTC | TMR2_IN_SYSCLK | TMR3_IN_SYSCLK
	OUT0	(TMR_ISS), A

configure_tmr1:
	LD	A, L
	OUT0	(TMR1_RR_L), A
	LD	A, H
	OUT0	(TMR1_RR_H), A
	LD	A, TMR_ENABLED | TMR_CONTINUOUS | TMR_RST_EN | TMR_CLK_DIV_16 | TMR_IRQ_EN  ; CLK DIV NOT APPLIED IF BASED ON RTC CLOCK SOURCE
	OUT0	(TMR1_CTL), A

	RET

configure_tmr1_to_sysclk:
	LD	A, TMR0_IN_SYSCLK | TMR1_IN_SYSCLK | TMR2_IN_SYSCLK | TMR3_IN_SYSCLK
	OUT0	(TMR_ISS), A

	JR	configure_tmr1


measure_cpu_freq:
	EI
	; // use timer ticks (60Hz) to evaluate the current CPU frequency
repeat:
	LD	HL, _system_ticks

	LD	DE, 0
	LD	BC, 0

	LD	A, (HL)
sync_tick:
	CP	(HL)					; SYNC TO NEXT TICK EDGE
	JR	Z, sync_tick

	LD	A, (HL)

cycle_counter:
	INC	DE
	CP	(HL)					; SYNC TO NEXT TICK EDGE
	JR	Z, cycle_counter

skip:
	PUSH	DE
	CALL	_assign_cpu_frequency
	POP	DE

	DI
	RET

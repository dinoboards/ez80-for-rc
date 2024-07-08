
        INCLUDE ".\config.inc"

	SEGMENT	CODE

	.ASSUME ADL=1

	PUBLIC	ms_60Hz_timer_counter_isr

ms_60Hz_timer_counter_isr:
	PUSH	AF
	PUSH	HL

	IN0    A, (TMR1_CTL)			; clear interrupt signal

	LD	HL, (counter_60hz)
	INC	HL
	LD	(counter_60hz), HL

	POP	HL
	POP	AF
	EI
	RETI.L

	SEGMENT	BSS

counter_60hz:
	DS	3

	END

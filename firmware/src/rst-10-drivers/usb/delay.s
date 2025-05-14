        INCLUDE "..\..\config.inc"
	SECTION CODE
	.assume adl=1

	GLOBAL	_delay_20ms
_delay_20ms:
	LD	DE, 1250
_delay_x:
	EZ80_DELAY
	EZ80_DELAY
	EZ80_DELAY
	DEC	DE
	LD	A,D
	OR	E
	JR	NZ, _delay_x
	RET
;
; DELAY approx 60ms
	GLOBAL	_delay_short
_delay_short:
	LD	DE, 3750
	JR	_delay_x
;
; DELAY approx 1/2 second
	GLOBAL	_delay_medium
_delay_medium:
	PUSH	AF
	PUSH	DE
	LD	DE,31250
	CALL	_delay_x
	POP	DE
	POP	AF
	RET

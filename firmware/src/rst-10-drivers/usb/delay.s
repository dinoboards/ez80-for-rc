        INCLUDE "..\..\config.inc"
	SECTION CODE
	.assume adl=1

	GLOBAL	_delay
_delay:
	push	af
	call	DELAY
	pop	af
	ret

	GLOBAL	_delay_20ms
_delay_20ms:
	LD	DE, 1250
	JP	VDELAY
;
; DELAY approx 60ms
	GLOBAL	_delay_short
_delay_short:
	LD	DE, 3750
	JP	VDELAY
;
; DELAY approx 1/2 second
	GLOBAL	_delay_medium
_delay_medium	.EQU	LDELAY

EZ80_DELAY: MACRO
	RST.L %18
ENDMAC EZ80_DELAY

DELAY:
	EZ80_DELAY
	EZ80_DELAY
	EZ80_DELAY
	RET

VDELAY:
	EZ80_DELAY
	EZ80_DELAY
	EZ80_DELAY
	DEC	DE
	LD	A,D
	OR	E
	JR	NZ, VDELAY
	RET

LDELAY:
	PUSH	AF
	PUSH	DE
	LD	DE,31250
	CALL	VDELAY
	POP	DE
	POP	AF
	RET

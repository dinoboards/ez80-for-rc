SECTION code_compiler

	PUBLIC	_EZ80_CPY_MBHL_TO_UHL

_EZ80_CPY_MBHL_TO_UHL:
	PUSH	IX
	PUSH	AF
	DB	$5B, $DD, $21, $00, $00, $00		; LD.LIL	IX, 0
	DB	$49, $DD, $39				; ADD.L		IX, SP
	DB	$49, $E5				; PUSH.L	HL
	DB	$ED, $6E				; LD		A, MB
	DB	$5B, $DD, $77, $FF			; LD.LIL	(IX-1), A
	DB	$49, $E1				; POP.L		HL
	POP	AF
	POP	IX
	RET

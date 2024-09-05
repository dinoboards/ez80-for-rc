	SECTION code_compiler

	PUBLIC	_EZ80_CPY_UHL_TO_EHL

_EZ80_CPY_UHL_TO_EHL:
	PUSH	IX
	DB	$5B, $DD, $21, $00, $00, $00		; LD.LIL	IX, 0
	DB	$49, $DD, $39				; ADD.L		IX, SP
	DB	$49, $E5				; PUSH.L	HL
	DB	$5B, $DD, $5E, $FF			; LD.LIL	E, (IX-1)
	DB	$49, $E1				; POP.L		HL
	POP	IX
	RET

SECTION code_compiler

	PUBLIC	_EZ80_CPY_MBDE_TO_UDE

_EZ80_CPY_MBDE_TO_UDE:
	PUSH	IX
	PUSH	AF
	DB	$5B, $DD, $21, $00, $00, $00		; LD.LIL	IX, 0
	DB	$49, $DD, $39				; ADD.L		IX, SP
	DB	$49, $D5				; PUSH.L	DE
	DB	$ED, $6E				; LD		A, MB
	DB	$5B, $DD, $77, $FF			; LD.LIL	(IX-1), A
	DB	$49, $D1				; POP.L		DE
	POP	AF
	POP	IX
	RET

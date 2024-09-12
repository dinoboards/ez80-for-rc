	SECTION code_compiler

	PUBLIC	_EZ80_CPY_EHL_TO_UHL

_EZ80_CPY_EHL_TO_UHL:
	PUSH	IX
	PUSH	AF
	DB	$5B, $DD, $21, $00, $00, $00			; LD.LIL	IX, 0
	DB	$49, $DD, $39					; ADD.L		IX, SP
	DB	$49, $E5					; PUSH.L	HL
	DB	$5B, $DD, $73, $FF				; LD.LIL	(IX-1), E
	DB	$49, $E1					; POP.L		HL
	POP	AF
	POP	IX
	RET




	SECTION code_compiler

	PUBLIC	_sysget_tick
; extern uint32_t sysget_tick(void);
_sysget_tick:
	LD	BC, $F8D0
	RST	$08
	RET

	PUBLIC	_sysget_tick_rate
; extern uint8_t sysget_tick_rate(void);
_sysget_tick_rate:
	LD	BC, $F8D0
	RST	$08
	LD	A, C
	LD	L, C
	RET



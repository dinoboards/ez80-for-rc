	include "../ez80-instr.inc"

	SECTION code_compiler


	PUBLIC	_ez80_bus_cycles_get
; extern uint16_t ez80_bus_cycles_get(void);
_ez80_bus_cycles_get:
	EZ80_UTIL_GET_BUSTM
	RET

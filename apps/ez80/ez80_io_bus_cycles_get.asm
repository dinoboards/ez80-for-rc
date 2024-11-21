	include "ez80-instr.inc"

	.assume	adl=1

	section	.text,"ax",@progbits
	.global	_ez80_io_bus_cycles_get

; extern uint8_t ez80_io_bus_cycles_get(void);
_ez80_io_bus_cycles_get:
	EZ80_UTIL_IOTM_GET
	LD	A, L
	RET

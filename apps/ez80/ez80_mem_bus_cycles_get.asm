	include	"ez80-instr.inc"

	.assume	adl=1

	section	.text, "ax", @progbits
	.global	_ez80_mem_bus_cycles_get

; extern uint8_t ez80_mem_bus_cycles_get(void);
_ez80_mem_bus_cycles_get:
	push	ix
	EZ80_UTIL_MEMTM_GET
	ld	a, l
	pop	ix
	ret

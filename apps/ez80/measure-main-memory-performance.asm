	section	.text_on_chip, "ax", @progbits
	.assume	adl=1

	global	_measure_memory_performance
	extern	_ez80_timers_ticks_get

.macro	EZ80_FN
	RST.L	$10
.endm

.macro EZ80_UTIL_TMR_TICK_GET
	ld	a, 2
	ld	b, 0
	EZ80_FN
.endm
; ;
; ; int24_t measure_main_memory_performance();
; ;
; _measure_main_memory_performance:
; 	EZ80_UTIL_TMR_TICK_GET
; 	push	hl
; 	call	conduct_main_tests
; 	EZ80_UTIL_TMR_TICK_GET

; 	pop	de
; 	xor	a
; 	sbc	hl, de
; 	ret

; conduct_main_tests:
; 	LD	bc, 1024
; .loop:
; 	push	bc

; 	ld	hl, .main_memory_buffer
; 	ld	de, .main_memory_buffer+1
; 	ld	bc, 4096
; 	ldir

; 	pop	bc
; 	dec	bc
; 	ld	a, b
; 	or	c
; 	jr	nz, .loop
; 	ret

; int24_t measure_memory_performance(uint8_t* ptr);

_measure_memory_performance:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy+3)
	ld	(.ptr_start+1), hl
	inc	hl
	ld	(.ptr_next+1), hl

	DI
	EZ80_UTIL_TMR_TICK_GET
	push	hl
	call	conduct_cs0_tests
	EZ80_UTIL_TMR_TICK_GET
	EI

	pop	de
	xor	a
	sbc	hl, de
	ret

conduct_cs0_tests:
	LD	bc, 1024
.loop2:
	push	bc

.ptr_start:
	ld	hl, _main_memory_buffer
.ptr_next:
	ld	de, _main_memory_buffer+1
	ld	bc, 4096
	ldir

	pop	bc
	dec	bc
	ld	a, b
	or	c
	jr	nz, .loop2
	ret

	section	.bss_z80,"aw",@progbits
	global	_main_memory_buffer
_main_memory_buffer:
	ds	4097

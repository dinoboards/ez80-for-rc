
	section	.text,"ax",@progbits
	.global	_asm_refresh_get_angl
	.extern _pixx
	.extern _pixelangle
	.extern _midangle
	.assume	adl = 1

; short asm_refresh_get_angl() { return midangle + pixelangle[pixx]; }

_asm_refresh_get_angl:
	; calculate pixelangle[pixx]

	ld	hl, (_pixx)		; retrieve 16bit value pixx
	ld	de, 0

	sla	l			; mlt by 2
	rl	h
	ld	e, l
	ld	d, h			; de = (short) pixx*2

	ld	hl, _pixelangle
	add	hl, de			; add it to _pixelangle array

	ld	hl, (hl)		; hl = pixelangle[pixx]

	ld	de, (_midangle)		; retrieve 16bit value _midangle

	add	hl, de			; hl += midangle

	ret


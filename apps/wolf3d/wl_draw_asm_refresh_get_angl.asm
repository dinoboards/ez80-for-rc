
	section	.text, "ax", @progbits
	.global	_asm_refresh_get_angl
	.global _asm_refresh_find_quarter
	.extern	_pixx
	.extern	_pixelangle
	.extern	_midangle
	.assume	adl=1

FINEANGLES
	equ	3600

; short asm_refresh_get_angl() {
;   short angl = midangle + pixelangle[pixx];
;
;   if (angl < 0)
;     angl += FINEANGLES;
;
;   if (angl >= 3600)
;     angl -= FINEANGLES;
;
;   return angl;
; }

_asm_refresh_get_angl:
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

	; make sure angl >=0 and < 3600 (FINEANGLES)

	bit	7, h			; is angle neg?
	jr	z, is_not_neg

	ld	bc, FINEANGLES
	add	hl, bc
	ret

is_not_neg:
	ld	de, FINEANGLES+1	; Load 3601 into DE
	or	a			; Clear the carry flag
	sbc.sis	hl, de
	jr	c, hl_was_less_than_fine_angles  ; Jump if HL was < 3600

	; HL was > 3600
	inc	hl			; remove the off by one test
	ret

hl_was_less_than_fine_angles:
	ld	de, FINEANGLES+1	; restore HL's original value
	add	hl, de
	ret

_asm_refresh_find_quarter:
	ld	hl, (_angl)

	or	a
	ld	de, 900
	sbc.sis	hl, de

	jr	c, less_than_90

	sbc.sis	hl, de
	jr	c, less_than_180

	sbc.sis	hl, de
	jr	c, less_than_270

	ld	a, 3
	ret

less_than_90:
	xor	a
	ret

less_than_180:
	ld	a, 1
	ret

less_than_270:
	ld	a, 2
	ret
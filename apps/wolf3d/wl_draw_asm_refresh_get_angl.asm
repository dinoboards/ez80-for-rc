
	section	.text_on_chip, "ax", @progbits
	.global	_asm_refresh_get_angl
	.global	_asm_refresh_find_quarter
	.extern	_pixx
	.extern	_pixelangle
	.extern	_midangle
	.assume	adl=1

; Multiplies HL by BC and returns the 16-bit product hl.
; corrupts DE
.macro	MUL_16_HL_BC
	ld	d, h
	ld	e, c
	mlt	de
	ld	d, l
	ld	h, b
	mlt	hl
	add	hl, de
	ld	h, l
	ld	l, 0
	ld	e, c
	mlt	de
	add	hl, de
.endm

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

; uint8_t scale_post_asm()
	.global	_scale_post_asm
	.extern	_wallheight
	.extern	_view_width

	.extern	_drawing_params
	.extern	_drpm_yd
	.extern	_drpm_yoffs
	.extern	_drpm_ywcount
	.extern	_drpm_postx
	.extern	_drpm_view_height
	.extern	_drpm_view_width

TEXTURESIZE
	equ	64
TEXTURE_SIZE_HALF
	equ	TEXTURESIZE/2

.yoffs_less_than_zero:
	ld	hl, (iy+_drpm_postx)
	jr	store_yoffs

.set_yd_to_100:
	ld	a, 100
	ld	(iy+_drpm_yd), a
	xor	a
	ld	(iy+_drpm_yd+1), a
	jr	.assign_ywcount_to_de

_scale_post_asm:
	ld	iy, _drawing_params

	ld	hl, (iy+_drpm_postx)		; retrieve 16 bit value postx
	ld	de, 0

	sla	l			; mlt by 2
	rl	h
	ld	e, l
	ld	d, h			; de = (short) postx*2

	ld	hl, _wallheight
	add	hl, de			; add it to wallheight array

	ld	hl, (hl)		; de = wallheight[postx]

	srl	h			; hl =>> 1
	rr	l

	srl	h			; hl =>> 1
	rr	l

	srl	h			; hl =>> 1
	rr	l

	ld	de, 0
	ld	e, l
	ld	d, h

	; if de <= 0 then 100
	ld	a, e
	or	d
	jr	z, .set_yd_to_100

	bit	7, d
	jr	nz, .set_yd_to_100

	ld	(iy+_drpm_yd), e
	ld	(iy+_drpm_yd+1), d

.assign_ywcount_to_de:
	ld	(iy+_drpm_ywcount), e
	ld	(iy+_drpm_ywcount+1), d

	ld	hl, (iy+_drpm_view_half_height)

	or	a
	sbc.sis	hl, de			; hl -= ywcount

	ld	bc, (iy+_drpm_view_width)

	MUL_16_HL_BC

	bit	7, h
	jr	nz, .yoffs_less_than_zero

	ld	de, (iy+_drpm_postx)
	add	hl, de

store_yoffs:
	ld	(iy+_drpm_yoffs), l
	ld	(iy+_drpm_yoffs+1), h

	; drawing_params.yendoffs = drawing_params.view_half_height + drawing_params.ywcount - 1;
	ld	hl, (iy+_drpm_view_half_height)
	ld	de, (iy+_drpm_ywcount)
	add	hl, de
	dec	hl
	ld	(iy+_drpm_yendoffs), l
	ld	(iy+_drpm_yendoffs+1), h

	ld	(iy+_drpm_yw), (TEXTURESIZE-1)
	ld	(iy+_drpm_yw+1), 0

	ld	hl, (iy+_drpm_yendoffs)		; hl yendoffs
	ld	de, (iy+_drpm_view_height)	; de view_height

	ld	bc, 0
	ld	c, (iy+_drpm_yw)		; bc yw
	ld	b, (iy+_drpm_yw+1)		; ubC <= 0

	exx					; switch to alt regs
	ld	hl, (iy+_drpm_ywcount)		; hl' ywcount
	ld	de, TEXTURE_SIZE_HALF		; de' TEXTURESIZE / 2
	ld	bc, (iy+_drpm_yd)		; bc' yd
	exx					; switch to main regs

	;  while (drawing_params.yendoffs >= drawing_params.view_height)
outer_loop:
	or	a
	sbc.sis	hl, de				; yendoffs -= view_height
	jr	c, outer_loop_exit
	add	hl, de				; yendoffs += view_height
	dec	hl				; yendoffs--

	exx					; switch to alt regs
	or	a
	sbc.sis	hl, de				; ywcount -= TEXTURESIZE / 2
	exx					; switch to main

inner_loop:
	exx					; switch to alt regs

	; while (drawing_params.ywcount <= 0)
	bit	7, h
	jr	nz, inner_loop_continue
	ld	a, h
	or	l
	jr	z, inner_loop_continue

	exx					; switch to main regs
	jr	outer_loop

inner_loop_continue:
	add	hl, bc				; ywcount + yd
	exx					; switch to main regs
	dec	bc				; yw--
	jr	inner_loop

outer_loop_exit:
	add	hl, de				; yendoffs += view_height
	ld	(iy+_drpm_yendoffs), l
	ld	(iy+_drpm_yendoffs+1), h
	ld	(iy+_drpm_yw), c
	ld	(iy+_drpm_yw+1), b

	exx					; switch to alt regs
	ld	(iy+_drpm_ywcount), l
	ld	(iy+_drpm_ywcount+1), h

	; if (drawing_params.yw < 0) return
	exx					; switch to main
	bit	7, b
	ret	nz

	; result = drawing_params.postsource[drawing_params.yw];
	ld	hl, (iy+_drpm_postsource)
	add	hl, bc
	ld	a, (hl)				; result


	; ;  drawing_params.yendoffs = drawing_params.yendoffs * drawing_params.view_width + drawing_params.postx;
	ld	hl, (iy+_drpm_yendoffs)		; reload hl yendoffs
	ld	bc, (iy+_drpm_view_width)
	MUL_16_HL_BC
	ld	de, (iy+_drpm_postx)
	add.sis	hl, de
	ld	(iy+_drpm_yendoffs), l
	ld	(iy+_drpm_yendoffs+1), h

	ret



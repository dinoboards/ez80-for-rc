
	include "macros.inc"

	section	.text, "ax", @progbits
	global	_asm_refresh_get_angl
	.extern	_pixx
	.extern	_pixelangle
	.extern	_midangle
	.assume	adl=1

mapshift 	equ	6
FINEANGLES	equ	3600

; uint8_t scale_post_asm()
	global	_scale_post_asm
	.extern	_wallheight
	.extern	_view_width

	.extern	_drawing_params
	.extern	_drpm_yd
	.extern	_drpm_yoffs
	.extern	_drpm_ywcount
	.extern	_drpm_postx
	.extern	_drpm_view_height
	.extern	_drpm_view_width

TEXTURESIZE		equ	64
TEXTURE_SIZE_HALF	equ	TEXTURESIZE/2

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

	global	_set_player_in_pushwall_back_tile
_set_player_in_pushwall_back_tile:
	ld	iy, _draw_state
	xor	a						; player_in_pushwall_back_tile = tilemap[focaltx][focalty] == 64;
	ld	(iy+PLAYER_IN_PUSHWALL_BACK_TILE), a
	sbc	hl, hl
	push	hl
	ld	l, (iy+FOCAL_TX)
	ld	h, (iy+FOCAL_TX+1)
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	pop	de		; zero ude
	ld	e, (iy+FOCAL_TY)
	ld	d, (iy+FOCAL_TY+1)
	add	hl, de
	ld	de, _tilemap
	add	hl, de
	ld	a,(hl)
	cp	64
	ret	nz
	ld	a, 1
	ld	(iy+PLAYER_IN_PUSHWALL_BACK_TILE), a
	ret

	global	_asm_init_quarter
_asm_init_quarter:


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
	jr	.init_loop_params

is_not_neg:
	ld	de, FINEANGLES+1	; Load 3601 into DE
	or	a			; Clear the carry flag
	sbc.sis	hl, de
	jr	c, hl_was_less_than_fine_angles  ; Jump if HL was < 3600

	; HL was > 3600
	inc	hl			; remove the off by one test
	jr	.init_loop_params

hl_was_less_than_fine_angles:
	ld	de, FINEANGLES+1	; restore HL's original value
	add	hl, de

.init_loop_params:
	ld	iy, _draw_state
	ld	(iy+ANGL), l
	ld	(iy+ANGL+1), h

	or	a
	ld	de, 900
	sbc.sis	hl, de

	jr	c, _start_quarter_0_90

	sbc.sis	hl, de
	jr	c, _start_quarter_90_180

	sbc.sis	hl, de
	jp	c, _start_quarter_180_270

	jp	_start_quarter_270_360

;
; extern void start_quarter_0_90();
;
	; global	_start_quarter_0_90

;       xtilestep = 1;
;       ytilestep = -1;
;       xstep     = finetangent[900 - 1 - angl];
;       ystep     = -finetangent[angl];
;       xpartial  = xpartialup;
;       ypartial  = ypartialdown;

_start_quarter_0_90:
	ld	(iy+X_TILE_STEP), 1		; xtilestep = 1;

	ld	(iy+Y_TILE_STEP), 255		; ytilestep = -1;

	ld	hl, (iy+X_PARTIAL_UP)		; xpartial = xpartialup;
	ld	(iy+X_PARTIAL), hl

	ld	hl, (iy+Y_PARTIAL_DOWN)		; ypartial = ypartialdown;
	ld	(iy+Y_PARTIAL), hl

	xor	a				; ystep = -finetangent[angl];
	sbc	hl, hl
	ld	l, (iy+ANGL)
	ld	h, (iy+ANGL+1)
	push	hl				; save angl
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)				; eubc = finetangent[angl]
	LNEG_EUBC_AUHL				; auhl = -eubc
	ld	(iy+Y_STEP), hl
	ld	(iy+Y_STEP+3), a

	pop	de				; xstep = finetangent[900 - 1 - angl];
	ld	hl, 900-1
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	a, (hl)
	ld	(iy+X_STEP), bc
	ld	(iy+X_STEP+3), a

	jp	.init_loop_params2

;       xtilestep = -1;
;       ytilestep = -1;
;       xstep     = -finetangent[angl - 900];
;       ystep     = -finetangent[1800 - 1 - angl];
;       xpartial  = xpartialdown;
;       ypartial  = ypartialdown;

	; global	_start_quarter_90_180
_start_quarter_90_180:
	ld	(iy+X_TILE_STEP), 255		; xtilestep = -1;

	ld	(iy+Y_TILE_STEP), 255		; ytilestep = -1;

	ld	hl, (iy+X_PARTIAL_DOWN)		; xpartial = xpartialdown;
	ld	(iy+X_PARTIAL), hl

	ld	hl, (iy+Y_PARTIAL_DOWN)		; ypartial = ypartialdown;
	ld	(iy+Y_PARTIAL), hl

	xor	a				; xstep = -finetangent[angl - 900];
	sbc	hl, hl
	ld	l, (iy+ANGL)
	ld	h, (iy+ANGL+1)
	push	hl				; save angl
	ld	de, 900
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)				; eubc = finetangent[angl - 900]
	LNEG_EUBC_AUHL				; auhl = -eubc
	ld	(iy+X_STEP), hl
	ld	(iy+X_STEP+3), a

	pop	de				; ystep = -finetangent[1800 - 1 - angl];
	ld	hl, 1800-1
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)
	LNEG_EUBC_AUHL				; auhl = -eubc
	ld	(iy+Y_STEP), hl
	ld	(iy+Y_STEP+3), a

	jp	.init_loop_params2

;       xtilestep = -1;
;       ytilestep = 1;
;       xstep     = -finetangent[2700 - 1 - angl];
;       ystep     = finetangent[angl - 1800];
;       xpartial  = xpartialdown;
;       ypartial  = ypartialup;

	; global	_start_quarter_180_270
_start_quarter_180_270:
	ld	(iy+X_TILE_STEP), 255		; xtilestep = -1;

	ld	(iy+Y_TILE_STEP), 1		; ytilestep = 1;

	ld	hl, (iy+X_PARTIAL_DOWN)		; xpartial = xpartialdown;
	ld	(iy+X_PARTIAL), hl

	ld	hl, (iy+Y_PARTIAL_UP)		; ypartial = ypartialup;
	ld	(iy+Y_PARTIAL), hl

	xor	a				; ystep = finetangent[angl - 1800];
	sbc	hl, hl
	ld	l, (iy+ANGL)
	ld	h, (iy+ANGL+1)
	push	hl				; save angl
	ld	de, 1800
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)				; eubc = finetangent[angl - 1800]
	ld	(iy+Y_STEP), bc
	ld	(iy+Y_STEP+3), e

	pop	de				; xstep = -finetangent[2700 - 1 - angl];
	ld	hl, 2700-1
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)
	LNEG_EUBC_AUHL				; auhl = -eubc
	ld	(iy+X_STEP), hl
	ld	(iy+X_STEP+3), a

	jr	.init_loop_params2

;       xtilestep = 1;
;       ytilestep = 1;
;       xstep     = finetangent[angl - 2700];
;       ystep     = finetangent[3600 - 1 - angl];
;       xpartial  = xpartialup;
;       ypartial  = ypartialup;

	; global	_start_quarter_270_360
_start_quarter_270_360:
	ld	(iy+X_TILE_STEP), 1		; xtilestep = 1;

	ld	(iy+Y_TILE_STEP), 1		; ytilestep = 1;

	ld	hl, (iy+X_PARTIAL_UP)		; xpartial = xpartialup;
	ld	(iy+X_PARTIAL), hl

	ld	hl, (iy+Y_PARTIAL_UP)		; ypartial = ypartialup;
	ld	(iy+Y_PARTIAL), hl

	xor	a				; xstep = finetangent[angl - 2700];
	sbc	hl, hl
	ld	l, (iy+ANGL)
	ld	h, (iy+ANGL+1)
	push	hl				; save angl
	ld	de, 2700
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)				; eubc = finetangent[angl - 2700]
	ld	(iy+X_STEP), bc
	ld	(iy+X_STEP+3), e

	pop	de				; ystep = finetangent[3600 - 1 - angl];
	ld	hl, 3600-1
	xor	a
	sbc	hl, de
	add	hl, hl
	add	hl, hl
	ld	de, _finetangent
	add	hl, de
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	e, (hl)
	ld	(iy+Y_STEP), bc
	ld	(iy+Y_STEP+3), e

.init_loop_params2:
	; xpartial is a uint24_t
	; convert to int32 and shift right by 8
	xor	a				; yintercept = FixedMul(ystep, xpartial) + viewy;
	sbc	hl, hl
	ld	e, l
	ld 	l, (iy+X_PARTIAL+1)	; >> 8
	ld 	h, (iy+X_PARTIAL+2)
	ld	bc, (iy+Y_STEP+1)	; >> 8
	ld	a, (iy+Y_STEP+3)	; extend sign bit
	rlc	a
	sbc	a, a
	push	iy
	call	_mul_euhl_aubc
	pop	iy
	ld	bc, (iy+VIEW_Y)
	ld	a, (iy+VIEW_Y+3)
	LADD_EUHL_AUBC
	ld	(iy+Y_INTERCEPT), hl
	ld	(iy+Y_INTERCEPT+3), e

	ld	a, (iy+X_TILE_STEP)		;  xtile = focaltx + xtilestep;
	ld	e, a
	rlc	a
	sbc	a, a
	ld	d, a
	ld	l, (iy+FOCAL_TX)
	ld	h, (iy+FOCAL_TX+1)
	add	hl, de
	ld	(iy+X_TILE), l
	ld	(iy+X_TILE+1), h

	add	hl, hl				; xspot  = (word)((xtile << mapshift) + yintercept_as_short);
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	ld	e, (iy+Y_INTERCEPT+2)
	ld	d, (iy+Y_INTERCEPT+3)
	add	hl, de
	ld	(iy+X_SPOT), l
	ld	(iy+X_SPOT+1), h

	xor	a				; xintercept = FixedMul(xstep, ypartial) + viewx;
	sbc	hl, hl
	ld	e, l
	ld 	l, (iy+Y_PARTIAL+1)	; >> 8
	ld 	h, (iy+Y_PARTIAL+2)
	ld	bc, (iy+X_STEP+1)	; >> 8
	ld	a, (iy+X_STEP+3)	; extend sign bit
	rlc	a
	sbc	a, a
	push	iy
	call	_mul_euhl_aubc
	pop	iy
	ld	bc, (iy+VIEW_X)
	ld	a, (iy+VIEW_X+3)
	LADD_EUHL_AUBC
	ld	(iy+X_INTERCEPT), hl
	ld	(iy+X_INTERCEPT+3), e

	ld	a, (iy+Y_TILE_STEP)		; ytile = focalty + ytilestep;
	ld	e, a
	rlc	a
	sbc	a, a
	ld	d, a
	ld	l, (iy+FOCAL_TY)
	ld	h, (iy+FOCAL_TY+1)
	add	hl, de
	ld	(iy+Y_TILE), l
	ld	(iy+Y_TILE+1), h

	ld	l, (iy+X_INTERCEPT+2)		; yspot = (word)((xintercept_as_short << mapshift) + ytile);
	ld	h, (iy+X_INTERCEPT+3)
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	ld	e, (iy+Y_TILE)
	ld	d, (iy+Y_TILE+1)
	add	hl, de
	ld	(iy+Y_SPOT), l
	ld	(iy+Y_SPOT+1), h

	or	a, a				; texdelta = 0;
	sbc	hl, hl
	ld	(iy+TEX_DELTA), hl

	ret


	global	_is_horiz_entry
; extern uint8_t is_horiz_entry()
_is_horiz_entry:
	ld	iy, _draw_state			; ytilestep == -1 && yintercept_as_short <= ytile

	ld	a, (iy+Y_TILE_STEP)
	cp	255
	jr	nz, .y_tile_step_not_neg

	ld	e, (iy+Y_INTERCEPT+2)
	ld	d, (iy+Y_INTERCEPT+3)
	ld	l, (iy+Y_TILE)
	ld	h, (iy+Y_TILE+1)

	; de <= hl => true
	; de > hl => false
	compare_16bit_signed				; hl = ytile - yintercept_as_short
	jp 	p, .y_tile_step_not_neg			; Jump if yintercept_as_short > ytile

.ret_true:
	ld	a, 1	; return TRUE
	ret

.y_tile_step_not_neg:
	ld	a, (iy+Y_TILE_STEP)
	cp	1
	jr	nz, .ret_false

	ld	l, (iy+Y_INTERCEPT+2)		; (ytilestep == 1 && ytile <= yintercept_as_short )
	ld	h, (iy+Y_INTERCEPT+3)
	ld	e, (iy+Y_TILE)
	ld	d, (iy+Y_TILE+1)
	compare_16bit_signed			; hl = yintercept_as_short - ytile
	jp	M, .ret_true			; Jump if ytile(de) <= yintercept_as_short(hl)

.ret_false:
	xor	a	; return FALSE
	ret

	section	.data_on_chip, "aw", @progbits

; extern int8_t xtilestep, ytilestep;
	global	_xtilestep
_draw_state:
X_TILE_STEP	equ	0
_xtilestep:
	ds	1

	global	_ytilestep
Y_TILE_STEP	equ	(_ytilestep-_draw_state)
_ytilestep:
	ds	1

; extern uint24_t xpartialup, xpartialdown, ypartialup, ypartialdown;
	global	_xpartialup
X_PARTIAL_UP	equ	(_xpartialup-_draw_state)
_xpartialup:
	ds	3

	global	_xpartialdown
X_PARTIAL_DOWN	equ	(_xpartialdown-_draw_state)
_xpartialdown:
	ds	3

	global	_ypartialup
Y_PARTIAL_UP	equ	(_ypartialup-_draw_state)
_ypartialup:
	ds	3

	global	_ypartialdown
Y_PARTIAL_DOWN	equ	(_ypartialdown-_draw_state)
_ypartialdown:
	ds	3

; extern uint24_t xpartial, ypartial;
	global	_xpartial
X_PARTIAL	equ	(_xpartial-_draw_state)
_xpartial:
	ds	3

	global	_ypartial
Y_PARTIAL	equ	(_ypartial-_draw_state)
_ypartial:
	ds	3

;extern short angl;
	global	_angl
ANGL	equ	(_angl-_draw_state)
_angl:
	ds	2

; extern fixed xstep, ystep;
	global	_xstep
X_STEP	equ	(_xstep-_draw_state)
_xstep:
	ds	4

	global	_ystep
Y_STEP	equ	(_ystep-_draw_state)
_ystep:
	ds	4

; extern fixed  xintercept, yintercept;
	global	_xintercept
	global	_xintercept_as_short
X_INTERCEPT	equ	(_xintercept-_draw_state)
_xintercept:
	ds	2
_xintercept_as_short:	; readonly
	ds	2

	global	_yintercept
	global	_yintercept_as_short
Y_INTERCEPT	equ	(_yintercept-_draw_state)
_yintercept:
	ds	2
_yintercept_as_short:	; readonly
	ds	2

; extern fixed viewx, viewy; // the focal point
	global	_viewx
	global	_focaltx
VIEW_X		equ	(_viewx-_draw_state)
FOCAL_TX	equ	(_focaltx-_draw_state)
_viewx:
	ds	2					; type fixed
_focaltx:
	ds	2					; type short: focaltx = viewx >> 16 readonly

	global	_viewy
	global	_focalty
VIEW_Y		equ	(_viewy-_draw_state)
FOCAL_TY	equ	(_focalty-_draw_state)
_viewy:							; type fixed
	ds	2
_focalty:						; type: short focalty = viewy >> 16 readonly
	ds	2

; extern short xtile, ytile;
	global	_xtile
X_TILE	equ	(_xtile-_draw_state)
_xtile:
	ds	2

	global	_ytile
Y_TILE	equ	(_ytile-_draw_state)
_ytile:
	ds	2

; extern uint16_t  xspot, yspot;
	global	_xspot
X_SPOT	equ	(_xspot-_draw_state)
_xspot:
	ds	2

	global	_yspot
Y_SPOT	equ	(_yspot-_draw_state)
_yspot:
	ds	2

; extern int24_t  texdelta;
	global	_texdelta
TEX_DELTA	equ	(_texdelta-_draw_state)
_texdelta:
	ds	3

	global	_player_in_pushwall_back_tile
PLAYER_IN_PUSHWALL_BACK_TILE	equ	(_player_in_pushwall_back_tile-_draw_state)
_player_in_pushwall_back_tile:
	ds	1


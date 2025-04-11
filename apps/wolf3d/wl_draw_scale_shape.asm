	include "macros.inc"

	section	.text_on_chip, "ax", @progbits
	.assume	adl=1

;drawing_params_t drawing_params __data_on_chip;
	extern _drawing_params

	global	_scale_shape_line
_scale_shape_line:
	ld	iy, ss_scale_shape

        ;ss_screndy = (ss_ycnt >> 6) + ss_upperedge;
	ld	hl, (iy+SS_YCNT)
	ld	a, (iy+SS_YCNT+2)		; u of hl
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	sra	a				; ss_ycnt >> 1
	rr	h
	rr	l
	ld	(iy+SS_TMP1), hl
	ld	(iy+SS_TMP1+2), a
	ld	hl, (iy+SS_TMP1)

	ld	de, (iy+SS_UPPEREDGE)
	add	hl, de
	ld	(iy+SS_SCRENDY), hl


	;if (ss_scrstarty == ss_screndy || ss_screndy <= 0)
	;  return
	ld	hl, (iy+SS_SCRSTARTY)
	ld	de, (iy+SS_SCRENDY)
	or	a
	sbc	hl, de
	ret	z

	xor	a
	sbc	hl, hl
	ex	de, hl
	compare_16bit_signed
	ret	z
	ret	p


	; ss_color = ((byte *)ss_shape)[ss_newstart + ss_j];
	xor	a
	sbc	hl, hl
	ld	l, (iy+SS_NEWSTART)
	ld	h, (iy+SS_NEWSTART+1)
	ex	de, hl
	ld	hl, (iy+SS_SHAPE)
	add	hl, de

	ld	e, (iy+SS_J)
	ld	d, (iy+SS_J+1)
	add	hl, de

	ld	a, (hl)
	ld	(iy+SS_COLOR), a

	; if (ss_scrstarty < 0)
	; 	ss_scrstarty = 0;
	ld	hl, (iy+SS_SCRSTARTY)
	ld	de, 0
	compare_16bit_signed
	jp	m, .not_neg

	ld	(iy+SS_SCRSTARTY), de

.not_neg:
	; if (ss_screndy > drawing_params.view_height)
	; 	ss_screndy = drawing_params.view_height, ss_j = ss_endy;
	ld	de, (iy+SS_SCRENDY)
	xor	a
	sbc	hl, hl
	ld	a, (_drawing_params+14) 	; de' drawing_params.view_width
	ld	l, a
	ld	a, (_drawing_params+15) 	; de' drawing_params.view_width
	ld	h, a
	push	hl
	pop	bc
	compare_16bit_signed
	jp	m, .within_height

	ld	(iy+SS_SCRENDY), bc		; ss_screndy = drawing_params.view_height

	ld	a, (iy+SS_ENDY)			; ss_j = ss_endy
	ld	(iy+SS_J), a
	ld	a, (iy+SS_ENDY+1)
	ld	(iy+SS_J+1), a

.within_height:

	; while (ss_scrstarty < ss_screndy) {
	; 	*ss_vmem = ss_color;
	; 	ss_vmem += drawing_params.view_width;
	; 	ss_scrstarty++;
	; }

	exx
	ld	hl, (iy+SS_VMEM)		; hl' ss_vmem
	ld	de, 0
	ld	a, (_drawing_params+14) 	; de' drawing_params.view_width
	ld	e, a
	ld	a, (_drawing_params+15) 	; de' drawing_params.view_width
	ld	d, a
	exx

	ld	de, (iy+SS_SCRSTARTY)
	ld	hl, (iy+SS_SCRENDY)
	compare_16bit_signed			; hl: (ss_screndy - ss_scrstarty), de: ss_scrstarty
	jp	p, .exit
	ex	de, hl				; de: counter, hl: ss_scrstarty

	ld	a, (iy+SS_COLOR)
	ex	af, af'			; af' ss_color
.loop:
	ld	a, d			; test if de 0
	or	e
	jr	z, .exit

	ex	af, af'
	exx
	ld	(hl), a			; *ss_vmem = ss_color;
	add	hl, de			; ss_vmem += drawing_params.view_width;
	exx
	ex	af, af'

	inc	hl			; ss_scrstarty++;
	dec	de			; counter--;
	jr	.loop

.exit:
	exx
	ld	(iy+SS_VMEM), hl
	ret

	section	.data_on_chip, "aw", @progbits

ss_scale_shape:
;t_compshape *ss_shape;
	global	_ss_shape
SS_SHAPE	equ	0
_ss_shape:
	ds	3

;uint24_t     ss_scale;
	global	_ss_scale
SS_SCALE	equ	(_ss_scale-ss_scale_shape)
_ss_scale:
	ds	3

; uint16_t     ss_pixheight;
	global	_ss_pixheight
SS_PIXHEIGHT	equ	(_ss_pixheight-ss_scale_shape)
_ss_pixheight:
	ds	2

; uint16_t     ss_starty;
	global	_ss_starty
SS_STARTY	equ	(_ss_starty-ss_scale_shape)
_ss_starty:
	ds	2

; uint16_t     ss_endy;
	global	_ss_endy
SS_ENDY	equ	(_ss_endy-ss_scale_shape)
_ss_endy:
	ds	2

; uint16_t    *ss_cmdptr;
	global	_ss_cmdptr
SS_CMDPTR	equ	(_ss_cmdptr-ss_scale_shape)
_ss_cmdptr:
	ds	3

; byte        *ss_cline;
	global	_ss_cline
SS_CLINE	equ	(_ss_cline-ss_scale_shape)
_ss_cline:
	ds	3

; byte        *ss_line;
	global	_ss_line
SS_LINE	equ	(_ss_line-ss_scale_shape)
_ss_line:
	ds	3

; byte        *ss_vmem;
	global	_ss_vmem
SS_VMEM	equ	(_ss_vmem-ss_scale_shape)
_ss_vmem:
	ds	3

; int24_t      ss_actx;
	global	_ss_actx
SS_ACTX	equ	(_ss_actx-ss_scale_shape)
_ss_actx:
	ds	3

; uint16_t     ss_i_counter;
	global	_ss_i_counter
SS_I_COUNTER	equ	(_ss_i_counter-ss_scale_shape)
_ss_i_counter:
	ds	2

; int24_t      ss_upperedge;
	global	_ss_upperedge
SS_UPPEREDGE	equ	(_ss_upperedge-ss_scale_shape)
_ss_upperedge:
	ds	3

; int16_t      ss_newstart;
	global	_ss_newstart
SS_NEWSTART	equ	(_ss_newstart-ss_scale_shape)
_ss_newstart:
	ds	2

; int24_t      ss_scrstarty;
	global	_ss_scrstarty
SS_SCRSTARTY	equ	(_ss_scrstarty-ss_scale_shape)
_ss_scrstarty:
	ds	3

; int24_t      ss_screndy;
	global	_ss_screndy
SS_SCRENDY	equ	(_ss_screndy-ss_scale_shape)
_ss_screndy:
	ds	3

; int24_t      ss_lpix;
	global	_ss_lpix
SS_LPIX	equ	(_ss_lpix-ss_scale_shape)
_ss_lpix:
	ds	3

; int24_t      ss_rpix;
	global	_ss_rpix
SS_RPIX	equ	(_ss_rpix-ss_scale_shape)
_ss_rpix:
	ds	3

; int24_t      ss_pixcnt;
	global	_ss_pixcnt
SS_PIXCNT	equ	(_ss_pixcnt-ss_scale_shape)
_ss_pixcnt:
	ds	3

; int24_t      ss_ycnt;
	global	_ss_ycnt
SS_YCNT	equ	(_ss_ycnt-ss_scale_shape)
_ss_ycnt:
	ds	3

; uint16_t     ss_j;
	global	_ss_j
SS_J	equ	(_ss_j-ss_scale_shape)
_ss_j:
	ds	2

; uint8_t      ss_color;
	global	_ss_color
SS_COLOR	equ	(_ss_color-ss_scale_shape)
_ss_color:
	ds	1
; uint24_t	ss_tmp1
SS_TMP1		equ	(_ss_tmp1-ss_scale_shape)
_ss_tmp1:
	ds	4

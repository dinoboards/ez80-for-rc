
	section	.text, "ax", @progbits
	.assume	adl=1

	section	.data_on_chip, "aw", @progbits

;t_compshape *ss_shape;
	global	_ss_shape
_ss_shape:
	ds	3

;uint24_t     ss_scale;
	global	_ss_scale
_ss_scale:
	ds	3

; uint16_t     ss_pixheight;
	global	_ss_pixheight
_ss_pixheight:
	ds	2

; uint16_t     ss_starty;
	global	_ss_starty
_ss_starty:
	ds	2

; uint16_t     ss_endy;
	global	_ss_endy
_ss_endy:
	ds	2

; uint16_t    *ss_cmdptr;
	global	_ss_cmdptr
_ss_cmdptr:
	ds	3

; byte        *ss_cline;
	global	_ss_cline
_ss_cline:
	ds	3

; byte        *ss_line;
	global	_ss_line
_ss_line:
	ds	3

; byte        *ss_vmem;
	global	_ss_vmem
_ss_vmem:
	ds	3

; int24_t      ss_actx;
	global	_ss_actx
_ss_actx:
	ds	3

; uint16_t     ss_i_counter;
	global	_ss_i_counter
_ss_i_counter:
	ds	2

; int24_t      ss_upperedge;
	global	_ss_upperedge
_ss_upperedge:
	ds	3

; int16_t      ss_newstart;
	global	_ss_newstart
_ss_newstart:
	ds	2

; int24_t      ss_scrstarty;
	global	_ss_scrstarty
_ss_scrstarty:
	ds	3

; int24_t      ss_screndy;
	global	_ss_screndy
_ss_screndy:
	ds	3

; int24_t      ss_lpix;
	global	_ss_lpix
_ss_lpix:
	ds	3

; int24_t      ss_rpix;
	global	_ss_rpix
_ss_rpix:
	ds	3

; int24_t      ss_pixcnt;
	global	_ss_pixcnt
_ss_pixcnt:
	ds	3

; int24_t      ss_ycnt;
	global	_ss_ycnt
_ss_ycnt:
	ds	3

; uint16_t     ss_j;
	global	_ss_j
_ss_j:
	ds	2

; uint8_t      ss_color;
	global	_ss_color
_ss_color:
	ds	1

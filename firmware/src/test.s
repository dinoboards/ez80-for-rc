
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

	XREF	z80_invoke
	XREF	__INTERNAL_RAM_RAM
	XREF	__INTERNAL_RAM_LEN
_spike:
	di

	; ram is in slot 3-2
	; put slot-3 into page 3

	; write to A8, to select slot 3 for page 3
	; set sub slot 3 to 3-2

	LD	A, 0		; set all pages to slot 0?
	ld	bc, %ffa8
	OUT	(bc), A	;

	ld	a, (%030000)


	LD	A, 255		; set all pages to slot 3?
	OUT	(%A8), A	;
	in	a, (%a8)
	LD	HL, %03FFFF	;
	LD	(HL), %AA	; 10101010 (set all pages to sub slot 2)
	ld	a, (hl)

	ld	hl, %030000
	ld	b, (hl)
	ld	a, %55
	ld	(hl), a
	ld	c, (hl)
	ld	a, %aa
	ld	(hl), a
	ld	d, (hl)

	ld	a, %3
	ld	mb, a
	ld.sis	sp, %100
	ld	hl, %1234
	ld 	de, %5678
	push.s	hl
	push.s	de

	nop
	nop
	ret

; 	; write spike code here to be run in ADL mode on  on-chip rom

; RAM_FREE	.EQU	__INTERNAL_RAM_RAM+__INTERNAL_RAM_LEN
; RAM_FREE_BASE	.EQU	RAM_FREE & %FFFF

; 	; copy code from rom to ram and run it in Z80 mode
; 	LD	HL, step1
; 	ld	de, RAM_FREE
; 	ld	bc, step1_size
; 	ldir

; 	LD	A, 02H
; 	LD	MB, A
; 	LD.SIS	SP, 0EA00H
; 	NOP
; 	NOP
; 	ld	iy, RAM_FREE_BASE
; 	call	z80_invoke
; 	; CALL.IS	RAM_FREE_BASE		; run the z80 code in RAM
; 	RET

; 	SEGMENT CODE

; 	.assume adl = 0

; step1:
; 	NOP
; 	ld	a, 3

; loop:
; 	dec	a
; 	jr	nz, loop

; 	nop
; 	nop
; 	nop

; 	JR	step1
; 	; write code here to be run in z80 mode on on-chip rom

; step1_end:

; step1_size	equ	step1_end-step1

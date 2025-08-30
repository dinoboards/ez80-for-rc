
; Things to test for

; boot up in emulated mode
;   do *not* do exchange version
;   code should be emulated
;   calls to ADL should transition to native
;   ints should always be emulated
;   emulated invoke from ADL should emulate routine

; boot up in emulated mode
;  do do exchange version
;  should transition to native execution
;  ints should always be marshalled as native
;  call to emulated routine should work
;  ints during emulation should still be executed natively


; for emulation boot
;  test int during normal emulation ./
;  test int during adl execution    ./
;  test int during emulator_invoke execution ./


if 1
        INCLUDE "startup\ez80F92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

	XREF	z80_invoke_iy
	xref	z80_regs
	xref	z80_flags
	XREF	__INTERNAL_RAM_RAM
	XREF	__INTERNAL_RAM_LEN

longspike:
	ld	a, 3
	ld	b, 1
	ld	e, 'l'
	rst.l	%10			; log out 'l'

	ld	a, 7
	ld	iy, bdosspike - step1 + %00E000
	rst.l	%10			; EMULATOR INVOKE

	ld	a, 3
	ld	b, 1
	rst.l	%10

; 	; call.sis	bdosspike - step1 + %00E000

; blocked:
; 	nop
; 	nop
; 	jr	blocked

	ret.l		; this should return to the emulator

_spike:

	LD	A, 8
	LD	B, 0
	LD	C, 65
	RST	%10

	LD	C, 65
	RST	%10

	LD	C, 65
	RST	%10

	LD	C, 65
	RST	%10

	LD	C, 65
	RST	%10

	LD	C, 25	; plot
	RST	%10

	LD	C, 85	; triangle
	RST	%10

	LD	C, 11
	RST	%10

	LD	C, 22
	RST	%10

	LD	C, 33
	RST	%10

	LD	C, 44
	RST	%10


	nop
	nop



	ret

	di

	; write spike code here to be run in ADL mode on  on-chip rom

RAM_FREE	.EQU	__INTERNAL_RAM_RAM+__INTERNAL_RAM_LEN
RAM_FREE_BASE	.EQU	RAM_FREE & %FFFF

	; copy code from rom to ram and run it in Z80 mode
	LD	HL, step1
	ld	de, %02E000
	ld	bc, step1_size
	ldir

	xref	z80_int_request2
	ld	hl, spikeemulatedint_rom - step1 + %00E000
	ld	(z80_int_request2+2), hl

	LD	A, 02H
	LD	MB, A
	LD.SIS	SP, 0EA00H
	NOP
	NOP
	ld	iy, %02E000
	ld	ix, z80_regs
	ld	(ix+z80_flags), 0

	call	z80_invoke_iy

	SEGMENT CODE

	.assume adl = 0

	; todo:
	; 1 discover if running emulation

step1:
; 	ld	hl, %0040
; 	ld	ix, %0000

; 	lea	hl, ix+0

; 	ld	a, l
; 	or	a
; 	jr	z, _detected_ez80
; _z80:
; 	nop
; 	nop
; 	jr	_z80

; _detected_ez80:
	; ld	a, 0
	; ld	b, 0
	; ld	c, 2
	; ld	d, 1
	; ld	e, 0
	; ld	hl, 0
	; rst.l	%10	; exchange ver and switch to native
	; 		; int should change to marshalling (and require handler to use ret.l)

	; ei

	; xor	a
	; ld	b, 19
	; rst.l	%10		; get emulation state

	; ei

	;call.il	longspike	; aka call.sil

	ld	a, 3
	ld	b, 1
	ld	e, 65		; print A
	rst.l	%10

	call.lil longspike	; bad but should be made to work

	ld	a, 3
	ld	b, 1
	ld	e, 66		; print B
	rst.l	%10

	ld	a, 3
	ld	b, 1
	ld	e, 67		; print C
	rst.l	%10


loop:
	nop
	jr	loop

	nop

	; write code here to be run in z80 mode on on-chip rom

spikeemulatedint_rom:
	push	af
	push	bc
	push	de
	ld	a, 3
	ld	b, 1
	ld	e, 'i'
	rst.l	%10			; log out 'l'
	pop	de
	pop	bc
	pop	af
	ei
	ret.l			; when marshalled native - needs to be change to ret.l

bdosspike:			; this should be executed emulated
	ld	a, %12
	ld	bc, %3456
	; ld	de, %789a
	ld	e, 'X'
; bdosspikelp:
; 	nop
; 	jr	bdosspikelp

	ret.l

step1_end:

step1_size	equ	step1_end-step1

endif

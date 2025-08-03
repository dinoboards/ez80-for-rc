
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

_spike:
;	ld	a, 8
;	ld	b, 0

	; ld	c, 22 ;set mode
	; rst	%10

	; ld	c, 0 ;set mode 0
	; rst	%10

;	ld	c, 'A'
;	rst	%10

;	ld	c, 'a'
;	rst	%10

;	ld	c, 'b'
;	rst	%10
_halt:
	nop
	nop
	jr	_halt

; 	di

; 	; write spike code here to be run in ADL mode on  on-chip rom

; RAM_FREE	.EQU	__INTERNAL_RAM_RAM+__INTERNAL_RAM_LEN
; RAM_FREE_BASE	.EQU	RAM_FREE & %FFFF

; 	; copy code from rom to ram and run it in Z80 mode
; 	LD	HL, step1
; 	ld	de, %02E000
; 	ld	bc, step1_size
; 	ldir

; 	call.sis	%E000
; 	ret.l

; 	SEGMENT CODE

; 	.assume adl = 0

; step1:

; 	ret.l			; when marshalled native - needs to be change to ret.l


; step1_end:

; step1_size	equ	step1_end-step1

endif

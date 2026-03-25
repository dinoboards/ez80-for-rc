        INCLUDE "..\config.inc"

	SEGMENT	CODE

	.assume adl=1

	global	_linear_mem_test_at

TEST_VALUES_SIZE	equ	44

; bool test_at(volatile uint8_t *ptr)

_linear_mem_test_at:
	ld	iy, 0
	add	iy, sp
	lea	HL, iy-TEST_VALUES_SIZE
	ld	sp, hl

	ld	b, TEST_VALUES_SIZE			; clear copy back array
	xor	a
.loop1:
	ld	(hl), a
	inc	hl
	djnz	.loop1

	ld	de, (iy+3)				; copy test values to ptr
	LD	HL, _test_values
	ld	bc, TEST_VALUES_SIZE
	ldir

	ld	hl, (iy+3)				; read back test values to test_values_back
	lea	de, iy-TEST_VALUES_SIZE
	ld	bc, TEST_VALUES_SIZE
	ldir

	lea	HL, iy-TEST_VALUES_SIZE			; test if values are correct
	ld	de, _test_values

.check:
	ld	b, TEST_VALUES_SIZE
.loop2:
	ld	a, (de)
	cp	(hl)
	jr	nz, .bad_copy				; if not, return false
	inc	hl
	inc	de
	djnz	.loop2

.success:
	ld	a, 1					; return true
	LD	sp, iy
	ret

.fail:
	pop	bc
.bad_copy:
	xor	a

	LD	sp, iy
	ret

	SECTION	TEXT

_test_values:	DB 	000H, 011H, 022H, 033H, 044H, 055H, 066H, 077H
		DB	088H, 099H, 0AAH, 0BBH, 0CCH, 0DDH, 0EEH, 0FFH


	PUBLIC	_romwbw_sig
	PUBLIC	_romwbw_name_0

_romwbw_sig	EQU	030070H
_romwbw_name_0	EQU	030080H


	PUBLIC	_msxrom_vdp_marker
	PUBLIC	_msxrom_ver_marker

_msxrom_vdp_marker	EQU	030006H
_msxrom_ver_marker	EQU	03002DH


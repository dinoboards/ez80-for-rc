	section	.text_on_chip, "ax", @progbits
	.assume	adl=1

	global	_test_at
	extern	_test_values

TEST_VALUES_SIZE	equ	44

; bool test_at(volatile uint8_t *ptr)

_test_at:
	di
	ld	iy, 0
	add	iy, sp

	ld	hl, (iy+3)				; address to be tested at
	ld	(.test_addr), hl			; SP may overlaps with test addr

	; first backup whats already at ptr
	ld	hl, (.test_addr)
	ld	de, .test_values_restore
	ld	bc, TEST_VALUES_SIZE
	ldir


	ld	hl, .test_values_back			; clear copy back array
	ld	b, TEST_VALUES_SIZE
	xor	a
.loop1:
	ld	(hl), a
	inc	hl
	djnz	.loop1

	ld	de, (.test_addr)				; copy test values to ptr
	ld	hl, _test_values
	ld	bc, TEST_VALUES_SIZE
	ldir

	ld	hl, (.test_addr)				; read back test values to test_values_back
	ld	de, .test_values_back
	ld	bc, TEST_VALUES_SIZE
	ldir

	ld	hl, .test_values_back			; test if values are correct
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
	jr	.try_restore_data

.bad_copy:
	xor	a

.try_restore_data:
	ld	de, (.test_addr)
	ld	hl, .test_values_restore
	ld	bc, TEST_VALUES_SIZE
	ldir
	ei
	ret

.test_addr:
	ds	3

.test_values_restore:
	ds	TEST_VALUES_SIZE

.test_values_back:
	ds	TEST_VALUES_SIZE


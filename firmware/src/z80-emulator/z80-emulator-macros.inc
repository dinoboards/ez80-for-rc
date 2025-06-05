
 	xref	z80_loop
	xref	test_poi

IO_SEGMENT	equ	%FF

; z80loop	macro
; 	call	test_poi
; 	jp	z80_loop
; 	endmacro

z80loop	macro
	jp	z80_loop
	endmacro

z80_byte_jump	macro	instr_table
	or	a
	sbc	hl, hl
	ld.s	l, (iy)
	inc	iy
	add	hl, hl
	add	hl, hl
	ld	bc, instr_table
	add	hl, bc
	jp	(hl)
	endmacro

z80_addirrr:	macro	ir, rr
z80_add&ir&rr:
	exx
	push	rr
	exx
	pop	bc
	ld	hl, (ix+z80_reg_&ir)
	ex	af, af'
	add.s	hl, bc
	ex	af, af'
	ld	(ix+z80_reg_&ir), hl
	z80loop
	endmacro

z80_ldirnn	macro ir
z80_ld&ir&nn:
	ld.s	hl, (iy)
	lea	iy,iy+2
	ld	(ix+z80_reg_&ir), hl
	z80loop
	endmacro

z80_addirir	macro	ir
z80_add&ir&ir:
	ld	hl, (ix+z80_reg_&ir)
	ex	af, af'
	add.s	hl, hl
	ex	af, af'
	ld	(ix+z80_reg_&ir), hl
	z80loop
	endmacro

z80_addirsp	macro ir
z80_add&ir&sp:
	ld	hl, (ix+z80_reg_&ir)
	ex	af, af'
	add.s	hl, sp
	ex	af, af'
	ld	(ix+z80_reg_&ir), hl
	z80loop
	endmacro

z80_popir	macro ir
z80_pop&ir:
	pop.s	hl
	ld	(ix+&z80_reg_&ir), hl
	z80loop
	endmacro

z80_pushir	macro ir
z80_push&ir:
	ld	hl, (ix+&z80_reg_&ir)
	push.s	hl
	z80loop
	endmacro

z80_jp_ir_	macro	ir
z80_jp_&ir&_:
	ld	iy, (ix+z80_reg_&ir)
	z80loop
	endmacro


z80_niy	macro name
	xref	not_implemented
z80_&name:
	call	not_implemented
	z80loop
	endmacro

z80_exmain macro operation, op2, op3, op4
	exx
	operation
	op2
	op3
	op4
	exx
	z80loop
	endmacro

z80_exmain2 macro name, operation, op2, op3, op4
z80_&name:
	exx
	operation
	op2
	op3
	op4
	exx
	z80loop
	endmacro

z80_exaf macro operation, op2, op3
	ex	af, af'
	operation
	op2
	op3
	ex	af, af'
	z80loop
	endmacro

z80_exaf2 macro name, operation, op2, op3
z80_&name:
	ex	af, af'
	operation
	op2
	op3
	ex	af, af'
	z80loop
	endmacro

z80_exall	macro	operation, op2, op3
	exx
	ex	af, af'
	operation
	op2
	op3
	ex	af, af'
	exx
	z80loop
	endmacro

z80_exall2	macro	name, operation, op2, op3
z80_&name:
	exx
	ex	af, af'
	operation
	op2
	op3
	ex	af, af'
	exx
	z80loop
	endmacro

z80_afir	macro	name, ir, operation, op2
z80_&name
	ld.s	a, (iy)
	inc	iy

	; Sign-extend 8-bit A into 16-bit HL
	ld	l, a  ; Store low byte
	add	a, a  ; Push sign into carry
	sbc	a     ; Turn it into 0 or -1
	ld	h, a  ; Store high byte

	ld	bc, (ix+z80_reg_&ir)
	add.s	hl, bc

	ex	af, af'
	operation
	op2
	ex	af, af'
	z80loop
	endmacro

z80_irtohl	macro	name, ir, operation, op2, op3
z80_&name
	ld.s	a, (iy)
	inc	iy

	; Sign-extend 8-bit A into 16-bit HL
	ld	l, a  ; Store low byte
	add	a, a  ; Push sign into carry
	sbc	a     ; Turn it into 0 or -1
	ld	h, a  ; Store high byte

	ld	bc, (ix+z80_reg_&ir)
	add.s	hl, bc

	operation
	op2
	op3
	z80loop
	endmacro

z80_irtoix	macro	name, ir, operation, op2, op3
z80_&name
	ld.s	a, (iy)
	inc	iy

	ld	bc, (ix+z80_reg_&ir)

	; Sign-extend 8-bit A into 16-bit HL
	ld	ixl, a  ; Store low byte
	add	a, a  ; Push sign into carry
	sbc	a     ; Turn it into 0 or -1
	ld	ixh, a  ; Store high byte

	add.s	ix, bc
	exx
	operation
	op2
	op3
	exx
	ld	ix, z80_regs
	z80loop
	endmacro

z80_retcc	macro		op, cc
z80_ret&cc:
	ex	af, af'
	op	cc, $$z80_retcc1
	ex	af, af'
	z80loop

$$z80_retcc1:
	ex	af, af'
	pop.s	iy
	z80loop
	endmacro

z80_out_c_jj	macro	jj
z80_out_c_&jj:
	exx
	push	bc
	ld	b, IO_SEGMENT
	out	(bc), &jj
	pop	bc
	exx
	z80loop
	endmacro

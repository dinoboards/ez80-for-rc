
 	xref	z80_loop
	xref	test_poi

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

z80_exaf2 macro name, operation, op2, op3
z80_&name:
	ex	af, af'
	operation
	op2
	op3
	ex	af, af'
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

z80_jrccd	macro opand
	inc	iy
	exx
	ex	af, af'
	opand	$$skip1
	ex	af, af'
	exx
	z80loop

$$skip1:
	ex	af, af'
	exx
	ld.s	a, (iy-1)
	jp	_z80_add_a_to_pc
	endmacro

z80_jpccnn	macro j, cc
z80_jp&cc&nn:
	ex	af, af'
	j	cc, $$skip
	ex	af, af'
	lea	iy, iy+2
	z80loop

$$skip:
	ex	af, af'
	ld.s	iy, (iy)
	z80loop
	endmacro

z80_callccnn	macro	cc
z80_call&cc&nn:
	ex	af, af'
	jp	cc, $$skip
	ex	af, af'
	lea	iy, iy+2
	z80loop
$$skip:
	ex	af, af'
	pea.s	iy+2
	ld.s	iy, (iy)
	z80loop

	endmacro

z80_out_c_jj	macro	jj
z80_out_c_&jj:
	exx
	push	bc
	ld	b, IO_SEGMENT
	io_rate_start
	out	(bc), &jj
	io_rate_end
	pop	bc
	exx
	z80loop
	endmacro

io_rate_start	macro
$$wait:
	IN0	A, (TMR2_DR_L)
	JR	nz, $$wait
	endmacro

io_rate_end	macro
	LD	A, TMR_ENABLED | TMR_SINGLE | TMR_RST_EN | TMR_CLK_DIV_4
	OUT0	(TMR2_CTL), A
	endmacro

	xref	z80_ei_enabled
di_and_save_s:	macro
	push	af
	ld	a, i
	di
	ld	a, 0
	jp	po, $$_di
	ld	a, 1
$$_di:
	ld	(z80_ei_enabled), a
	pop	af
	endmac	di_and_save_s

restore_ei_s:	macro
	push	af
	ld	a, (z80_ei_enabled)
	or	a
	jr	z, $$_di
	ei
$$_di:
	pop	af
	endmac	restore_ei_s

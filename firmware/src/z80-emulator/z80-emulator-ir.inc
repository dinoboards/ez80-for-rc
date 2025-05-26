
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
	inc	iy
	inc	iy
	ld	(ix+z80_reg_&ir), iy
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

	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

	section	CODE

	xref	z80_nop
	xref	z80_regs
	xref	z80_reg_ix
	xref	dd_bit_instr
	xref	z80_ixbits2

	xref	z80_ldbb
	xref	z80_ldbc
	xref	z80_ldbd
	xref	z80_ldbe
	xref	z80_ldba
	xref	z80_ldcb
	xref	z80_ldcc
	xref	z80_ldcd
	xref	z80_ldce
	xref	z80_ldca
	xref	z80_lddb
	xref	z80_lddc
	xref	z80_lddd
	xref	z80_ldde
	xref	z80_ldda
	xref	z80_ldeb
	xref	z80_ldec
	xref	z80_lded
	xref	z80_ldee
	xref	z80_ldea
	xref	z80_ldab
	xref	z80_ldac
	xref	z80_ldad
	xref	z80_ldae
	xref	z80_ldaa
	xref	z80_addab
	xref	z80_addac
	xref	z80_addad
	xref	z80_addac
	xref	z80_addaa
	xref	z80_adcab
	xref	z80_adcac
	xref	z80_adcad
	xref	z80_adcae
	xref	z80_adcaa
	xref	z80_subab
	xref	z80_subac
	xref	z80_subad
	xref	z80_subae
	xref	z80_subaa
	xref	z80_sbcaa
	xref	z80_andab
	xref	z80_andac
	xref	z80_andad
	xref	z80_andae

regir	equ	z80_reg_ix

	; DD ....
	global	ix_instr_table

ix_instr_table:
	jp	z80_ix_nop		; DD 00
	jp	z80_ix_nop		; DD 01
	jp	z80_ix_nop		; DD 02
	jp	z80_ix_nop		; DD 03
	jp	z80_ix_nop		; DD 04
	jp	z80_ix_nop		; DD 05
	jp	z80_ix_nop		; DD 06
	jp	z80_ldbc_ixd_		; DD 07 ld bc, (ix+d)
	jp	z80_ix_nop		; DD 08
	jp	z80_addixbc		; DD 09 add ix,bc
	jp	z80_ix_nop		; DD 0A
	jp	z80_ix_nop		; DD 0B
	jp	z80_ix_nop		; DD 0C
	jp	z80_ix_nop		; DD 0D
	jp	z80_ix_nop		; DD 0E
	jp	z80_ld_ixd_bc		; DD 0F ld (ix+d), bc
	jp	z80_ix_nop		; DD 10
	jp	z80_ix_nop		; DD 11
	jp	z80_ix_nop		; DD 12
	jp	z80_ix_nop		; DD 13
	jp	z80_ix_nop		; DD 14
	jp	z80_ix_nop		; DD 15
	jp	z80_ix_nop		; DD 16
	jp	z80_ldde_ixd_		; DD 17 ld de, (ix+d)
	jp	z80_ix_nop		; DD 18
	jp	z80_addixde		; DD 19 add ix,de
	jp	z80_ix_nop		; DD 1A
	jp	z80_ix_nop		; DD 1B
	jp	z80_ix_nop		; DD 1C
	jp	z80_ix_nop		; DD 1D
	jp	z80_ix_nop		; DD 1E
	jp	z80_ld_ixd_de		; DD 1F ld (ix+d), de
	jp	z80_ix_nop		; DD 20
	jp	z80_ldixnn		; DD 21 ld ix,nn
	jp	z80_ld_nn_ix		; DD 22 ld (nn),ix
	jp	z80_incix		; DD 23 inc ix
	jp	z80_incixh		; DD 24 inc ixh
	jp	z80_decixh		; DD 25 dec ixl
	jp	z80_ldixhn		; DD 26 ld ixh, n
	jp	z80_ldhl_ixd_		; DD 27 ld hl, (ix+d)
	jp	z80_ix_nop		; DD 28
	jp	z80_addixix		; DD 29 add ix,ix
	jp	z80_ldix_nn_		; DD 2A ld ix,(nn)
	jp	z80_decix		; DD 2B dec ix
	jp	z80_incixl		; DD 2C inc ixl
	jp	z80_decixl		; DD 2D dec ixl
	jp	z80_ldixln		; DD 2E ld ixl, n
	jp	z80_ld_ixd_hl		; DD 2F ld (ix+d), hl
	jp	z80_ix_nop		; DD 30
	jp	z80_ldiy_ixd_		; DD 31 ld iy, (ix+d)
	jp	z80_ix_nop		; DD 32
	jp	z80_ix_nop		; DD 33
	jp	z80_inc_ixd_		; DD 34 inc (ix+d)
	jp	z80_dec_ixd_		; DD 35 dec (ix+d)
	jp	z80_ld_ixd_n_		; DD 36 ld (ix+d),n
	jp	z80_ldix_ixd_		; DD 37 ld ix, (ix+d)
	jp	z80_ix_nop		; DD 38
	jp	z80_addixsp		; DD 39 add ix,sp
	jp	z80_ix_nop		; DD 3A
	jp	z80_ix_nop		; DD 3B
	jp	z80_ix_nop		; DD 3C
	jp	z80_ix_nop		; DD 3D
	jp	z80_ld_ixd_iy		; DD 3E ld (ix+d), iy
	jp	z80_ld_ixd_ix		; DD 3F ld (ix+d), ix
	jp	z80_ldbb		; DD 40
	jp	z80_ldbc		; DD 41
	jp	z80_ldbd		; DD 42
	jp	z80_ldbe		; DD 43
	jp	z80_ldbixh		; DD 44 ld b, ixh
	jp	z80_ldbixl		; DD 45 ld b, ixl
	jp	z80_ldb_ixd_		; DD 46 ld b,(ix+d)
	jp	z80_ldba		; DD 47
	jp	z80_ldcb		; DD 48
	jp	z80_ldcc		; DD 49
	jp	z80_ldcd		; DD 4A
	jp	z80_ldce		; DD 4B
	jp	z80_ldcixh		; DD 4C ld c, ixh
	jp	z80_ldcixl		; DD 4D ld c, ixl
	jp	z80_ldc_ixd_		; DD 4E ld c,(ix+d)
	jp	z80_ldca		; DD 4F
	jp	z80_lddb		; DD 50
	jp	z80_lddc		; DD 51
	jp	z80_lddd		; DD 52
	jp	z80_ldde		; DD 53
	jp	z80_lddixh		; DD 54 ld d, ixh
	jp	z80_lddixl		; DD 55 ld d, ixl
	jp	z80_ldd_ixd_		; DD 56 ld d,(ix+d)
	jp	z80_ldda		; DD 57
	jp	z80_ldeb		; DD 58
	jp	z80_ldec		; DD 59
	jp	z80_lded		; DD 5A
	jp	z80_ldee		; DD 5B
	jp	z80_ldeixh		; DD 5C ld e, ixh
	jp	z80_ldeixl		; DD 5D ld e, ixl
	jp	z80_lde_ixd_		; DD 5E ld e,(ix+d)
	jp	z80_ldea		; DD 5F
	jp	z80_ldixhb		; DD 60 ld ixh, b
	jp	z80_ldixhc		; DD 61 ld ixh, c
	jp	z80_ldixhd		; DD 62 ld ixh, d
	jp	z80_ldixhe		; DD 63 ld ixh, e
	jp	z80_ldixhixh		; DD 64 ld ixh, ixh
	jp	z80_ldixhixl		; DD 65 ld ixh, ixl
	jp	z80_ldh_ixd_		; DD 66 ld h,(ix+d)
	jp	z80_ldixha		; DD 67 ld ixh, a
	jp	z80_ldixlb		; DD 68 ld ixl, b
	jp	z80_ldixlc		; DD 69 ld ixl, c
	jp	z80_ldixld		; DD 6A ld ixl, d
	jp	z80_ldixle		; DD 6B ld ixl, e
	jp	z80_ldixlixh		; DD 6C ld ixl, ixh
	jp	z80_ldixlixl		; DD 6D ld ixl, ixl
	jp	z80_ldl_ixd_		; DD 6E ld l,(ix+d)
	jp	z80_ldixla		; DD 6F ld ixl, a
	jp	z80_ld_ixd_b		; DD 70 ld (ix+d),b
	jp	z80_ld_ixd_c		; DD 71 ld (ix+d),c
	jp	z80_ld_ixd_d		; DD 72 ld (ix+d),d
	jp	z80_ld_ixd_e		; DD 73 ld (ix+d),e
	jp	z80_ld_ixd_h		; DD 74 ld (ix+d),h
	jp	z80_ld_ixd_l		; DD 75 ld (ix+d),l
	jp	z80_ix_nop			; DD 76
	jp	z80_ld_ixd_a		; DD 77 ld (ix+d),a
	jp	z80_ldab		; DD 78
	jp	z80_ldac		; DD 79
	jp	z80_ldad		; DD 7A
	jp	z80_ldae		; DD 7B
	jp	z80_ldaixh		; DD 7C ld a, ixh
	jp	z80_ldaixl		; DD 7D ld a, ixl
	jp	z80_lda_ixd_		; DD 7E ld a,(ix+d)
	jp	z80_ldaa		; DD 7F
	jp	z80_addab		; DD 80
	jp	z80_addac		; DD 81
	jp	z80_addad		; DD 82
	jp	z80_addac		; DD 83
	jp	z80_addaixh		; DD 84 add a, ixh
	jp	z80_addaixl		; DD 85 add a, ixl
	jp	z80_adda_ixd_		; DD 86 add a,(ix+d)
	jp	z80_addaa		; DD 87
	jp	z80_adcab		; DD 88
	jp	z80_adcac		; DD 89
	jp	z80_adcad		; DD 8A
	jp	z80_adcae		; DD 8B
	jp	z80_adcaixh		; DD 8C adc a, ixh
	jp	z80_adcaixl		; DD 8D adc a, ixl
	jp	z80_adca_ixd_		; DD 8E adc a,(ix+d)
	jp	z80_adcaa		; DD 8F
	jp	z80_subab		; DD 90
	jp	z80_subac		; DD 91
	jp	z80_subad		; DD 92
	jp	z80_subae		; DD 93
	jp	z80_subaixh		; DD 94 sub a, ixh
	jp	z80_subaixl		; DD 95 sub a, ixl
	jp	z80_suba_ixd_		; DD 96 sub a, (ix+d)
	jp	z80_subaa		; DD 97
	jp	z80_subab		; DD 98
	jp	z80_subac		; DD 99
	jp	z80_subad		; DD 9A
	jp	z80_subae		; DD 9B
	jp	z80_sbcaixh		; DD 9C sbc a, ixh
	jp	z80_sbcaixl		; DD 9D abc a, ixl
	jp	z80_sbca_ixd_		; DD 9E sbc a, (ix+d)
	jp	z80_sbcaa		; DD 9F
	jp	z80_andab		; DD A0
	jp	z80_andac		; DD A1
	jp	z80_andad		; DD A2
	jp	z80_andae		; DD A3
	jp	z80_andaixh		; DD A4 and a, ixh
	jp	z80_andaixl		; DD A5 and a, ixl
	jp	z80_anda_ixd_		; DD A6 and a, (ix+d)
	jp	z80_ix_nop		; DD A7
	jp	z80_ix_nop		; DD A8
	jp	z80_ix_nop		; DD A9
	jp	z80_ix_nop		; DD AA
	jp	z80_ix_nop		; DD AB
	jp	z80_xoraixh		; DD AC xor a, ixh
	jp	z80_xoraixl		; DD AD xor a, ixl
	jp	z80_xora_ixd_		; DD AE xor a, (ix+d)
	jp	z80_ix_nop		; DD AF
	jp	z80_ix_nop		; DD B0
	jp	z80_ix_nop		; DD B1
	jp	z80_ix_nop		; DD B2
	jp	z80_ix_nop		; DD B3
	jp	z80_oraixh		; DD B4 or a, ixh
	jp	z80_oraixl		; DD B5 or a, ixl
	jp	z80_ora_ixd_		; DD B6 or a, (ix+d)
	jp	z80_ix_nop		; DD B7
	jp	z80_ix_nop		; DD B8
	jp	z80_ix_nop		; DD B9
	jp	z80_ix_nop		; DD BA
	jp	z80_ix_nop		; DD BB
	jp	z80_cpaixh		; DD BC cp a, ixh
	jp	z80_cpaixl		; DD BD cp a, ixl
	jp	z80_cpa_ixd_		; DD BE cp a, (ix+d)
	jp	z80_ix_nop		; DD BF
	jp	z80_ix_nop		; DD C0
	jp	z80_ix_nop		; DD C1
	jp	z80_ix_nop		; DD C2
	jp	z80_ix_nop		; DD C3
	jp	z80_ix_nop		; DD C4
	jp	z80_ix_nop		; DD C5
	jp	z80_ix_nop		; DD C6
	jp	z80_ix_nop		; DD C7
	jp	z80_ix_nop		; DD C8
	jp	z80_ix_nop		; DD C9
	jp	z80_ix_nop		; DD CA
	jp	z80_ixbits		; DD CB
	jp	z80_ix_nop		; DD CC
	jp	z80_ix_nop		; DD CD
	jp	z80_ix_nop		; DD CE
	jp	z80_ix_nop		; DD CF
	jp	z80_ix_nop		; DD D0
	jp	z80_ix_nop		; DD D1
	jp	z80_ix_nop		; DD D2
	jp	z80_ix_nop		; DD D3
	jp	z80_ix_nop		; DD D4
	jp	z80_ix_nop		; DD D5
	jp	z80_ix_nop		; DD D6
	jp	z80_ix_nop		; DD D7
	jp	z80_ix_nop		; DD D8
	jp	z80_ix_nop		; DD D9
	jp	z80_ix_nop		; DD DA
	jp	z80_ix_nop		; DD DB
	jp	z80_ix_nop		; DD DC
	jp	z80_ix_nop		; DD DD
	jp	z80_ix_nop		; DD DE
	jp	z80_ix_nop		; DD DF
	jp	z80_ix_nop		; DD E0
	jp	z80_popix		; DD E1 pop ix
	jp	z80_ix_nop		; DD E2
	jp	z80_ex_sp_ix		; DD E3 ex (sp), ix
	jp	z80_ix_nop		; DD E4
	jp	z80_pushix		; DD E5 push ix
	jp	z80_ix_nop		; DD E6
	jp	z80_ix_nop		; DD E7
	jp	z80_ix_nop		; DD E8
	jp	z80_jp_ix_		; DD E9 jp (ix)
	jp	z80_ix_nop		; DD EA
	jp	z80_ix_nop		; DD EB
	jp	z80_ix_nop		; DD EC
	jp	z80_ix_nop		; DD ED
	jp	z80_ix_nop		; DD EE
	jp	z80_ix_nop		; DD EF
	jp	z80_ix_nop		; DD F0
	jp	z80_ix_nop		; DD F1
	jp	z80_ix_nop		; DD F2
	jp	z80_ix_nop		; DD F3
	jp	z80_ix_nop		; DD F4
	jp	z80_ix_nop		; DD F5
	jp	z80_ix_nop		; DD F6
	jp	z80_ix_nop		; DD F7
	jp	z80_ix_nop		; DD F8
	jp	z80_spix		; DD F9 ld sp, ix
	jp	z80_ix_nop		; DD FA
	jp	z80_ix_nop		; DD FB
	jp	z80_ix_nop		; DD FC
	jp	z80_ix_nop		; DD DD
	jp	z80_ix_nop		; DD FE
	jp	z80_ix_nop		; DD FF

z80_ix_nop:
	call	not_implemented
	jp	z80_nop

	; $DD 07 ld bc, (ix+d)
	z80_niy	ldbc_ixd_

	; $DD 09 add ix, bc
	z80_addirrr	ix, bc

	; $DD 0F ld (ix+d), bc
	z80_niy	ld_ixd_bc

	; DD 17 ld de, (ix+d)
	z80_niy ldde_ixd_

	; $DD 19 add ix, de
	z80_addirrr	ix, de

	; DD 1F ld (ix+d), de
	z80_niy	ld_ixd_de

	; $DD 21 ld ix, nn
	z80_ldirnn	ix

	; DD 22 ld (nn),ix
z80_ld_nn_ix:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld	bc, (ix+z80_reg_ix)
	ld.s	(hl), bc
	z80loop

	; DD 23 inc ix
z80_incix:
	ld	hl, (ix+z80_reg_ix)
	inc	hl
	ld	(ix+z80_reg_ix), hl
	z80loop

	; DD 24 inc ixh
	z80_exaf2	incixh, {inc (ix+z80_reg_ix+1)}

	; DD 25 dec ixh
	z80_exaf2	decixh, {dec (ix+z80_reg_ix+1)}

	; DD 26 ld ixh, n
z80_ldixhn:
	ld.s	a, (iy)
	inc	iy
	ld	(ix+z80_reg_ix+1), a
	z80loop

	; DD 27 ld hl, (ix+d)
	z80_niy	ldhl_ixd_

	; $fd $29 add ix, ix
	z80_addirir	ix

	; DD 2A ld ix, (nn)
z80_ldix_nn_:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld.s	hl, (hl)
	ld	(ix+z80_reg_ix), hl
	z80loop

	; DD 2B dec ix
z80_decix:
	ld	hl, (ix+z80_reg_ix)
	dec	hl
	ld	(ix+z80_reg_ix), hl
	z80loop

	; DD 2C inc ixl
	z80_exaf2	incixl, {inc (ix+z80_reg_ix+0)}

	; DD 2D dec ixl
	z80_exaf2	decixl, {dec (ix+z80_reg_ix+0)}

	; DD 2E ld ixl, n
z80_ldixln:
	ld.s	a, (iy)
	inc	iy
	ld	(ix+z80_reg_ix+0), a
	z80loop

	; DD 2F ld (ix+d), hl
	z80_niy	ld_ixd_hl

	; DD 31 ld iy, (ix+d)
	z80_niy	ldiy_ixd_

	; DD 34 inc (ix+d)
	z80_afir	inc_ixd_, ix, {inc.s (hl)}

	; DD 35 dec (ix+d)
	z80_afir	dec_ixd_, ix, {dec.s (hl)}

	; DD 36 ld (ix+d),n
	z80_irtohl	ld_ixd_n_, ix, {ld.s a, (iy)}, {inc iy}, {ld.s (hl), a}

	; DD 37 ld ix, (ix+d)
	z80_niy	ldix_ixd_

	; $DD $39 add ix,sp
	z80_addirsp	ix

	; DD 3E ld (ix+d), iy
	z80_niy	ld_ixd_iy

	; DD 3F ld (ix+d), ix
	z80_niy	ld_ixd_ix

	; DD 44 ld b, ixh
	z80_exmain2	ldbixh, {ld b, (ix+z80_reg_ix+1)}

	; DD 45 ld b, ixl
	z80_exmain2	ldbixl, {ld b, (ix+z80_reg_ix+0)}

	; DD 46 ld b,(ix+d)
	z80_irtoix	ldb_ixd_, ix, {ld.s b, (ix)}

	; DD 4C ld c, ixh
	z80_exmain2	ldcixh, {ld c, (ix+z80_reg_ix+1)}

	; DD 4D ld c, ixl
	z80_exmain2	ldcixl, {ld c, (ix+z80_reg_ix+0)}

	; DD 4E ld c,(ix+d)
	z80_irtoix	ldc_ixd_, ix, {ld.s c, (ix)}

	; DD 54 ld d, ixh
	z80_exmain2	lddixh, {ld d, (ix+z80_reg_ix+1)}

	; DD 55 ld d, ixl
	z80_exmain2	lddixl, {ld d, (ix+z80_reg_ix+0)}

	; DD 56 ld d,(ix+d)
	z80_irtoix	ldd_ixd_, ix, {ld.s d, (ix)}

	; DD 5C ld e, ixh
	z80_exmain2	ldeixh, {ld e, (ix+z80_reg_ix+1)}

	; DD 5D ld e, ixl
	z80_exmain2	ldeixl, {ld e, (ix+z80_reg_ix+0)}

	; DD 5E ld e,(ix+d)
	z80_irtoix	lde_ixd_, ix, {ld.s e, (ix)}

	; DD 60 ld ixh, b
	z80_exmain2	ldixhb, {ld (ix+z80_reg_ix+1), b}

	; DD 61 ld ixh, c
	z80_exmain2	ldixhc, {ld (ix+z80_reg_ix+1), c}

	; DD 62 ld ixh, d
	z80_exmain2	ldixhd, {ld (ix+z80_reg_ix+1), d}

	; DD 63 ld ixh, e
	z80_exmain2	ldixhe, {ld (ix+z80_reg_ix+1), e}

	; DD 64 ld ixh, ixh
z80_ldixhixh:
	z80loop

	; DD 65 ld ixh, ixl
z80_ldixhixl:
	ld a, (ix+z80_reg_ix+0)
	ld (ix+z80_reg_ix+1), a
	z80loop

	; DD 66 ld h,(ix+d)
	z80_irtoix	ldh_ixd_, ix, {ld.s h, (ix)}

	; DD 67 ld ixh, a
	z80_exaf2	ldixha, {ld (ix+z80_reg_ix+1), a}

	; DD 68 ld ixl, b
	z80_exmain2	ldixlb, {ld (ix+z80_reg_ix+0), b}

	; DD 69 ld ixl, c
	z80_exmain2	ldixlc, {ld (ix+z80_reg_ix+0), c}

	; DD 6A ld ixl, d
	z80_exmain2	ldixld, {ld (ix+z80_reg_ix+0), d}

	; DD 6B ld ixl, e
	z80_exmain2	ldixle, {ld (ix+z80_reg_ix+0), e}

	; DD 6C ld ixl, ixh
z80_ldixlixh:
	ld	a, (ix+z80_reg_ix+1)
	ld	(ix+z80_reg_ix+0), a
	z80loop

	; DD 6D ld ixl, ixl
z80_ldixlixl:
	z80loop

	; DD 6E ld l,(ix+d)
	z80_irtoix	ldl_ixd_, ix, {ld.s l, (ix)}

	; DD 6F ld ixl, a
	z80_exaf2	ldixla, {ld (ix+z80_reg_ix+0), a}

	; DD 70 ld (ix+d),b
z80_ld_ixd_b:
	ld.s	a, (iy)
	inc	iy

	; Sign-extend 8-bit A into 16-bit HL
	ld	l, a  ; Store low byte
	add	a, a  ; Push sign into carry
	sbc	a     ; Turn it into 0 or -1
	ld	h, a  ; Store high byte

	ld	bc, (ix+z80_reg_ix)
	add.s	hl, bc

	exx
	push	bc
	exx
	pop	bc
	ld.s	(hl), b
	z80loop

	; DD 71 ld (ix+d),c
	z80_irtoix	ld_ixd_c, ix, {ld.s (ix), c}

	; DD 72 ld (ix+d),d
	z80_irtoix	ld_ixd_d, ix, {ld.s (ix), d}

	; DD 73 ld (ix+d),e
	z80_irtoix	ld_ixd_e, ix, {ld.s (ix), e}

	; DD 74 ld (ix+d),h
	z80_irtoix	ld_ixd_h, ix, {ld.s (ix), h}

	; DD 75 ld (ix+d),l
	z80_irtoix	ld_ixd_l, ix, {ld.s (ix), l}

	; DD 77 ld (ix+d),a
	z80_afir	ld_ixd_a, ix, {ld.s (hl), a}

	; DD 7C ld a, ixh
	z80_exaf2	ldaixh, {ld a, (ix+z80_reg_ix+1)}

	; DD 7D ld a, ixl
	z80_exaf2	ldaixl, {ld a, (ix+z80_reg_ix+0)}

	; DD 7E ld a,(ix+d)
	z80_afir	lda_ixd_, ix, {ld.s a, (hl)}

	; DD 84 add a, ixh
	z80_exaf2	addaixh, {add a, (ix+z80_reg_ix+1)}

	; DD 85 add a, ixl
	z80_exaf2	addaixl, {add a, (ix+z80_reg_ix+0)}

	; DD 86 add a,(ix+d)
	z80_afir	adda_ixd_, ix, {add.s a, (hl)}

	; DD 8C adc a, ixh
	z80_exaf2	adcaixh, {adc a, (ix+z80_reg_ix+1)}

	; DD 8D adc a, ixl
	z80_exaf2	adcaixl, {adc a, (ix+z80_reg_ix+0)}

	; DD 8E adc a,(ix+d)
	z80_afir	adca_ixd_, ix, {adc.s a, (hl)}

	; DD 94 sub a, ixh
	z80_exaf2	subaixh, {sub a, (ix+z80_reg_ix+1)}

	; DD 95 sub a, ixl
	z80_exaf2	subaixl, {sub a, (ix+z80_reg_ix+0)}

	; DD 96 sub a, (ix+d)
	z80_afir	suba_ixd_, ix, {db %52}, {sub a, (hl)} 	; bug in assembler sub.s a, (hl)

	; DD 9C sbc a, ixh
	z80_exaf2	sbcaixh, {sbc a, (ix+z80_reg_ix+1)}

	; DD 9D abc a, ixl
	z80_exaf2	sbcaixl, {sbc a, (ix+z80_reg_ix+0)}

	; DD 9E sbc a, (ix+d)
	z80_afir	sbca_ixd_, ix, {sbc.s a, (hl)}

	; DD A4 and a, ixh
	z80_exaf2	andaixh, {and a, (ix+z80_reg_ix+1)}

	; DD A5 and a, ixl
	z80_exaf2	andaixl, {and a, (ix+z80_reg_ix+0)}

	; DD A6 and a, (ix+d)
	z80_afir	anda_ixd_, ix, {and.s a, (hl)}

	; DD AC xor a, ixh
	z80_exaf2	xoraixh, {xor a, (ix+z80_reg_ix+1)}

	; DD AD xor a, ixl
	z80_exaf2	xoraixl, {xor a, (ix+z80_reg_ix+0)}

	; DD AE xor a, (ix+d)
	z80_afir	xora_ixd_, ix, {xor.s a, (hl)}

	; DD B4 or a, ixh
	z80_exaf2	oraixh, {or a, (ix+z80_reg_ix+1)}

	; DD B5 or a, ixl
	z80_exaf2	oraixl, {or a, (ix+z80_reg_ix+0)}

	; DD B6 or a, (ix+d)
	z80_afir	ora_ixd_, ix, {or.s a, (hl)}

	; DD BC cp a, ixh
	z80_exaf2	cpaixh, {cp a, (ix+z80_reg_ix+1)}

	; DD BD cp a, ixl
	z80_exaf2	cpaixl, {cp a, (ix+z80_reg_ix+0)}

	; DD BE cp a, (ix+d)
	z80_afir	cpa_ixd_, ix, {cp.s a, (hl)}

	; DD CB ix bit operators
	; DD, CB, dd, kk -> 52, DD, CB, dd, kk

z80_ixbits:
	ld.s	bc, (iy)
	inc	iy
	inc	iy

	ld	a, b		; kk
	cp	%36		; undocumented sl1 not implemented
	jr	z, z80_ixbits_nop
	and	%07		; if 6 or e ok, otherwise nop
	cp	%06
	jr	nz, z80_ixbits_nop

	ld	hl, dd_bit_instr
	ld	(hl), c
	inc	hl
	ld	(hl), b
	ld	ix, (ix+z80_reg_ix)
	exx
	ex	af, af'
	jp	z80_ixbits2

z80_ixbits_nop:
	z80loop

	; $DD E1 pop ix
	z80_popir	ix

	; DD E3 ex (sp), ix
z80_ex_sp_ix:
	ld	hl, (ix+z80_reg_ix)
	ex.s	(sp), hl
	ld	(ix+z80_reg_ix), hl
	z80loop

	; $DD $E5 push ix
	z80_pushir ix

	; $DD $E9 jp (ix)
	z80_jp_ir_ ix

	; DD F9 ld sp, ix
z80_spix:
	ld	hl, (ix+z80_reg_ix)
	ld.s	sp, hl
	z80loop

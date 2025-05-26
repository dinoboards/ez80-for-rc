	section	INTERNAL_RAM_ROM
	; DD ....
z80_ix:
	z80_byte_jump	ix_instr_table
	section CODE

ix_instr_table:
	jp	z80_nop			; DD 00
	jp	z80_nop			; DD 01
	jp	z80_nop			; DD 02
	jp	z80_nop			; DD 03
	jp	z80_nop			; DD 04
	jp	z80_nop			; DD 05
	jp	z80_nop			; DD 06
	jp	z80_ldbc_ixd_		; DD 07 ld bc, (ix+d)
	jp	z80_nop			; DD 08
	jp	z80_addixbc		; DD 09 add ix,bc
	jp	z80_nop			; DD 0A
	jp	z80_nop			; DD 0B
	jp	z80_nop			; DD 0C
	jp	z80_nop			; DD 0D
	jp	z80_nop			; DD 0E
	jp	z80_ld_ixd_bc		; DD 0F ld (ix+d), bc
	jp	z80_nop			; DD 10
	jp	z80_nop			; DD 11
	jp	z80_nop			; DD 12
	jp	z80_nop			; DD 13
	jp	z80_nop			; DD 14
	jp	z80_nop			; DD 15
	jp	z80_nop			; DD 16
	jp	z80_ldde_ixd_		; DD 17 ld de, (ix+d)
	jp	z80_nop			; DD 18
	jp	z80_addixde		; DD 19 add ix,de
	jp	z80_nop			; DD 1A
	jp	z80_nop			; DD 1B
	jp	z80_nop			; DD 1C
	jp	z80_nop			; DD 1D
	jp	z80_nop			; DD 1E
	jp	z80_ld_ixd_de		; DD 1F ld (ix+d), de
	jp	z80_nop			; DD 20
	jp	z80_ldixnn		; DD 21 ld ix,nn
	jp	z80_ld_nn_ix		; DD 22 ld (nn),ix
	jp	z80_incix		; DD 23 inc ix
	jp	z80_incixh		; DD 24 inc ixh
	jp	z80_decixh		; DD 25 dec ixl
	jp	z80_ldixhn		; DD 26 ld ixh, n
	jp	z80_ldhl_ixd_		; DD 27 ld hl, (ix+d)
	jp	z80_nop			; DD 28
	jp	z80_addixix		; DD 29 add ix,ix
	jp	z80_ldix_nn_		; DD 2A ld ix,(nn)
	jp	z80_decix		; DD 2B dec ix
	jp	z80_incixl		; DD 2C inc ixl
	jp	z80_decixl		; DD 2D dec ixl
	jp	z80_ldixln		; DD 2E ld ixl, n
	jp	z80_ld_ixd_hl		; DD 2F ld (ix+d), hl
	jp	z80_nop			; DD 30
	jp	z80_ldiy_ixd_		; DD 31 ld iy, (ix+d)
	jp	z80_nop			; DD 32
	jp	z80_nop			; DD 33
	jp	z80_inc_ixd_		; DD 34 inc (ix+d)
	jp	z80_dec_ixd_		; DD 35 dec (ix+d)
	jp	z80_ld_ixd_n_		; DD 36 ld (ix+d),n
	jp	z80_ldix_ixd_		; DD 37 ld ix, (ix+d)
	jp	z80_nop			; DD 38
	jp	z80_addixsp		; DD 39 add ix,sp
	jp	z80_nop			; DD 3A
	jp	z80_nop			; DD 3B
	jp	z80_nop			; DD 3C
	jp	z80_nop			; DD 3D
	jp	z80_ld_ixd_iy		; DD 3E ld (ix+d), iy
	jp	z80_ld_ixd_ix		; DD 3F ld (ix+d), ix
	jp	z80_nop			; DD 40
	jp	z80_nop			; DD 41
	jp	z80_nop			; DD 42
	jp	z80_nop			; DD 43
	jp	z80_ldbixh		; DD 44 ld b, ixh
	jp	z80_ldbixl		; DD 45 ld b, ixl
	jp	z80_ldb_ixd_		; DD 46 ld b,(ix+d)
	jp	z80_nop			; DD 47
	jp	z80_nop			; DD 48
	jp	z80_nop			; DD 49
	jp	z80_nop			; DD 4A
	jp	z80_nop			; DD 4B
	jp	z80_ldcixh		; DD 4C ld c, ixh
	jp	z80_ldcixl		; DD 4D ld c, ixl
	jp	z80_ldc_ixd_		; DD 4E ld c,(ix+d)
	jp	z80_nop			; DD 4F
	jp	z80_nop			; DD 50
	jp	z80_nop			; DD 51
	jp	z80_nop			; DD 52
	jp	z80_nop			; DD 53
	jp	z80_lddixh		; DD 54 ld d, ixh
	jp	z80_lddixl		; DD 55 ld d, ixl
	jp	z80_ldd_ixd_		; DD 56 ld d,(ix+d)
	jp	z80_nop			; DD 57
	jp	z80_nop			; DD 58
	jp	z80_nop			; DD 59
	jp	z80_nop			; DD 5A
	jp	z80_nop			; DD 5B
	jp	z80_ldeixh		; DD 5C ld e, ixh
	jp	z80_ldeixl		; DD 5D ld e, ixl
	jp	z80_lde_ixd_		; DD 5E ld e,(ix+d)
	jp	z80_nop			; DD 5F
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
	jp	z80_nop			; DD 76
	jp	z80_ld_ixd_a		; DD 77 ld (ix+d),a
	jp	z80_nop			; DD 78
	jp	z80_nop			; DD 79
	jp	z80_nop			; DD 7A
	jp	z80_nop			; DD 7B
	jp	z80_ldaixh		; DD 7C ld a, ixh
	jp	z80_ldaixl		; DD 7D ld a, ixl
	jp	z80_lda_ixd_		; DD 7E ld a,(ix+d)
	jp	z80_nop			; DD 7F
	jp	z80_nop			; DD 80
	jp	z80_nop			; DD 81
	jp	z80_nop			; DD 82
	jp	z80_nop			; DD 83
	jp	z80_addaixh		; DD 84 add a, ixh
	jp	z80_addaixl		; DD 85 add a, ixl
	jp	z80_adda_ixd_		; DD 86 add a,(ix+d)
	jp	z80_nop			; DD 87
	jp	z80_nop			; DD 88
	jp	z80_nop			; DD 89
	jp	z80_nop			; DD 8A
	jp	z80_nop			; DD 8B
	jp	z80_adcaixh		; DD 8C adc a, ixh
	jp	z80_adcaixl		; DD 8D adc a, ixl
	jp	z80_adca_ixd_		; DD 8E adc a,(ix+d)
	jp	z80_nop			; DD 8F
	jp	z80_nop			; DD 90
	jp	z80_nop			; DD 91
	jp	z80_nop			; DD 92
	jp	z80_nop			; DD 93
	jp	z80_subaixh		; DD 94 sub a, ixh
	jp	z80_subaixl		; DD 95 sub a, ixl
	jp	z80_suba_ixd_		; DD 96 sub a, (ix+d)
	jp	z80_nop			; DD 97
	jp	z80_nop			; DD 98
	jp	z80_nop			; DD 99
	jp	z80_nop			; DD 9A
	jp	z80_nop			; DD 9B
	jp	z80_sbcaixh		; DD 9C sbc a, ixh
	jp	z80_sbcaixl		; DD 9D abc a, ixl
	jp	z80_sbca_ixd_		; DD 9E sbc a, (ix+d)
	jp	z80_nop			; DD 9F
	jp	z80_nop			; DD A0
	jp	z80_nop			; DD A1
	jp	z80_nop			; DD A2
	jp	z80_nop			; DD A3
	jp	z80_andaixh		; DD A4 and a, ixh
	jp	z80_andaixl		; DD A5 and a, ixl
	jp	z80_anda_ixd_		; DD A6 and a, (ix+d)
	jp	z80_nop			; DD A7
	jp	z80_nop			; DD A8
	jp	z80_nop			; DD A9
	jp	z80_nop			; DD AA
	jp	z80_nop			; DD AB
	jp	z80_xoraixh		; DD AC xor a, ixh
	jp	z80_xoraixl		; DD AD xor a, ixl
	jp	z80_xora_ixd_		; DD AE xor a, (ix+d)
	jp	z80_nop			; DD AF
	jp	z80_nop			; DD B0
	jp	z80_nop			; DD B1
	jp	z80_nop			; DD B2
	jp	z80_nop			; DD B3
	jp	z80_oraixh		; DD B4 or a, ixh
	jp	z80_oraixl		; DD B5 or a, ixl
	jp	z80_ora_ixd_		; DD B6 or a, (ix+d)
	jp	z80_nop			; DD B7
	jp	z80_nop			; DD B8
	jp	z80_nop			; DD B9
	jp	z80_nop			; DD BA
	jp	z80_nop			; DD BB
	jp	z80_cpaixh		; DD BC cp a, ixh
	jp	z80_cpaixl		; DD BD cp a, ixl
	jp	z80_cpa_ixd_		; DD BE cp a, (ix+d)
	jp	z80_nop			; DD BF
	jp	z80_nop			; DD C0
	jp	z80_nop			; DD C1
	jp	z80_nop			; DD C2
	jp	z80_nop			; DD C3
	jp	z80_nop			; DD C4
	jp	z80_nop			; DD C5
	jp	z80_nop			; DD C6
	jp	z80_nop			; DD C7
	jp	z80_nop			; DD C8
	jp	z80_nop			; DD C9
	jp	z80_nop			; DD CA
	jp	z80_ixbits		; DD CB
	jp	z80_nop			; DD CC
	jp	z80_nop			; DD CD
	jp	z80_nop			; DD CE
	jp	z80_nop			; DD CF
	jp	z80_nop			; DD D0
	jp	z80_nop			; DD D1
	jp	z80_nop			; DD D2
	jp	z80_nop			; DD D3
	jp	z80_nop			; DD D4
	jp	z80_nop			; DD D5
	jp	z80_nop			; DD D6
	jp	z80_nop			; DD D7
	jp	z80_nop			; DD D8
	jp	z80_nop			; DD D9
	jp	z80_nop			; DD DA
	jp	z80_nop			; DD DB
	jp	z80_nop			; DD DC
	jp	z80_nop			; DD DD
	jp	z80_nop			; DD DE
	jp	z80_nop			; DD DF
	jp	z80_nop			; DD E0
	jp	z80_popix		; DD E1 pop ix
	jp	z80_nop			; DD E2
	jp	z80_ex_sp_ix		; DD E3 ex (sp), ix
	jp	z80_nop			; DD E4
	jp	z80_pushix		; DD E5 push ix
	jp	z80_nop			; DD E6
	jp	z80_nop			; DD E7
	jp	z80_nop			; DD E8
	jp	z80_jp_ix_		; DD E9 jp (ix)
	jp	z80_nop			; DD EA
	jp	z80_nop			; DD EB
	jp	z80_nop			; DD EC
	jp	z80_nop			; DD ED
	jp	z80_nop			; DD EE
	jp	z80_nop			; DD EF
	jp	z80_nop			; DD F0
	jp	z80_nop			; DD F1
	jp	z80_nop			; DD F2
	jp	z80_nop			; DD F3
	jp	z80_nop			; DD F4
	jp	z80_nop			; DD F5
	jp	z80_nop			; DD F6
	jp	z80_nop			; DD F7
	jp	z80_nop			; DD F8
	jp	z80_spix		; DD F9 ld sp, ix
	jp	z80_nop			; DD FA
	jp	z80_nop			; DD FB
	jp	z80_nop			; DD FC
	jp	z80_nop			; DD DD
	jp	z80_nop			; DD FE
	jp	z80_nop			; DD FF

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
	z80_niy	ld_nn_ix

	; DD 23 inc ix
	z80_niy	incix

	; DD 24 inc ixh
	z80_niy	incixh

	; DD 25 dec ixl
	z80_niy	decixh

	; DD 26 ld ixh, n
	z80_niy	ldixhn

	; DD 27 ld hl, (ix+d)
	z80_niy	ldhl_ixd_

	; $fd $29 add ix, ix
	z80_addirir	ix

	; DD 2A ld ix,(nn)
	z80_niy	ldix_nn_

	; DD 2B dec ix
	z80_niy	decix

	; DD 2C inc ixl
	z80_niy	incixl

	; DD 2D dec ixl
	z80_niy	decixl

	; DD 2E ld ixl, n
	z80_niy	ldixln

	; DD 2F ld (ix+d), hl
	z80_niy	ld_ixd_hl

	; DD 31 ld iy, (ix+d)
	z80_niy	ldiy_ixd_

	; DD 34 inc (ix+d)
	z80_niy	inc_ixd_

	; DD 35 dec (ix+d)
	z80_niy	dec_ixd_

	; DD 36 ld (ix+d),n
	z80_niy	ld_ixd_n_

	; DD 37 ld ix, (ix+d)
	z80_niy	ldix_ixd_

	; $DD $39 add ix,sp
	z80_addirsp	ix

	; DD 3E ld (ix+d), iy
	z80_niy	ld_ixd_iy

	; DD 3F ld (ix+d), ix
	z80_niy	ld_ixd_ix

	; DD 44 ld b, ixh
	z80_niy	ldbixh

	; DD 45 ld b, ixl
	z80_niy	ldbixl

	; DD 46 ld b,(ix+d)
	z80_niy	ldb_ixd_

	; DD 4C ld c, ixh
	z80_niy	ldcixh

	; DD 4D ld c, ixl
	z80_niy	ldcixl

	; DD 4E ld c,(ix+d)
	z80_niy	ldc_ixd_

	; DD 54 ld d, ixh
	z80_niy	lddixh

	; DD 55 ld d, ixl
	z80_niy	lddixl

	; DD 56 ld d,(ix+d)
	z80_niy	ldd_ixd_

	; DD 5C ld e, ixh
	z80_niy	ldeixh

	; DD 5D ld e, ixl
	z80_niy	ldeixl

	; DD 5E ld e,(ix+d)
	z80_niy	lde_ixd_

	; DD 60 ld ixh, b
	z80_niy	ldixhb

	; DD 61 ld ixh, c
	z80_niy	ldixhc

	; DD 62 ld ixh, d
	z80_niy	ldixhd

	; DD 63 ld ixh, e
	z80_niy	ldixhe

	; DD 64 ld ixh, ixh
	z80_niy	ldixhixh

	; DD 65 ld ixh, ixl
	z80_niy	ldixhixl

	; DD 66 ld h,(ix+d)
	z80_niy	ldh_ixd_

	; DD 67 ld ixh, a
	z80_niy	ldixha

	; DD 68 ld ixl, b
	z80_niy	ldixlb

	; DD 69 ld ixl, c
	z80_niy	ldixlc

	; DD 6A ld ixl, d
	z80_niy	ldixld

	; DD 6B ld ixl, e
	z80_niy	ldixle

	; DD 6C ld ixl, ixh
	z80_niy	ldixlixh

	; DD 6D ld ixl, ixl
	z80_niy	ldixlixl

	; DD 6E ld l,(ix+d)
	z80_niy	ldl_ixd_

	; DD 6F ld ixl, a
	z80_niy	ldixla

	; DD 70 ld (ix+d),b
	z80_niy	ld_ixd_b

	; DD 71 ld (ix+d),c
	z80_niy	ld_ixd_c

	; DD 72 ld (ix+d),d
	z80_niy	ld_ixd_d

	; DD 73 ld (ix+d),e
	z80_niy	ld_ixd_e

	; DD 74 ld (ix+d),h
	z80_niy	ld_ixd_h

	; DD 75 ld (ix+d),l
	z80_niy	ld_ixd_l

	; DD 77 ld (ix+d),a
	z80_niy	ld_ixd_a

	; DD 7C ld a, ixh
	z80_niy	ldaixh

	; DD 7D ld a, ixl
	z80_niy	ldaixl

	; DD 7E ld a,(ix+d)
	z80_niy	lda_ixd_

	; DD 84 add a, ixh
	z80_exaf2	addaixh, {add a, (ix+z80_reg_ix+1)}

	; DD 85 add a, ixl
	z80_exaf2	addaixl, {add a, (ix+z80_reg_ix+0)}

	; DD 86 add a,(ix+d)
	z80_niy	adda_ixd_

	; DD 8C adc a, ixh
	z80_exaf2	adcaixh, {adc a, (ix+z80_reg_ix+1)}

	; DD 8D adc a, ixl
	z80_exaf2	adcaixl, {adc a, (ix+z80_reg_ix+0)}

	; DD 8E adc a,(ix+d)
	z80_niy	adca_ixd_

	; DD 94 sub a, ixh
	z80_niy	subaixh

	; DD 95 sub a, ixl
	z80_niy	subaixl

	; DD 96 sub a, (ix+d)
	z80_niy	suba_ixd_

	; DD 9C sbc a, ixh
	z80_niy	sbcaixh

	; DD 9D abc a, ixl
	z80_niy	sbcaixl

	; DD 9E sbc a, (ix+d)
	z80_niy	sbca_ixd_

	; DD A4 and a, ixh
	z80_niy	andaixh

	; DD A5 and a, ixl
	z80_niy	andaixl

	; DD A6 and a, (ix+d)
	z80_niy	anda_ixd_

	; DD AC xor a, ixh
	z80_niy	xoraixh

	; DD AD xor a, ixl
	z80_niy	xoraixl

	; DD AE xor a, (ix+d)
	z80_niy	xora_ixd_

	; DD B4 or a, ixh
	z80_niy	oraixh

	; DD B5 or a, ixl
	z80_niy	oraixl

	; DD B6 or a, (ix+d)
	z80_niy	ora_ixd_

	; DD BC cp a, ixh
	z80_niy	cpaixh

	; DD BD cp a, ixl
	z80_niy	cpaixl

	; DD BE cp a, (ix+d)
	z80_niy	cpa_ixd_

	; DD CB ix bit operators
	z80_niy	ixbits

	; $DD E1 pop ix
	z80_popir	ix

	; DD E3 ex (sp), ix
	z80_niy	ex_sp_ix

	; $DD $E5 push ix
	z80_pushir ix

	; $DD $E9 jp (ix)
	z80_jp_ir_ ix

	; DD F9 ld sp, ix
	z80_niy	spix

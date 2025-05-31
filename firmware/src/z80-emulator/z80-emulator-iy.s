	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

	section	CODE

	xref	z80_nop
	xref	z80_regs
	XREF	z80_reg_iy
	xref	fd_bit_instr
	xref	z80_iybits2

regir	equ	z80_reg_iy

	; FD ....
	global	iy_instr_table
iy_instr_table:
	jp	z80_nop			; FD 00
	jp	z80_nop			; FD 01
	jp	z80_nop			; FD 02
	jp	z80_nop			; FD 03
	jp	z80_nop			; FD 04
	jp	z80_nop			; FD 05
	jp	z80_nop			; FD 06
	jp	z80_ldbc_iyd_		; FD 07 ld bc, (iy+d)
	jp	z80_nop			; FD 08
	jp	z80_addiybc		; FD 09 add iy,bc
	jp	z80_nop			; FD 0A
	jp	z80_nop			; FD 0B
	jp	z80_nop			; FD 0C
	jp	z80_nop			; FD 0D
	jp	z80_nop			; FD 0E
	jp	z80_ld_iyd_bc		; FD 0F ld (iy+d), bc
	jp	z80_nop			; FD 10
	jp	z80_nop			; FD 11
	jp	z80_nop			; FD 12
	jp	z80_nop			; FD 13
	jp	z80_nop			; FD 14
	jp	z80_nop			; FD 15
	jp	z80_nop			; FD 16
	jp	z80_ldde_iyd_		; FD 17 ld de, (iy+d)
	jp	z80_nop			; FD 18
	jp	z80_addiyde		; FD 19 add iy,de
	jp	z80_nop			; FD 1A
	jp	z80_nop			; FD 1B
	jp	z80_nop			; FD 1C
	jp	z80_nop			; FD 1D
	jp	z80_nop			; FD 1E
	jp	z80_ld_iyd_de		; FD 1F ld (iy+d), de
	jp	z80_nop			; FD 20
	jp	z80_ldiynn		; FD 21 ld iy,nn
	jp	z80_ld_nn_iy		; FD 22 ld (nn),iy
	jp	z80_inciy		; FD 23 inc iy
	jp	z80_inciyh		; FD 24 inc iyh
	jp	z80_deciyh		; FD 25 dec iyl
	jp	z80_ldiyhn		; FD 26 ld iyh, n
	jp	z80_ldhl_iyd_		; FD 27 ld hl, (iy+d)
	jp	z80_nop			; FD 28
	jp	z80_addiyiy		; FD 29 add iy,iy
	jp	z80_ldiy_nn_		; FD 2A ld iy,(nn)
	jp	z80_deciy		; FD 2B dec iy
	jp	z80_inciyl		; FD 2C inc iyl
	jp	z80_deciyl		; FD 2D dec iyl
	jp	z80_ldiyln		; FD 2E ld iyl, n
	jp	z80_ld_iyd_hl		; FD 2F ld (iy+d), hl
	jp	z80_nop			; FD 30
	jp	z80_ldix_iyd_		; FD 31 ld ix, (iy+d)
	jp	z80_nop			; FD 32
	jp	z80_nop			; FD 33
	jp	z80_inc_iyd_		; FD 34 inc (iy+d)
	jp	z80_dec_iyd_		; FD 35 dec (iy+d)
	jp	z80_ld_iyd_n_		; FD 36 ld (iy+d),n
	jp	z80_ldiy_iyd_		; FD 37 ld iy, (iy+d)
	jp	z80_nop			; FD 38
	jp	z80_addiysp		; FD 39 add iy,sp
	jp	z80_nop			; FD 3A
	jp	z80_nop			; FD 3B
	jp	z80_nop			; FD 3C
	jp	z80_nop			; FD 3D
	jp	z80_ld_iyd_ix		; FD 3E ld (iy+d), ix
	jp	z80_ld_iyd_iy		; FD 3F ld (iy+d), iy
	jp	z80_nop			; FD 40
	jp	z80_nop			; FD 41
	jp	z80_nop			; FD 42
	jp	z80_nop			; FD 43
	jp	z80_ldbiyh		; FD 44 ld b, iyh
	jp	z80_ldbiyl		; FD 45 ld b, iyl
	jp	z80_ldb_iyd_		; FD 46 ld b,(iy+d)
	jp	z80_nop			; FD 47
	jp	z80_nop			; FD 48
	jp	z80_nop			; FD 49
	jp	z80_nop			; FD 4A
	jp	z80_nop			; FD 4B
	jp	z80_ldciyh		; FD 4C ld c, iyh
	jp	z80_ldciyl		; FD 4D ld c, iyl
	jp	z80_ldc_iyd_		; FD 4E ld c,(iy+d)
	jp	z80_nop			; FD 4F
	jp	z80_nop			; FD 50
	jp	z80_nop			; FD 51
	jp	z80_nop			; FD 52
	jp	z80_nop			; FD 53
	jp	z80_lddiyh		; FD 54 ld d, iyh
	jp	z80_lddiyl		; FD 55 ld d, iyl
	jp	z80_ldd_iyd_		; FD 56 ld d,(iy+d)
	jp	z80_nop			; FD 57
	jp	z80_nop			; FD 58
	jp	z80_nop			; FD 59
	jp	z80_nop			; FD 5A
	jp	z80_nop			; FD 5B
	jp	z80_ldeiyh		; FD 5C ld e, iyh
	jp	z80_ldeiyl		; FD 5D ld e, iyl
	jp	z80_lde_iyd_		; FD 5E ld e,(iy+d)
	jp	z80_nop			; FD 5F
	jp	z80_ldiyhb		; FD 60 ld iyh, b
	jp	z80_ldiyhc		; FD 61 ld iyh, c
	jp	z80_ldiyhd		; FD 62 ld iyh, d
	jp	z80_ldiyhe		; FD 63 ld iyh, e
	jp	z80_ldiyhiyh		; FD 64 ld iyh, iyh
	jp	z80_ldiyhiyl		; FD 65 ld iyh, iyl
	jp	z80_ldh_iyd_		; FD 66 ld h,(iy+d)
	jp	z80_ldiyha		; FD 67 ld iyh, a
	jp	z80_ldiylb		; FD 68 ld iyl, b
	jp	z80_ldiylc		; FD 69 ld iyl, c
	jp	z80_ldiyld		; FD 6A ld iyl, d
	jp	z80_ldiyle		; FD 6B ld iyl, e
	jp	z80_ldiyliyh		; FD 6C ld iyl, iyh
	jp	z80_ldiyliyl		; FD 6D ld iyl, iyl
	jp	z80_ldl_iyd_		; FD 6E ld l,(iy+d)
	jp	z80_ldiyla		; FD 6F ld iyl, a
	jp	z80_ld_iyd_b		; FD 70 ld (iy+d),b
	jp	z80_ld_iyd_c		; FD 71 ld (iy+d),c
	jp	z80_ld_iyd_d		; FD 72 ld (iy+d),d
	jp	z80_ld_iyd_e		; FD 73 ld (iy+d),e
	jp	z80_ld_iyd_h		; FD 74 ld (iy+d),h
	jp	z80_ld_iyd_l		; FD 75 ld (iy+d),l
	jp	z80_nop			; FD 76
	jp	z80_ld_iyd_a		; FD 77 ld (iy+d),a
	jp	z80_nop			; FD 78
	jp	z80_nop			; FD 79
	jp	z80_nop			; FD 7A
	jp	z80_nop			; FD 7B
	jp	z80_ldaiyh		; FD 7C ld a, iyh
	jp	z80_ldaiyl		; FD 7D ld a, iyl
	jp	z80_lda_iyd_		; FD 7E ld a,(iy+d)
	jp	z80_nop			; FD 7F
	jp	z80_nop			; FD 80
	jp	z80_nop			; FD 81
	jp	z80_nop			; FD 82
	jp	z80_nop			; FD 83
	jp	z80_addaiyh		; FD 84 add a, iyh
	jp	z80_addaiyl		; FD 85 add a, iyl
	jp	z80_adda_iyd_		; FD 86 add a,(iy+d)
	jp	z80_nop			; FD 87
	jp	z80_nop			; FD 88
	jp	z80_nop			; FD 89
	jp	z80_nop			; FD 8A
	jp	z80_nop			; FD 8B
	jp	z80_adcaiyh		; FD 8C adc a, iyh
	jp	z80_adcaiyl		; FD 8D adc a, iyl
	jp	z80_adca_iyd_		; FD 8E adc a,(iy+d)
	jp	z80_nop			; FD 8F
	jp	z80_nop			; FD 90
	jp	z80_nop			; FD 91
	jp	z80_nop			; FD 92
	jp	z80_nop			; FD 93
	jp	z80_subaiyh		; FD 94 sub a, iyh
	jp	z80_subaiyl		; FD 95 sub a, iyl
	jp	z80_suba_iyd_		; FD 96 sub a, (iy+d)
	jp	z80_nop			; FD 97
	jp	z80_nop			; FD 98
	jp	z80_nop			; FD 99
	jp	z80_nop			; FD 9A
	jp	z80_nop			; FD 9B
	jp	z80_sbcaiyh		; FD 9C sbc a, iyh
	jp	z80_sbcaiyl		; FD 9D abc a, iyl
	jp	z80_sbca_iyd_		; FD 9E sbc a, (iy+d)
	jp	z80_nop			; FD 9F
	jp	z80_nop			; FD A0
	jp	z80_nop			; FD A1
	jp	z80_nop			; FD A2
	jp	z80_nop			; FD A3
	jp	z80_andaiyh		; FD A4 and a, iyh
	jp	z80_andaiyl		; FD A5 and a, iyl
	jp	z80_anda_iyd_		; FD A6 and a, (iy+d)
	jp	z80_nop			; FD A7
	jp	z80_nop			; FD A8
	jp	z80_nop			; FD A9
	jp	z80_nop			; FD AA
	jp	z80_nop			; FD AB
	jp	z80_xoraiyh		; FD AC xor a, iyh
	jp	z80_xoraiyl		; FD AD xor a, iyl
	jp	z80_xora_iyd_		; FD AE xor a, (iy+d)
	jp	z80_nop			; FD AF
	jp	z80_nop			; FD B0
	jp	z80_nop			; FD B1
	jp	z80_nop			; FD B2
	jp	z80_nop			; FD B3
	jp	z80_oraiyh		; FD B4 or a, iyh
	jp	z80_oraiyl		; FD B5 or a, iyl
	jp	z80_ora_iyd_		; FD B6 or a, (iy+d)
	jp	z80_nop			; FD B7
	jp	z80_nop			; FD B8
	jp	z80_nop			; FD B9
	jp	z80_nop			; FD BA
	jp	z80_nop			; FD BB
	jp	z80_cpaiyh		; FD BC cp a, iyh
	jp	z80_cpaiyl		; FD BD cp a, iyl
	jp	z80_cpa_iyd_		; FD BE cp a, (iy+d)
	jp	z80_nop			; FD BF
	jp	z80_nop			; FD C0
	jp	z80_nop			; FD C1
	jp	z80_nop			; FD C2
	jp	z80_nop			; FD C3
	jp	z80_nop			; FD C4
	jp	z80_nop			; FD C5
	jp	z80_nop			; FD C6
	jp	z80_nop			; FD C7
	jp	z80_nop			; FD C8
	jp	z80_nop			; FD C9
	jp	z80_nop			; FD CA
	jp	z80_iybits		; FD CB
	jp	z80_nop			; FD CC
	jp	z80_nop			; FD CD
	jp	z80_nop			; FD CE
	jp	z80_nop			; FD CF
	jp	z80_nop			; FD D0
	jp	z80_nop			; FD D1
	jp	z80_nop			; FD D2
	jp	z80_nop			; FD D3
	jp	z80_nop			; FD D4
	jp	z80_nop			; FD D5
	jp	z80_nop			; FD D6
	jp	z80_nop			; FD D7
	jp	z80_nop			; FD D8
	jp	z80_nop			; FD D9
	jp	z80_nop			; FD DA
	jp	z80_nop			; FD DB
	jp	z80_nop			; FD DC
	jp	z80_nop			; FD DD
	jp	z80_nop			; FD DE
	jp	z80_nop			; FD DF
	jp	z80_nop			; FD E0
	jp	z80_popiy		; FD E1 pop iy
	jp	z80_nop			; FD E2
	jp	z80_ex_sp_iy		; FD E3 ex (sp), iy
	jp	z80_nop			; FD E4
	jp	z80_pushiy		; FD E5 push iy
	jp	z80_nop			; FD E6
	jp	z80_nop			; FD E7
	jp	z80_nop			; FD E8
	jp	z80_jp_iy_		; FD E9 jp (iy)
	jp	z80_nop			; FD EA
	jp	z80_nop			; FD EB
	jp	z80_nop			; FD EC
	jp	z80_nop			; FD ED
	jp	z80_nop			; FD EE
	jp	z80_nop			; FD EF
	jp	z80_nop			; FD F0
	jp	z80_nop			; FD F1
	jp	z80_nop			; FD F2
	jp	z80_nop			; FD F3
	jp	z80_nop			; FD F4
	jp	z80_nop			; FD F5
	jp	z80_nop			; FD F6
	jp	z80_nop			; FD F7
	jp	z80_nop			; FD F8
	jp	z80_spiy		; FD F9 ld sp, iy
	jp	z80_nop			; FD FA
	jp	z80_nop			; FD FB
	jp	z80_nop			; FD FC
	jp	z80_nop			; FD FD
	jp	z80_nop			; FD FE
	jp	z80_nop			; FD FF

	; $FD 07 ld bc, (iy+d)
	z80_niy	ldbc_iyd_

	; $FD 09 add iy, bc
	z80_addirrr	iy, bc

	; $FD 0F ld (iy+d), bc
	z80_niy	ld_iyd_bc

	; FD 17 ld de, (iy+d)
	z80_niy ldde_iyd_

	; $FD 19 add iy, de
	z80_addirrr	iy, de

	; FD 1F ld (iy+d), de
	z80_niy	ld_iyd_de

	; $FD 21 ld iy, nn
	z80_ldirnn	iy

	; FD 22 ld (nn),iy
z80_ld_nn_iy:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld	bc, (ix+z80_reg_iy)
	ld.s	(hl), bc
	z80loop

	; FD 23 inc iy
z80_inciy:
	ld	hl, (ix+z80_reg_iy)
	inc	hl
	ld	(ix+z80_reg_iy), hl
	z80loop

	; FD 24 inc iyh
	z80_exaf2	inciyh, {inc (ix+z80_reg_iy+1)}

	; FD 25 dec iyh
	z80_exaf2	deciyh, {dec (ix+z80_reg_iy+1)}

	; FD 26 ld iyh, n
	z80_niy	ldiyhn

	; FD 27 ld hl, (iy+d)
	z80_niy	ldhl_iyd_

	; $fd $29 add iy, iy
	z80_addirir	iy

	; FD 2A ld iy, (nn)
z80_ldiy_nn_:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld.s	hl, (hl)
	ld	(ix+z80_reg_iy), hl
	z80loop

	; FD 2B dec iy
z80_deciy:
	ld	hl, (ix+z80_reg_iy)
	dec	hl
	ld	(ix+z80_reg_iy), hl
	z80loop

	; FD 2C inc iyl
	z80_exaf2	inciyl, {inc (ix+z80_reg_iy+0)}

	; FD 2D dec iyl
	z80_exaf2	deciyl, {dec (ix+z80_reg_iy+0)}

	; FD 2E ld iyl, n
	z80_niy	ldiyln

	; FD 2F ld (iy+d), hl
	z80_niy	ld_iyd_hl

	; FD 31 ld ix, (iy+d)
	z80_niy	ldix_iyd_

	; FD 34 inc (iy+d)
	z80_afir	inc_iyd_, iy, {inc.s (hl)}

	; FD 35 dec (iy+d)
	z80_afir	dec_iyd_, iy, {dec.s (hl)}

	; FD 36 ld (iy+d),n
	z80_irtohl	ld_iyd_n_, iy, {ld.s a, (iy)}, {inc iy}, {ld.s (hl), a}

	; FD 37 ld iy, (iy+d)
	z80_niy	ldiy_iyd_

	; $FD $39 add iy,sp
	z80_addirsp	iy

	; FD 3E ld (iy+d), ix
	z80_niy	ld_iyd_ix

	; FD 3F ld (iy+d), iy
	z80_niy	ld_iyd_iy

	; FD 44 ld b, iyh
	z80_niy	ldbiyh

	; FD 45 ld b, iyl
	z80_niy	ldbiyl

	; FD 46 ld b,(iy+d)
	z80_irtoix	ldb_iyd_, iy, {ld.s b, (ix)}

	; FD 4C ld c, iyh
	z80_niy	ldciyh

	; FD 4D ld c, iyl
	z80_niy	ldciyl

	; FD 4E ld c,(iy+d)
	z80_irtoix	ldc_iyd_, iy, {ld.s c, (ix)}

	; FD 54 ld d, iyh
	z80_niy	lddiyh

	; FD 55 ld d, iyl
	z80_niy	lddiyl

	; FD 56 ld d,(iy+d)
	z80_irtoix	ldd_iyd_, iy, {ld.s d, (ix)}

	; FD 5C ld e, iyh
	z80_niy	ldeiyh

	; FD 5D ld e, iyl
	z80_niy	ldeiyl

	; FD 5E ld e,(iy+d)
	z80_irtoix	lde_iyd_, iy, {ld.s e, (ix)}

	; FD 60 ld iyh, b
	z80_exmain2	ldiyhb, {ld (ix+z80_reg_iy+1), b}

	; FD 61 ld iyh, c
	z80_exmain2	ldiyhc, {ld (ix+z80_reg_iy+1), c}

	; FD 62 ld iyh, d
	z80_exmain2	ldiyhd, {ld (ix+z80_reg_iy+1), d}

	; FD 63 ld iyh, e
	z80_exmain2	ldiyhe, {ld (ix+z80_reg_iy+1), e}

	; FD 64 ld iyh, iyh
	z80_niy	ldiyhiyh

	; FD 65 ld iyh, iyl
	z80_exmain2	ldiyhiyl, {ld a, (ix+z80_reg_iy+0)}, {ld (ix+z80_reg_iy+1), a}

	; FD 66 ld h,(iy+d)
	z80_irtoix	ldh_iyd_, iy, {ld.s h, (ix)}

	; FD 67 ld iyh, a
	z80_exaf2	ldiyha, {ld (ix+z80_reg_iy+1), a}

	; FD 68 ld iyl, b
	z80_niy	ldiylb

	; FD 69 ld iyl, c
	z80_niy	ldiylc

	; FD 6A ld iyl, d
	z80_niy	ldiyld

	; FD 6B ld iyl, e
	z80_niy	ldiyle

	; FD 6C ld iyl, iyh
	z80_niy	ldiyliyh

	; FD 6D ld iyl, iyl
	z80_niy	ldiyliyl

	; FD 6E ld l,(iy+d)
	z80_irtoix	ldl_iyd_, iy, {ld.s l, (ix)}

	; FD 6F ld iyl, a
	z80_niy	ldiyla

	; FD 70 ld (iy+d),b
z80_ld_iyd_b:
	ld.s	a, (iy)
	inc	iy

	; Sign-extend 8-bit A into 16-bit HL
	ld	l, a  ; Store low byte
	add	a, a  ; Push sign into carry
	sbc	a     ; Turn it into 0 or -1
	ld	h, a  ; Store high byte

	ld	bc, (ix+z80_reg_iy)
	add.s	hl, bc

	exx
	push	bc
	exx
	pop	bc
	ld.s	(hl), b
	z80loop

	; FD 71 ld (iy+d),c
	z80_irtoix	ld_iyd_c, iy, {ld.s (ix), c}

	; DD 72 ld (ix+d),d
	z80_irtoix	ld_iyd_d, iy, {ld.s (ix), d}

	; DD 73 ld (ix+d),e
	z80_irtoix	ld_iyd_e, iy, {ld.s (ix), e}

	; DD 74 ld (ix+d),h
	z80_irtoix	ld_iyd_h, iy, {ld.s (ix), h}

	; DD 75 ld (ix+d),l
	z80_irtoix	ld_iyd_l, iy, {ld.s (ix), l}

	; FD 77 ld (iy+d),a
	z80_afir	ld_iyd_a, iy, {ld.s (hl), a}

	; FD 7C ld a, iyh
	z80_niy	ldaiyh

	; FD 7D ld a, iyl
	z80_niy	ldaiyl

	; FD 7E ld a,(iy+d)
	z80_afir	lda_iyd_, iy, {ld.s a, (hl)}

	; FD 84 add a, iyh
	z80_exaf2	addaiyh, {add a, (ix+z80_reg_iy+1)}

	; FD 85 add a, iyl
	z80_exaf2	addaiyl, {add a, (ix+z80_reg_iy+0)}

	; FD 86 add a,(iy+d)
	z80_afir	adda_iyd_, iy, {add.s a, (hl)}

	; FD 8C adc a, iyh
	z80_exaf2	adcaiyh, {adc a, (ix+z80_reg_iy+1)}

	; FD 8D adc a, iyl
	z80_exaf2	adcaiyl, {adc a, (ix+z80_reg_iy+0)}

	; FD 8E adc a,(iy+d)
	z80_afir	adca_iyd_, iy, {adc.s a, (hl)}

	; FD 94 sub a, iyh
	z80_exaf2	subaiyh, {sub a, (ix+z80_reg_iy+1)}

	; FD 95 sub a, iyl
	z80_exaf2	subaiyl, {sub a, (ix+z80_reg_iy+0)}

	; FD 96 sub a, (iy+d)
	z80_afir	suba_iyd_, iy, {db %52}, {sub a, (hl)} 	; bug in assembler sub.s a, (hl)

	; FD 9C sbc a, iyh
	z80_exaf2	sbcaiyh, {sbc a, (ix+z80_reg_iy+1)}

	; FD 9D abc a, iyl
	z80_exaf2	sbcaiyl, {sbc a, (ix+z80_reg_iy+0)}

	; FD 9E sbc a, (iy+d)
	z80_afir	sbca_iyd_, iy, {sbc.s a, (hl)}

	; FD A4 and a, iyh
	z80_exaf2	andaiyh, {and a, (ix+z80_reg_iy+1)}

	; FD A5 and a, iyl
	z80_exaf2	andaiyl, {and a, (ix+z80_reg_iy+0)}

	; FD A6 and a, (iy+d)
	z80_afir	anda_iyd_, iy, {and.s a, (hl)}

	; FD AC xor a, iyh
	z80_exaf2	xoraiyh, {xor a, (ix+z80_reg_iy+1)}

	; FD AD xor a, iyl
	z80_exaf2	xoraiyl, {xor a, (ix+z80_reg_iy+0)}

	; FD AE xor a, (iy+d)
	z80_afir	xora_iyd_, iy, {xor.s a, (hl)}

	; FD B4 or a, iyh
	z80_exaf2	oraiyh, {or a, (ix+z80_reg_iy+1)}

	; FD B5 or a, iyl
	z80_exaf2	oraiyl, {or a, (ix+z80_reg_iy+0)}

	; FD B6 or a, (iy+d)
	z80_afir	ora_iyd_, iy, {or.s a, (hl)}

	; FD BC cp a, iyh
	z80_exaf2	cpaiyh, {cp a, (ix+z80_reg_iy+1)}

	; FD BD cp a, iyl
	z80_exaf2	cpaiyl, {cp a, (ix+z80_reg_iy+0)}

	; FD BE cp a, (iy+d)
	z80_afir	cpa_iyd_, iy, {cp.s a, (hl)}

	; FD CB iy bit operators
	; FD, CB, dd, kk -> 52, FD, CB, dd, kk
z80_iybits:
	ld.s	bc, (iy)
	inc	iy
	inc	iy
	ld	hl, fd_bit_instr
	ld	(hl), c
	inc	hl
	ld	(hl), b
	ld	ix, (ix+z80_reg_iy)
	exx
	ex	af, af'
	jp	z80_iybits2

	; $FD E1 pop iy
	z80_popir	iy

	; FD E3 ex (sp), iy
z80_ex_sp_iy:
	ld	hl, (ix+z80_reg_iy)
	ex.s	(sp), hl
	ld	(ix+z80_reg_iy), hl
	z80loop

	; $FD $E5 push iy
	z80_pushir iy

	; $FD $E9 jp (iy)
	z80_jp_ir_ iy

	; FD F9 ld sp, iy
z80_spiy:
	ld	hl, (ix+z80_reg_iy)
	ld.s	sp, hl
	z80loop


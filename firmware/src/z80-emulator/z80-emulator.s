	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1
	section	CODE

	xref	_reg_ix
	xref	_z80_flags
	xref	z80_flags
	xref	z80_reg_aaf
	xref	z80_reg_abc
	xref	z80_reg_ade
	xref	z80_reg_ahl
	xref	z80_reg_ix
	xref	z80_reg_iy
	xref	z80_regs

	xref	_calculate_emulated_io_clock_rate
	xref	_marshall_isr_hook
	xref	cb_bit_instr
	xref	switch_addr
	xref	z80_bit2
	xref	z80_ldcc
	xref	z80_lddd
	xref	z80_ldee
	xref	z80_loop2
	xref	z80_emulator_isr
	xref	z80_misc
	xref	z80_switch_to_native2

	global	z80_ldbb
	global	z80_ldbc
	global	z80_ldbd
	global	z80_ldbe
	global	z80_ldba
	global	z80_ldcb
	global	z80_ldcd
	global	z80_ldce
	global	z80_ldca
	global	z80_lddb
	global	z80_lddc
	global	z80_ldde
	global	z80_ldda
	global	z80_ldeb
	global	z80_ldec
	global	z80_lded
	global	z80_ldea
	global	z80_ldab
	global	z80_ldac
	global	z80_ldad
	global	z80_ldae
	global	z80_ldaa
	global	z80_addab
	global	z80_addac
	global	z80_addad
	global	z80_addac
	global	z80_addaa
	global	z80_adcab
	global	z80_adcac
	global	z80_adcad
	global	z80_adcae
	global	z80_adcaa
	global	z80_subab
	global	z80_subac
	global	z80_subad
	global	z80_subae
	global	z80_subaa
	global	z80_sbcaa
	global	z80_andab
	global	z80_andac
	global	z80_andad
	global	z80_andae

	global	z80_invoke
	global	z80_invoke_iy
	global	z80_load_all_registers
	global	z80_save_all_registers
	global	z80_set_int_state
	global	z80_switch_to_native

; start executing z80 code at MBASE:0000
z80_invoke:
	ld	ix, z80_regs
	ld	(ix+z80_flags), 0
	ld	iy, 0

z80_invoke_iy:
	; fall through to z80_nop

	global	z80_instr_table
z80_instr_table:
	jp	z80_nop			; 00
	jp	z80_ldbcnn		; 01
	jp	z80_ld_bc_a		; 02
	jp	z80_incbc		; 03
	jp	z80_incb		; 04
	jp	z80_decb		; 05
	jp	z80_ldbn		; 06
	jp	z80_rlca		; 07
	jp	z80_exafaaf		; 08
	jp	z80_addhlbc		; 09
	jp	z80_lda_bc_		; 0A
	jp	z80_decbc		; 0B
	jp	z80_incc		; 0C
	jp	z80_decc		; 0D
	jp	z80_ldcn		; 0E
	jp	z80_rrca		; 0F
	jp	z80_djnzd		; 10
	jp	z80_lddenn		; 11
	jp	z80_ld_de_a		; 12
	jp	z80_incde		; 13
	jp	z80_incd		; 14
	jp	z80_decd		; 15
	jp	z80_lddn		; 16
	jp	z80_rla			; 17
	jp	z80_jrd			; 18
	jp	z80_addhlde		; 19
	jp	z80_lda_de_		; 1A
	jp	z80_decde		; 1B
	jp	z80_ince		; 1C
	jp	z80_dece		; 1D
	jp	z80_lden		; 1E
	jp	z80_rra			; 1F
	jp	z80_jrnzd		; 20
	jp	z80_ldhlnn		; 21
	jp	z80_ld_nn_hl		; 22
	jp	z80_inchl		; 23
	jp	z80_inch		; 24
	jp	z80_dech		; 25
	jp	z80_ldhn		; 26
	jp	z80_daa			; 27
	jp	z80_jrzd		; 28
	jp	z80_addhlhl		; 29
	jp	z80_ldhl_nn_		; 2A
	jp	z80_dechl		; 2B
	jp	z80_incl		; 2C
	jp	z80_decl		; 2D
	jp	z80_ldln		; 2E
	jp	z80_cpl			; 2F
	jp	z80_jrncd		; 30
	jp	z80_ldspnn		; 31
	jp	z80_ld_nn_a		; 32
	jp	z80_incsp		; 33
	jp	z80_inc_hl_		; 34
	jp	z80_dec_hl_		; 35
	jp	z80_ld_hl_n		; 36
	jp	z80_scf			; 37
	jp	z80_jrcd		; 38
	jp	z80_addhlsp		; 39
	jp	z80_lda_nn_		; 3A
	jp	z80_decsp		; 3B
	jp	z80_inca		; 3C
	jp	z80_deca		; 3D
	jp	z80_ldan		; 3E
	jp	z80_ccf			; 3F
	jp	z80_ldbb		; 40
	jp	z80_ldbc		; 41
	jp	z80_ldbd		; 42
	jp	z80_ldbe		; 43
	jp	z80_ldbh		; 44
	jp	z80_ldbl		; 45
	jp	z80_ldb_hl_		; 46
	jp	z80_ldba		; 47
	jp	z80_ldcb		; 48
	jp	z80_ldcc		; 49
	jp	z80_ldcd		; 4A
	jp	z80_ldce		; 4B
	jp	z80_ldch		; 4C
	jp	z80_ldcl		; 4D
	jp	z80_ldc_hl_		; 4E
	jp	z80_ldca		; 4F
	jp	z80_lddb		; 50
	jp	z80_lddc		; 51
	jp	z80_lddd		; 52
	jp	z80_ldde		; 53
	jp	z80_lddh		; 54
	jp	z80_lddl		; 55
	jp	z80_ldd_hl_		; 56
	jp	z80_ldda		; 57
	jp	z80_ldeb		; 58
	jp	z80_ldec		; 59
	jp	z80_lded		; 5A
	jp	z80_ldee		; 5B
	jp	z80_ldeh		; 5C
	jp	z80_ldel		; 5D
	jp	z80_lde_hl_		; 5E
	jp	z80_ldea		; 5F
	jp	z80_ldhb		; 60
	jp	z80_ldhc		; 61
	jp	z80_ldhd		; 62
	jp	z80_ldhe		; 63
	jp	z80_ldhh		; 64
	jp	z80_ldhl		; 65
	jp	z80_ldh_hl_		; 66
	jp	z80_ldha		; 67
	jp	z80_ldlb		; 68
	jp	z80_ldlc		; 69
	jp	z80_ldld		; 6A
	jp	z80_ldle		; 6B
	jp	z80_ldlh		; 6C
	jp	z80_ldll		; 6D
	jp	z80_ldl_hl_		; 6E
	jp	z80_ldla		; 6F
	jp	z80_ld_hl_b		; 70
	jp	z80_ld_hl_c		; 71
	jp	z80_ld_hl_d		; 72
	jp	z80_ld_hl_e		; 73
	jp	z80_ld_hl_h		; 74
	jp	z80_ld_hl_l		; 75
	jp	z80_halt		; 76
	jp	z80_ld_hl_a		; 77
	jp	z80_ldab		; 78
	jp	z80_ldac		; 79
	jp	z80_ldad		; 7A
	jp	z80_ldae		; 7B
	jp	z80_ldah		; 7C
	jp	z80_ldal		; 7D
	jp	z80_lda_hl_		; 7E
	jp	z80_ldaa		; 7F
	jp	z80_addab		; 80
	jp	z80_addac		; 81
	jp	z80_addad		; 82
	jp	z80_addae		; 83
	jp	z80_addah		; 84
	jp	z80_addal		; 85
	jp	z80_adda_hl_		; 86
	jp	z80_addaa		; 87
	jp	z80_adcab		; 88
	jp	z80_adcac		; 89
	jp	z80_adcad		; 8A
	jp	z80_adcae		; 8B
	jp	z80_adcah		; 8C
	jp	z80_adcal		; 8D
	jp	z80_adca_hl_		; 8E
	jp	z80_adcaa		; 8F
	jp	z80_subab		; 90
	jp	z80_subac		; 91
	jp	z80_subad		; 92
	jp	z80_subae		; 93
	jp	z80_subah		; 94
	jp	z80_subal		; 95
	jp	z80_suba_hl_		; 96
	jp	z80_subaa		; 97
	jp	z80_sbcab		; 98
	jp	z80_sbcac		; 99
	jp	z80_sbcad		; 9A
	jp	z80_sbcae		; 9B
	jp	z80_sbcah		; 9C
	jp	z80_sbcal		; 9D
	jp	z80_sbca_hl_		; 9E
	jp	z80_sbcaa		; 9F
	jp	z80_andab		; A0
	jp	z80_andac		; A1
	jp	z80_andad		; A2
	jp	z80_andae		; A3
	jp	z80_andah		; A4
	jp	z80_andal		; A5
	jp	z80_anda_hl_		; A6
	jp	z80_andaa		; A7
	jp	z80_xorab		; A8
	jp	z80_xorac		; A9
	jp	z80_xorad		; AA
	jp	z80_xorae		; AB
	jp	z80_xorah		; AC
	jp	z80_xoral		; AD
	jp	z80_xora_hl_		; AE
	jp	z80_xoraa		; AF
	jp	z80_orab		; B0
	jp	z80_orac		; B1
	jp	z80_orad		; B2
	jp	z80_orae		; B3
	jp	z80_orah		; B4
	jp	z80_oral		; B5
	jp	z80_ora_hl_		; B6
	jp	z80_oraa		; B7
	jp	z80_cpab		; B8
	jp	z80_cpac		; B9
	jp	z80_cpad		; BA
	jp	z80_cpae		; BB
	jp	z80_cpah		; BC
	jp	z80_cpal		; BD
	jp	z80_cpa_hl_		; BE
	jp	z80_cpaa		; BF
	jp	z80_retnz		; C0
	jp	z80_popbc		; C1
	jp	z80_jpnznn		; C2
	jp	z80_jpnn		; C3
	jp	z80_callnznn		; C4
	jp	z80_pushbc		; C5
	jp	z80_addan		; C6
	jp	z80_rst00		; C7
	jp	z80_retz		; C8
	jp	z80_ret			; C9
	jp	z80_jpznn		; CA
	jp	z80_bit			; CB
	jp	z80_callznn		; CC
	jp	z80_callnn		; CD
	jp	z80_adcan		; CE
	jp	z80_rst08		; CF
	jp	z80_retnc		; D0
	jp	z80_popde		; D1
	jp	z80_jpncnn		; D2
	jp	z80_out_n_a		; D3
	jp	z80_callncnn		; D4
	jp	z80_pushde		; D5
	jp	z80_subn		; D6
	jp	z80_rst10		; D7
	jp	z80_retc		; D8
	jp	z80_exx			; D9
	jp	z80_jpcnn		; DA
	jp	z80_ina_n_		; DB
	jp	z80_callcnn		; DC
	jp	z80_ix			; DD
	jp	z80_sbcan		; DE
	jp	z80_rst18		; DF
	jp	z80_retpo		; E0
	jp	z80_pophl		; E1
	jp	z80_jpponn		; E2
	jp	z80_ex_sp_hl		; E3
	jp	z80_callponn		; E4
	jp	z80_pushhl		; E5
	jp	z80_andn		; E6
	jp	z80_rst20		; E7
	jp	z80_retpe		; E8
	jp	z80_jp_hl_		; E9
	jp	z80_jppenn		; EA
	jp	z80_exdehl		; EB
	jp	z80_callpenn		; EC
	jp	z80_misc		; ED
	jp	z80_xoran		; EE
	jp	z80_rst28		; EF
	jp	z80_retp		; F0
	jp	z80_popaf		; F1
	jp	z80_jppnn		; F2
	jp	z80_di			; F3
	jp	z80_callpnn		; F4
	jp	z80_pushaf		; F5
	jp	z80_oran		; F6
	jp	z80_rst30		; F7
	jp	z80_retm		; F8
	jp	z80_ldsphl		; F9
	jp	z80_jpmnn		; FA
	jp	z80_ei			; FB
	jp	z80_callmnn		; FC
	jp	z80_iy			; FD
	jp	z80_cpn			; FE
	jp	z80_rst38		; FF

	global	not_implemented
not_implemented:
	di
	nop
	nop
	halt

	; $00 nop
	global	z80_nop
z80_nop:
	z80loop

	; $01 ld bc, nn
	z80_exmain2	ldbcnn, {ld.s bc, (iy)}, {lea iy, iy+2}

	; $02 ld (bc), a
	z80_exall2	ld_bc_a, {ld.s (bc), a}

	; $03 inc bc
	z80_exmain2	incbc, {inc bc}

	; $04 inc b
	z80_exall2	incb, {inc b}

	; $05 dec b
	z80_exall2	decb, {dec b}

	; $06 ld b, n
	z80_exmain2	ldbn, {ld.s b, (iy)}, {inc iy}

	; $07 rlca
	z80_exaf2	rlca, {rlca}

	; $08 ex af, af'
; z80 emulated state: A =>1, A'=>2
; ez80 state        : ()=>2, A'=>1
z80_exafaaf:
					; a'=1, ()=>2
	ld	bc, (ix+z80_reg_aaf)	; b=2, ()=>2
	push	bc
	pop	af			; a=2, a'=1
	ex	af, af'			; a=1, a'=2
	push	af
	pop	bc
	ld	(ix+z80_reg_aaf), bc	; ()=>1
	z80loop

	; $09 add hl, bc
	z80_exall2	addhlbc, {add.s hl, bc}

	; $0A ld a, (bc)
	z80_exall2	lda_bc_, {ld.s a, (bc)}

	; $0B dec bc
	z80_exmain2	decbc, {dec bc}

	; $0C inc c
	z80_exall2	incc, {inc c}

	; $0d dec c
	z80_exall2	decc, {dec c}

	; $0E ld c, n
	z80_exmain2	ldcn, {ld.s c, (iy)}, {inc iy}

	; $0F rrca
	z80_exaf2	rrca, {rrca}

	section	INTERNAL_RAM_ROM
	; $10 djnz d
z80_djnzd:
	inc	iy
	exx
	ex	af, af'
	djnz	z80_jrccd1
	ex	af, af'
	exx
	z80loop
	section	CODE

z80_jrccd1:
	ex	af, af'
	exx
	ld.s	a, (iy-1)
	jp	_z80_add_a_to_pc

	; $11 ld de, nn
	z80_exmain2	lddenn, {ld.s de, (iy)}, {lea iy, iy+2}

	; $12 ld (de), a
	z80_exall2	ld_de_a, {ld.s (de), a}

	; $13 inc de
	z80_exmain2	incde, {inc de}

	; $14 inc d
	z80_exall2	incd, {inc d}

	; $15 dec d
	z80_exall2	decd, {dec d}

	; $16 ld d, n
	z80_exmain2	lddn, {ld.s d, (iy)}, {inc iy}

	; $17 rla
	z80_exaf2	rla, {rla}

	; $18 JR d
	section	INTERNAL_RAM_ROM
z80_jrd:
	ld.s	a, (iy)
	inc	iy
_z80_add_a_to_pc:
	ld	e, a
	rlca			; Shift left to put sign in carry
	sbc	a, a		; A = 0x00 if positive, 0xFF if negative
	ld	d, a		; D = sign extension
	add.s	iy, de
	z80loop

	section	CODE

	; $19 add hl, de
	z80_exall2	addhlde, {add.s hl, de}

	; $1A ld a, (de)
	z80_exall2	lda_de_, {ld.s a, (de)}

	; $1B dec de
	z80_exmain2	decde, {dec de}

	; $1D inc e
	z80_exall2	ince, {inc e}

	; $1D dec e
	z80_exall2	dece, {dec e}

	; $1E ld e, n
	z80_exmain2  	lden, {ld.s e, (iy)}, {inc iy}

	; $1F rra
	z80_exaf2	rra, {rra}

	; $20 jr nz, d
	section	INTERNAL_RAM_ROM
z80_jrnzd:
	z80_jrccd	{jr nz,}
	section	CODE

	; $21 ld hl, nn
	z80_exmain2	ldhlnn, {ld.s hl, (iy)}, {lea iy, iy+2}

	; $22 ld (nn), hl
z80_ld_nn_hl:
	ld.s	hl, (iy)
	lea	iy, iy+2
	exx
	push	hl
	exx
	pop	bc
	ld.s	(hl), bc
	z80loop

	; $23 inc hl
	z80_exmain2	inchl, {inc hl}

	; $24 inc h
	z80_exall2	inch, {inc h}

	; $25 dec h
	z80_exall2	dech, {dec h}

	; $26 ld h, n
	z80_exmain2	ldhn, {ld.s h, (iy)}, {inc iy}

	; $27 daa
	z80_exaf2	daa, {daa}

	; $28 jr z, d
	section	INTERNAL_RAM_ROM
z80_jrzd:
	z80_jrccd	{jr z,}
	section	CODE

	; $29 add hl, hl
	z80_exall2	addhlhl, {add.s	hl, hl}

	; $2A ld hl, (nn)
z80_ldhl_nn_:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ld.s	a, (hl)
	ld	c, a
	inc	hl
	ld.s	a, (hl)
	ld	b, a
	push	bc
	exx
	pop	hl
	exx
	z80loop

	; $2b dec hl
	z80_exmain2	dechl, {dec hl}

	; $2C inc l
	z80_exall2	incl, {inc l}

	; $2D dec l
	z80_exall2	decl, {dec l}

	; $2e ld l, n
	z80_exmain2	ldln, {ld.s l, (iy)}, {inc iy}
	; $2F cpl
	z80_exaf2	cpl, {cpl}

	; $30 jr nc, d
	section	INTERNAL_RAM_ROM
z80_jrncd:
	z80_jrccd	{jr nc,}
	section	CODE

	; $31 ld sp, nn
z80_ldspnn:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ld.s	sp, hl
	z80loop

	; $32 ld (nn), a
z80_ld_nn_a:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ex	af, af'
	ld.s	(hl), a
	ex	af, af'
	z80loop

	; $33 inc sp
z80_incsp:
	inc.s	sp
	z80loop

	; $34 inc (hl)
	z80_exall2	inc_hl_, {inc.s (hl)}

	; $35 dec (hl)
	z80_exall2	dec_hl_, {dec.s (hl)}

	; $36 ld (hl), n
z80_ld_hl_n:
	ld.s	a, (iy)
	inc	iy
	exx
	ld.s	(hl), a
	exx
	z80loop

	; $37 scf
	z80_exaf2	scf, {scf}

	; $38 jr c,d
	section	INTERNAL_RAM_ROM
z80_jrcd:
	z80_jrccd	{jr c,}
	section	CODE

	; $39 add hl, sp
	z80_exall2	addhlsp, {add.s hl, sp}

	; $3A ld a, (nn)
z80_lda_nn_:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ex	af, af'
	ld.s	a, (hl)
	ex	af, af'
	z80loop

	; $3B dec sp
z80_decsp:
	dec.s	sp
	z80loop

	; $3C inc a
	z80_exaf2	inca, {inc a}

	; $3D dec a
	z80_exaf2	deca, {dec a}

	; $3E ld a, n
z80_ldan:
	ex	af, af'
	ld.s	a, (iy)
	inc	iy
	ex	af, af'
	z80loop

	; $3F ccf
	z80_exaf2	ccf, {ccf}

	; $40 ld b,b aka .SIS suffix
z80_ldbb:
.if	EN_EZ80_INSTR
	jp	not_implemented
.else
	z80loop
.endif

	; $41 ld b, c
	z80_exmain2	ldbc, {ld b, c}

	; $42 ld b, d
	z80_exmain2	ldbd, {ld b, d}

	; $43 ld b, e
	z80_exmain2	ldbe, {ld b, e}

	; %44 ld b, h
	z80_exmain2	ldbh, {ld b, h}

	; $45 ld b, l
	z80_exmain2	ldbl, {ld b, l}

	; $46 ld b, (hl)
	z80_exmain2	ldb_hl_, {db %52}, {ld b, (hl)}	; bug in assembler ld.s b, (hl) not supported??

	; $47 ld b, a
	z80_exall2	ldba, {ld b, a}

	; $48 ld c, b
	z80_exmain2	ldcb, {ld c, b}

	; 49 ld c, c aka lil suffix
	; implemented in z80-emulator-lis.s

	; $4A ld c, d
	z80_exmain2     ldcd, {ld c, d}

	; $4B ld c, e
	z80_exmain2     ldce, {ld c, e}

	; $4C ld c, h
	z80_exmain2     ldch, {ld c, h}

	; $4D ld c, l
	z80_exmain2     ldcl, {ld c, l}

	; $4E ld c, (hl)
	z80_exmain2	ldc_hl_, {db %52}, {ld c, (hl)}	; bug in assembler ld.s c, (hl) not supported??

	; $4F ld c, a
	z80_exall2      ldca, {ld c, a}

	; $50 ld d, b
	z80_exmain2	lddb, {ld d, b}

	; $51 ld d, c
	z80_exmain2	lddc, {ld d, c}

	; $52 ld d, d aka .sil/il suffix
	; implemented in z80-emulator-sil.s

	; $53 ld d, e
	z80_exmain2	ldde, {ld d, e}

	; $54 ld d, h
	z80_exmain2	lddh, {ld d, h}

	; $55 ld d, l
	z80_exmain2	lddl, {ld d, l}

	; $56 ld d, (hl)
	z80_exmain2	ldd_hl_, {db %52}, {ld d, (hl)}	; bug in assembler ld.s d, (hl) not supported??

	; $57 ld d, a
	z80_exall2	ldda, {ld d, a}

	; $58 ld e, b
	z80_exmain2	ldeb, {ld e, b}

	; $59 ld e, c
	z80_exmain2	ldec, {ld e, c}

	; $5A ld e, d
	z80_exmain2	lded, {ld e, d}

	; $5B ld e, e aka lil suffix
	; implemented in z80-emulator-lil.s

	; $5C ld e, h
	z80_exmain2	ldeh, {ld e, h}

	; $5D ld e, l
	z80_exmain2	ldel, {ld e, l}

	; $5E ld e, (hl)
	z80_exmain2	lde_hl_, {db %52}, {ld e, (hl)}	; bug in assembler ld.s e, (hl) not supported??

	; $5f	ld e, a
	z80_exall2	ldea, {ld e, a}

	; $60 ld h, b
	z80_exmain2	ldhb, {ld h, b}

	; $61 ld h, c
	z80_exmain2	ldhc, {ld h, c}

	; $62 ld h, d
	z80_exmain2     ldhd, {ld h, d}

	; $63 ld h, e
	z80_exmain2     ldhe, {ld h, e}

	; $64 ld h, h
z80_ldhh:
	z80loop

	; $ 65 ld h, l
	z80_exmain2	ldhl, {ld h, l}

	; $66 ld h, (hl)
	z80_exall2	ldh_hl_, {db %52}, {ld h, (hl)}		; bug in assembler does not support ld.s h, (hl)

	; $67 ld h, a
	z80_exall2	ldha, {ld h, a}

	; $67 ld l, b
	z80_exmain2	ldlb, {ld l, b}

	; $69 ld l, c
	z80_exmain2	ldlc, {ld l, c}

	; $6A ld l, d
	z80_exmain2	ldld, {ld l, d}

	; $6B ld l, e
	z80_exmain2	ldle, {ld l, e}

	; $6C ld l, h
	z80_exmain2	ldlh, {ld l, h}

	; $6D ld l, l
z80_ldll:
	z80loop

	; $6E ld l, (hl)
	z80_exall2	ldl_hl_, {db %52}, {ld l, (hl)}		; bug in assembler does not support ld.s l, (hl)

	; $6F ld l, a
	z80_exall2	ldla, {ld l, a}

	; $70 ld (hl), b
	z80_exmain2	ld_hl_b, {ld.s (hl), b}

	; $72 ld (hl), c
	z80_exmain2	ld_hl_c, {ld.s (hl), c}

	; $72 ld (hl), d
	z80_exmain2	ld_hl_d, {ld.s (hl), d}

	; $73 ld (hl), e
	z80_exmain2	ld_hl_e, {ld.s (hl), e}

	; $74 ld (hl), h
	z80_exmain2	ld_hl_h, {ld.s (hl), h}

	; $75 ld (hl), e
	z80_exmain2	ld_hl_l, {ld.s (hl), l}

	; $76 halt
z80_halt:
	bit	0, (ix+z80_flags)		; int pending?
	jr	z, z80_halt
	z80loop

	; $77 ld (hl), a
	z80_exall2	ld_hl_a, {ld.s (hl), a}

	; $78 ld a, b
	z80_exall2	ldab, {ld a, b}

	; $79 ld a, c
	z80_exall2	ldac, {ld a, c}

	; $7A ld a, d
	z80_exall2	ldad, {ld a, d}

	; $7B ld a, e
	z80_exall2	ldae, {ld a, e}

	; $7C ld a, h
	z80_exall2	ldah, {ld a, h}

	; $7D ld a, l
	z80_exall2	ldal, {ld a, l}

	; $7E ld a, (hl)
	z80_exall2	lda_hl_, {ld.s a, (hl)}

	; $7F ld a, a
z80_ldaa:
	z80loop

	; $80 add a, b
	z80_exall2	addab, {add a, b}

	; $81 add a, c
	z80_exall2	addac, {add a, c}

	; $82 add a, d
	z80_exall2	addad, {add a, d}

	; $83 add a, e
	z80_exall2	addae, {add a, e}

	; $84 add a, h
	z80_exall2	addah, {add a, h}

	; $85 add a, l
	z80_exall2	addal, {add a, l}

	; $86 add a, (hl)
	z80_exall2	adda_hl_, {add.s a, (hl)}

	; $87 add a, a
	z80_exaf2	addaa, {add a, a}

	; $88 adc a, b
	z80_exall2	adcab, {adc a, b}

	; $89 adc a, c
	z80_exall2	adcac, {adc a, c}

	; $8A adc a, d
	z80_exall2	adcad, {adc a, d}

	; $8B adc a, e
	z80_exall2	adcae, {adc a, e}

	; $8C adc a, h
	z80_exall2	adcah, {adc a, h}

	; $8D adc a, l
	z80_exall2	adcal, {adc a, l}

	; $8E adc a, (hl)
	z80_exall2	adca_hl_, {adc.s a, (hl)}

	; $8F adc a, a
	z80_exaf2	adcaa, {adc a, a}

	; $90 sub a, b
	z80_exall2	subab, {sub a, b}

	; $91 sub a, c
	z80_exall2	subac, {sub a, c}

	; $92 sub a, d
	z80_exall2	subad, {sub a, d}

	; $93 sub a, e
	z80_exall2	subae, {sub a, e}

	; $94 sub a, h
	z80_exall2	subah, {sub a, h}

	; $95 sub a, l
	z80_exall2	subal, {sub a, l}

	; $96 sub a, (hl)
	z80_exall2	suba_hl_, {db %52}, {sub a, (hl)} 	; bug in assembler sub.s a, (hl) not supported??

	; $97 sub a, a
	z80_exaf2	subaa, {sub a, a}

	; $98 sbc a, b
	z80_exall2	sbcab, {sbc a, b}

	; $99 sbc a, c
	z80_exall2	sbcac, {sbc a, c}

	; $9A sbc a, d
	z80_exall2	sbcad, {sbc a, d}

	; $9B sbc a, e
	z80_exall2	sbcae, {sbc a, e}

	; $9C sbc a, h
	z80_exall2	sbcah, {sbc a, h}

	; $9D sbc a, l
	z80_exall2	sbcal, {sbc a, l}

	; $9E sbc a, (hl)
	z80_exall2	sbca_hl_, {sbc.s a, (hl)}

	; $9F sbc a, a
	z80_exaf2	sbcaa, {sbc a, a}

	; $A0 and a, b
	z80_exall2	andab, {and a, b}

	; $A1 and a,c
	z80_exall2	andac, {and a, c}

	; $A2 and a,d
	z80_exall2	andad, {and a, d}

	; $A3 and a,e
	z80_exall2	andae, {and a, e}

	; $A4 and a,h
	z80_exall2	andah, {and a, h}

	; $A5 and a,h
	z80_exall2	andal, {and a, l}

	; $A6 and a, (hl)
	z80_exall2	anda_hl_, {and.s a, (hl)}

	; $A7 and a, a
	z80_exaf2	andaa, {and a, a}

	; $A8 xor a, b
	z80_exall2	xorab, {xor a, b}

	; $A9 xor a, c
	z80_exall2	xorac, {xor a, c}

	; $AA xor a, d
	z80_exall2	xorad, {xor a, d}

	; $AB xor a, e
	z80_exall2	xorae, {xor a, e}

	; $AC xor a, h
	z80_exall2	xorah, {xor a, h}

	; $AD xor a, l
	z80_exall2	xoral, {xor a, l}

	; $AE xor (hl)
	z80_exall2	xora_hl_, {xor.s a, (hl)}

	; $AF xor a
	z80_exaf2 	xoraa, {xor a, a}

	; $B0 or a, b
	z80_exall2	orab, {or a, b}

	; $B1 or a, c
	z80_exall2	orac, {or a, c}

	; $B2 or a, d
	z80_exall2	orad, {or a, d}

	; $B3 or a, e
	z80_exall2	orae, {or a, e}

	; $B4 or a, h
	z80_exall2	orah, {or a, h}

	; $B5 or a, l
	z80_exall2	oral, {or a, l}

	; $B6 or a, (hl)
	z80_exall2	ora_hl_, {or.s a, (hl)}

	; $B7 or a, a
	z80_exaf2	oraa, {or a, a}

	; $B8 cp a, b
	z80_exall2	cpab, {cp a, b}

	; $B9 cp a, c
	z80_exall2	cpac, {cp a, c}

	; $BA cp a, d
	z80_exall2	cpad, {cp a, d}

	; $BB cp a, e
	z80_exall2	cpae, {cp a, e}

	; $BC cp a, h
	z80_exall2	cpah, {cp a, h}

	; $BD cp a, l
	z80_exall2	cpal, {cp a, l}

	; $BE cp (hl)
	z80_exall2	cpa_hl_, {cp.s a, (hl)}

	; $BF cp a, a
	z80_exaf2	cpaa, {cp a, a}

	; $C0 ret nz
	z80_retcc	jr, nz

	; $C1 pop bc
	z80_exmain2	popbc, {pop.s bc}

	; $C2 jp nz, nn
	z80_jpccnn	jr, nz

	; $C3 JP nn
z80_jpnn:
	ld.s	iy, (iy)
	z80loop

	; $ C4 call nz, nn
	z80_callccnn	nz

	; $c5 push bc
	z80_exmain2	pushbc, {push.s bc}

	; $C6 add a, n
z80_addan:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	add	a, b
	ex	af, af'
	z80loop

	; $C7 rst 0
z80_rst00:
	push.s	iy
	ld	iy, %00
	z80loop

	; $C8 ret z
	z80_retcc	jr, z

	; $C9 ret
z80_ret:
	pop.s	iy
	z80loop

	; $CA jp z, nn
	z80_jpccnn	jr, z

	; $CB ....
	xref	z80_bit

	global	z80_switch_to_native
z80_switch_to_native:
	di_and_save_s
	ld	a, iyl
	ld	(switch_addr+0), a
	ld	a, iyh
	ld	(switch_addr+1), a
	call	z80_load_all_registers
	jp	z80_switch_to_native2

	; $CC call z, nn
	z80_callccnn	z

	; $CD nn
z80_callnn:
	pea.s	iy+2
	ld.s	iy, (iy)
	z80loop

	; $CE adc a, n
	z80_exaf2	adcan, {adc.s a, (iy)}, {inc iy}

	; $CF rst $08
z80_rst08:
	push.s	iy
	ld	iy, %08
	z80loop

	; $D0 ret nc
	z80_retcc	jr, nc

	; $d1 pop de
	z80_exmain2	popde, {pop.s de}

	; $D2 jp nc, nn
	z80_jpccnn	jr, nc

	; $D3 out (n), a
z80_out_n_a:
	ld.s	c, (iy)
	inc	iy
	ld	b, IO_SEGMENT
	ex	af, af'
	out	(bc), a
	ex	af, af'
	z80loop

	; $D4 call nc, nn
	z80_callccnn	nc

	; $D5 push de
	z80_exmain2	pushde, {push.s de}

	; $D6 sub a, n
z80_subn:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	sub	a, b
	ex	af, af'
	z80loop

	; $D7 rst $10
z80_rst10:
	push.s	iy
	ld	iy, %10
	z80loop

	; $D8 ret c
	z80_retcc	jr, c

	; $D9 exx
z80_exx:
	ld	bc, (ix+z80_reg_abc)
	ld	de, (ix+z80_reg_ade)
	ld	hl, (ix+z80_reg_ahl)
	exx
	ld	(ix+z80_reg_abc), bc
	ld	(ix+z80_reg_ade), de
	ld	(ix+z80_reg_ahl), hl
	z80loop

	; $DA jp c, nn
	z80_jpccnn	jr, c

	; $DB in a, (n)
	global	z80_ina_n_
z80_ina_n_:
	ld.s	c, (iy)
	inc	iy
	ld	b, IO_SEGMENT
	ex	af, af'
	jr	unknown_issue1
	align	2		; this does not fill with zeros - but %FF
	global	unknown_issue1	; this address must *not* align with address ...XXXXXX01
unknown_issue1:			; Unknown glitch 1 - see notes
	in	a, (bc)
	ex	af, af'
	z80loop

	; $DC call c, nn
	z80_callccnn	c

	; $DD .....
	XREF	z80_ix

	; $DE sbc a, n
	z80_exaf2	sbcan, {sbc.s a, (iy)}, {inc iy}

	; $DF rst $18
z80_rst18:
	push.s	iy
	ld	iy, %18
	z80loop

	; $E0 ret po
	z80_retcc	jp, po

	; $E1 pop hl
	z80_exmain2	pophl, {pop.s hl}

	; $E2 jp po, nn
	z80_jpccnn	jp, po

	; $E3 ex (sp), hl
z80_ex_sp_hl:
	exx
	ex.s	(sp), hl
	exx
	z80loop

	; $E4 call po, nn
	z80_callccnn	po

	; $E5 push hl
	z80_exmain2	pushhl, {push.s hl}

	; $E6 and a, n
z80_andn:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	and	a, b
	ex	af, af'
	z80loop

	; $E7 rst %20
z80_rst20:
	push.s	iy
	ld	iy, %20
	z80loop

	; $E8 ret pe
	z80_retcc	jp, pe

	; $E9 jp (hl)
z80_jp_hl_:
	exx
	push	hl
	exx
	pop	iy
	z80loop

	; $EA jp pe, nn
	z80_jpccnn	jp, pe

	; $EB ex de, hl
	z80_exmain2	exdehl, {ex de, hl}

	; $EC call pe, nn
	z80_callccnn	pe

	; $EE xor a, n
	z80_exaf2	xoran, {xor.s a, (iy)}, {inc iy}

	; $EF rst %28
z80_rst28:
	push.s	iy
	ld	iy, %28
	z80loop

	; $F0 ret p
	z80_retcc	jp, p

	; $F1 pop af
	z80_exaf2	popaf, {pop.s af}

	; $F2 jp p, nn
	z80_jpccnn	jp, p

	; $F3 DI
z80_di:
	set	1, (ix+z80_flags)	; IEF2 unset
	di
	jp	z80_loop2

	; $F4 call p, nn
	z80_callccnn	p

	; $F5 push af
	z80_exaf2	pushaf, {push.s af}

	; $F6 or a, n
z80_oran:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	or	a, b
	ex	af, af'
	z80loop

	; $F7 rst 30
z80_rst30:
	push.s	iy
	ld	iy, %30
	z80loop

	; $f8 ret m
	z80_retcc	jp, m

	; $F9 ld sp, hl
	z80_exmain2	ldsphl, {ld.s sp, hl}

	; $FA jp m, nn
	z80_jpccnn	jp, m

	; $FB ei
z80_ei:
	res	1, (ix+z80_flags)	; IEF2 set
	ei
	z80loop

	; $FC call m, nn
	z80_callccnn	m

	; $FD .....
	XREF	z80_iy

	; $FE cp a, n
z80_cpn:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	cp	b
	ex	af, af'
	z80loop

	; $FF rst %38
z80_rst38:
	push.s	iy
	ld	iy, %38
	z80loop

; MISC ED Instructions
	global	z80_instr_misc_table
z80_instr_misc_table:
	jp	z80_in0b_n_	; ED 00 in0 b,(n)
	jp	z80_out0_n_b	; ED 01 out0 (n), b
	jp	z80_leabcixd	; ED 02 lea bc, ix+d
	jp	z80_leabciyd	; ED 03 lea bc, iy+d
	jp	z80_tstab	; ED 04 tst a, b
	jp	z80_nop		; ED 05
	jp	z80_nop		; ED 06
	jp	z80_ldbc_hl_	; ED 07 ld bc, (hl)
	jp	z80_in0c_n_	; ED 08 in0 c, (n)
	jp	z80_out0_n_c	; ED 09 out0 (n),c
	jp	z80_nop		; ED 0A
	jp	z80_nop		; ED 0B
	jp	z80_tstac	; ED 0C tst a, c
	jp	z80_nop		; ED 0D
	jp	z80_nop		; ED 0E
	jp	z80_ld_hl_bc	; ED 0F ld (hl), bc
	jp	z80_in0d_n_	; ED 10 in0 d, (n)
	jp	z80_out0_n_d	; ED 11 out0 (n), d
	jp	z80_leadeixd	; ED 12 lea de, ix+d
	jp	z80_leadeiyd	; ED 13 lea de, iy+d
	jp	z80_tstad	; ED 14 tst a, d
	jp	z80_nop		; ED 15
	jp	z80_nop		; ED 16
	jp	z80_ldde_hl	; ED 17 ld de, (hl)
	jp	z80_in0e_n_	; ED 18 in0 e, (n)
	jp	z80_out0_n_e	; ED 19 out0 (n), e
	jp	z80_nop		; ED 1A
	jp	z80_nop		; ED 1B
	jp	z80_tstae	; ED 1C tst a, e
	jp	z80_nop		; ED 1D
	jp	z80_nop		; ED 1E
	jp	z80_ld_hl_de	; ED 1F ld (hl), de
	jp	z80_in0h_n_	; ED 20 in0 h, (n)
	jp	z80_out0_n_h	; ED 21 out0 (n), h
	jp	z80_leahlixd	; ED 22 lea hl, ix+d
	jp	z80_leahliyd	; ED 23 lea hl, iy+d
	jp	z80_tstah	; ED 24 tst a, h
	jp	z80_nop		; ED 25
	jp	z80_nop		; ED 26
	jp	z80_ldhl_hl_	; ED 27 ld hl, (hl)
	jp	z80_in0l_n_	; ED 28 in0 l, (n)
	jp	z80_out0_n_l	; ED 29 out0 (n), l
	jp	z80_nop		; ED 2A
	jp	z80_nop		; ED 2B
	jp	z80_tstal	; ED 2C tst a, l
	jp	z80_nop		; ED 2D
	jp	z80_nop		; ED 2E
	jp	z80_ld_hl_hl	; ED 2F ld (hl), hl
	jp	z80_nop		; ED 30
	jp	z80_ldiy_hl_	; ED 31 ld iy, (hl)
	jp	z80_leaixixd	; ED 32 lea ix, ix+d
	jp	z80_leaiyiyd	; ED 33 lea iy, iy+d
	jp	z80_tsta_hl_	; ED 34 tst a, (hl)
	jp	z80_nop		; ED 35
	jp	z80_nop		; ED 36
	jp	z80_ldix_hl_	; ED 37 ld ix, (hl)
	jp	z80_in0a_n_	; ED 38 in0 a, (n)
	jp	z80_out0_n_a	; ED 39 out0 (n), a
	jp	z80_nop		; ED 3A
	jp	z80_nop		; ED 3B
	jp	z80_tstaa	; ED 3C tst a, a
	jp	z80_nop		; ED 3D
	jp	z80_ld_hl_iy	; ED 3E ld (hl), iy
	jp	z80_ld_hl_ix	; ED 3F ld (hl), ix
	jp	z80_inb_c       ; ED 40 in b, (bc)
	jp	z80_out_c_b     ; ED 41 out (bc), b
	jp	z80_sbchlbc	; ED 42 sbc hl, bc
	jp	z80_ld_nn_bc    ; ED 43 ld (nn), bc
	jp	z80_neg		; ED 44 neg
	jp	z80_retn        ; ED 45 retn
	jp	z80_im0		; ED 46 im 0
	jp	z80_ldia	; ED 47 ld i, a
	jp	z80_inc_c_      ; ED 48 in c, (bc)
	jp	z80_out_c_c     ; ED 49 out (bc), c
	jp	z80_adchlbc	; ED 4A adc hl, bc
	jp	z80_ld_bc_nn    ; ED 4B ld bc, (nn)
	jp	z80_mltbc	; ED 4C mlt bc
	jp	z80_reti	; ED 4D reti
	jp	z80_nop		; ED 4E
	jp	z80_ldra	; ED 4F ld r,a
	jp	z80_ind_c_      ; ED 50 in d, (bc)
	jp	z80_out_c_d     ; ED 51 out (bc), d
	jp	z80_sbchlde	; ED 52 sbc hl, de
	jp	z80_ld_nn_de    ; ED 53 ld (nn), de
	jp	z80_leaixiyd	; ED 54 lea ix, iy+d
	jp	z80_leaiyixd	; ED 55 lea iy, ix+d
	jp	z80_im1		; ED 56 im 1
	jp	z80_ldai	; ED 57 ld a, i
	jp	z80_ine_c_      ; ED 58 in e, (bc)
	jp	z80_out_c_e     ; ED 59 out (bc), e
	jp	z80_adchlde	; ED 5A adc hl, de
	jp	z80_ld_de_nn    ; ED 5B ld de, (nn)
	jp	z80_mltde	; ED 5C mlt de
	jp	z80_nop		; ED 5D
	jp	z80_im2		; ED 5E im 2
	jp	z80_ldar	; ED 5F ld a, r
	jp	z80_inh_c_      ; ED 60 in h, (bc)
	jp	z80_out_c_h     ; ED 61 out (bc), h
	jp	z80_sbchlhl	; ED 62 sbc hl, hl
	jp	z80_ld_nn_hl	; ED 63 ld (nn), hl
	jp	z80_tstan	; ED 64 tst a, n
	jp	z80_peaixd	; ED 65 pea ix+d
	jp	z80_peaiyd	; ED 66 pea iy+d
	jp	z80_rrd_hl_	; ED 67 rrd (hl)
	jp	z80_inl_c_      ; ED 68 in l, (bc)
	jp	z80_out_c_l     ; ED 69 out (bc), l
	jp	z80_adchlhl	; ED 6A adc hl, hl
	jp	z80_ldhl_nn_	; ED 6B ld hl, (nn)
	jp	z80_mlthl	; ED 6C mlt hl
	jp	z80_nop		; ED 6D ld mb, a
	jp	z80_nop		; ED 6E ld a, mb
	jp	z80_rld_hl_	; ED 6F rld (hl)
	jp	z80_nop		; ED 70
	jp	z80_nop		; ED 71
	jp	z80_sbchlsp	; ED 72 sbc hl, sp
	jp	z80_ld_nn_sp    ; ED 73 ld (nn), sp
	jp	z80_tstion	; ED 74 tstio n
	jp	z80_nop		; ED 75
	jp	z80_slp		; ED 76 slp
	jp	z80_nop		; ED 77
	jp	z80_ina_c_      ; ED 78 in a, (bc)
	jp	z80_out_c_a     ; ED 79 out (bc), a
	jp	z80_adchlsp	; ED 7A adc hl, sp
	jp	z80_ldsp_nn_    ; ED 7B ld sp, (nn)
	jp	z80_mltsp	; ED 7C mlt sp
	jp	z80_nop		; ED 7D stmix
	jp	z80_nop		; ED 7E rsmix
	jp	z80_nop		; ED 7F
	jp	z80_nop		; ED 80
	jp	z80_nop		; ED 81
	jp	z80_inim	; ED 82 inim
	jp	z80_otim	; ED 83 otim
	jp	z80_ini2	; ED 84 ini2
	jp	z80_nop		; ED 85
	jp	z80_nop		; ED 86
	jp	z80_nop		; ED 87
	jp	z80_nop		; ED 88
	jp	z80_nop		; ED 89
	jp	z80_indm	; ED 8A indm
	jp	z80_otdm	; ED 8B otdm
	jp	z80_ind2	; ED 8C ind2
	jp	z80_nop		; ED 8D
	jp	z80_nop		; ED 8E
	jp	z80_nop		; ED 8F
	jp	z80_nop		; ED 90
	jp	z80_nop		; ED 91
	jp	z80_inimr	; ED 92 inimr
	jp	z80_otimr	; ED 93 otimr
	jp	z80_ini2r	; ED 94 ini2r
	jp	z80_nop		; ED 95
	jp	z80_nop		; ED 96
	jp	z80_nop		; ED 97
	jp	z80_nop		; ED 98
	jp	z80_nop		; ED 99
	jp	z80_indmr	; ED 9A indmr
	jp	z80_ordmr	; ED 9B otdmr
	jp	z80_ind2r	; ED 9C ind2r
	jp	z80_nop		; ED 9D
	jp	z80_nop		; ED 9E
	jp	z80_nop		; ED 9F
	jp	z80_ldi		; ED A0 ldi
	jp	z80_cpi		; ED A1 cpi
	jp	z80_ini		; ED A2 ini
	jp	z80_outi        ; ED A3 outi
	jp	z80_outi2	; ED A4 outi2
	jp	z80_nop		; ED A5
	jp	z80_nop		; ED A6
	jp	z80_nop		; ED A7
	jp	z80_ldd		; ED A8 ldd
	jp	z80_cpd		; ED A9 cpd
	jp	z80_ind		; ED AA ind
	jp	z80_outd        ; ED AB outd
	jp	z80_outd2	; ED AC outd2
	jp	z80_nop		; ED AD
	jp	z80_nop		; ED AE
	jp	z80_nop		; ED AF
	jp	z80_ldir        ; ED B0 ldir
	jp	z80_cpir        ; ED B1 cpir
	jp	z80_inir        ; ED B2 inir
	jp	z80_otir        ; ED B3 otir
	jp	z80_oti2r	; ED B4 oti2r
	jp	z80_nop		; ED B5
	jp	z80_nop		; ED B6
	jp	z80_nop		; ED B7
	jp	z80_lddr        ; ED B8 lddr
	jp	z80_cpdr        ; ED B9 cpdr
	jp	z80_indr        ; ED BA indr
	jp	z80_otdr        ; ED BB otdr
	jp	z80_otd2r	; ED BC otd2r
	jp	z80_nop		; ED BD
	jp	z80_nop		; ED BE
	jp	z80_nop		; ED BF
	jp	z80_nop		; ED C0
	jp	z80_nop		; ED C1
	jp	z80_inirx	; ED C2 inirx
	jp	z80_otirx	; ED C3 otirx
	jp	z80_nop		; ED C4
	jp	z80_nop		; ED C5
	jp	z80_nop		; ED C6
	jp	z80_nop		; ED C7 ld i, hl
	jp	z80_nop		; ED C8
	jp	z80_nop		; ED C9
	jp	z80_indrx	; ED CA indrx
	jp	z80_otdrx	; ED CB otdrx
	jp	z80_nop		; ED CC
	jp	z80_nop		; ED CD
	jp	z80_nop		; ED CE
	jp	z80_nop		; ED CF
	jp	z80_nop		; ED D0
	jp	z80_nop		; ED D1
	jp	z80_nop		; ED D2
	jp	z80_nop		; ED D3
	jp	z80_nop		; ED D4
	jp	z80_nop		; ED D5
	jp	z80_nop		; ED D6
	jp	z80_nop		; ED D7 ld hl, i
	jp	z80_nop		; ED D8
	jp	z80_nop		; ED D9
	jp	z80_nop		; ED DA
	jp	z80_nop		; ED DB
	jp	z80_nop		; ED DC
	jp	z80_nop		; ED DD
	jp	z80_nop		; ED DE
	jp	z80_nop		; ED DF
	jp	z80_nop		; ED E0
	jp	z80_nop		; ED E1
	jp	z80_nop		; ED E2
	jp	z80_nop		; ED E3
	jp	z80_nop		; ED E4
	jp	z80_nop		; ED E5
	jp	z80_nop		; ED E6
	jp	z80_nop		; ED E7
	jp	z80_nop		; ED E8
	jp	z80_nop		; ED E9
	jp	z80_nop		; ED EA
	jp	z80_nop		; ED EB
	jp	z80_nop		; ED EC
	jp	z80_nop		; ED ED
	jp	z80_nop		; ED EE
	jp	z80_nop		; ED EF
	jp	z80_nop		; ED F0
	jp	z80_nop		; ED F1
	jp	z80_nop		; ED F2
	jp	z80_nop		; ED F3
	jp	z80_nop		; ED F4
	jp	z80_nop		; ED F5
	jp	z80_nop		; ED F6
	jp	z80_nop		; ED F7
	jp	z80_nop		; ED F8
	jp	z80_nop		; ED F9
	jp	z80_nop		; ED FA
	jp	z80_nop		; ED FB
	jp	z80_nop		; ED FC
	jp	z80_nop		; ED FD
	jp	z80_nop		; ED FE
	jp	z80_nop		; ED FF

	; $ED $19 out0 (n), e
z80_out0_n_e:
	ld	b, 0
	ld.s	c, (iy)
	inc	iy
	exx
	ld	a, e
	exx
	out	(bc), a
	z80loop

	; ED 22 lea hl, ix+d
	z80_irtoix	leahlixd, ix, {lea hl, ix+0}

	; $ED $38 in0 a, (n)
z80_in0a_n_:
	ld	b, 0
	ld.s	c, (iy)
	inc	iy
	ex	af, af'
	in	a, (bc)
	ex	af, af'
	z80loop

	; $ED $39 out0 (n), a
z80_out0_n_a:
	ld	b, 0
	ld.s	c, (iy)
	inc	iy
	ex	af, af'
	out	(bc), a
	ex	af, af'
	z80loop

	; $ED 40 in b, (bc)
 z80_inb_c:
	exx
	ld	b, IO_SEGMENT
	in	b, (bc)
	exx
	z80loop

	; $ED $41 out (c), b
z80_out_c_b:
	exx
	push	bc
	ld	a, b
	ld	b, IO_SEGMENT
	out	(bc), a
	pop	bc
	exx
	z80loop

	; $ED $42 sbc hl, bc
	z80_exall2	sbchlbc, {sbc.s hl, bc}

	; $ED 43 ld (nn), bc
z80_ld_nn_bc:
	ld.s	hl, (iy)
	lea	iy, iy+2
	exx
	push	bc
	exx
	pop	bc
	ld.s	(hl), bc
	z80loop

	; $ED 44 neg
	z80_exaf2	neg, {neg}

	; $ED 45 retn
z80_retn:
	pop.s	iy
	z80loop

	; $ED 46 im 0
z80_im0:	; IM 1 is always active and only mode supported
	z80loop

	; $ED 47 ld i, a
	; TODO testing
	z80_exaf2	ldia, {ld i, a}

	; $ED 48 in c, (c)
	; TODO testing
z80_inc_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	a, (bc)
	pop	bc
	ld	c, a
	exx
	z80loop

	; $ED $49 out (c), c
z80_out_c_c:
	exx
	push	bc
	ld	b, IO_SEGMENT
	out	(bc), c
	pop	bc
	exx
	z80loop

	; $ED 4A adc hl, bc
	z80_exall2	adchlbc, {adc.s hl, bc}

	; $ED 48 ld bc, (nn)
z80_ld_bc_nn:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ld.s	bc, (hl)
	push	bc
	exx
	pop	bc
	exx
	z80loop

	; $ED 4D reti - redirected to ret
z80_reti:
	pop.s	iy
	z80loop

	; $ED 4F ld r, a
	; TODO testing
	z80_exaf2	ldra, {ld r, a}

	; $ED 50 in d, (c)
	; TODO testing
z80_ind_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	d, (bc)
	pop	bc
	exx
	z80loop

	; $ED 59 out (c), d
	z80_out_c_jj	d

	; $ED 52 add hl, de
	z80_exall2	sbchlde, {sbc.s hl, de}

	; $ED 53 ld (nn), de
z80_ld_nn_de:
	ld.s	hl, (iy)
	lea	iy, iy+2
	exx
	push	de
	exx
	pop	bc
	ld.s	(hl), bc
	z80loop

	; $ED 56 im 1
z80_im1:	; IM 1 is always active and only mode supported
	z80loop

	; $ED 57 ld a, i
	; simulate the i register - by setting the P/V flag
	; as per current emulated IEFx state
	; will be 0 (flag PE) when EI, 1 (flag PO) when DI
	z80_exaf2	ldai, {ld a, (ix+z80_flags)}, {and a, 1}, {ld a, 0}

	; $ED 58 in e, (c)
	; TODO testing
z80_ine_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	e, (bc)
	pop	bc
	exx
	z80loop

	; $ED 59 out (c), e
	z80_out_c_jj	e

	; $ED 5A adc hl, de
	z80_exall2	adchlde, {adc.s hl, de}

	; $ED 5B ld de, (nn)
z80_ld_de_nn:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ld.s	hl, (hl)
	push	hl
	exx
	pop	de
	exx
	z80loop

	; $ED 5E
z80_im2:	; IM 1 is always active and only mode supported
	z80loop

	; $ED 5F ld a, r
	z80_exaf2	ldar, {ld a, r}

	; $ED 60 in h, (c)
	; TODO testing
z80_inh_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	h, (bc)
	pop	bc
	exx
	z80loop

	; $ED 61 out (c), h
	z80_out_c_jj	h

	; $ED 62 add hl, hl
	z80_exall2	sbchlhl, {sbc.s hl, hl}

	; $ED 67 rrd (hl)
	z80_exall2	rrd_hl_, {db %52}, {rrd (hl)}

	; $ED 58 in l, (c)
	; TODO testing
z80_inl_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	l, (bc)
	pop	bc
	exx
	z80loop

	; $ED 69 out (c), l
	z80_out_c_jj	l

	; $ED 6A adc hl, hl
	z80_exall2	adchlhl, {adc.s hl, hl}

	; $ED 6F rld (hl)
	z80_exall2	rld_hl_, {db %52}, {rld (hl)}

	; $ED $72 sbc hl, sp
	z80_exall2	sbchlsp, {sbc.s hl, sp}

	; $ED $83 ld (nn), sp
z80_ld_nn_sp:
	ld	hl, 0
	add.s	hl, sp
	ld.s	de, (iy)
	lea	iy, iy+2
	ex	de, hl
	ld.s	(hl), de
	z80loop

	; $ED 78 in a, (c)
	; TODO testing
z80_ina_c_:
	exx
	push	bc
	ld	b, IO_SEGMENT
	ex	af, af'
	in	a, (bc)
	ex	af, af'
	pop	bc
	exx
	z80loop

	; $ED 79 out (c), a
z80_out_c_a:
	exx
	push	bc
	ld	b, IO_SEGMENT
	ex	af, af'
	out	(bc), a
	ex	af, af'
	pop	bc
	exx
	z80loop

	; $ED $7A adc hl, sp
	z80_exall2	adchlsp, {adc.s hl, sp}

	; $ED 7B ld sp, (nn)
z80_ldsp_nn_:
	ld.s	hl, (iy)
	lea	iy, iy+2
	ld.s	hl, (hl)
	ld.s	sp, hl
	z80loop

	; $ED A0 ldi
	z80_exall2	ldi, {ldi.s}

	; $ED A1
	z80_exall2	cpi, {cpi.s}

	; $ED A2 ini
	; TODO testing
z80_ini:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	a, (bc)
	ld.s	(hl), a
	inc	hl
	pop	bc
	ex	af, af'
	dec	b		; flag may not be consistent with ini
	ex	af, af'
	exx
	z80loop

	; $ED A3
z80_outi:
	exx
	push	bc
	ld	b, IO_SEGMENT
	ld.s	a, (hl)
	out	(bc), a
	inc	hl
	pop	bc
	ex	af, af'
	dec	b		; flag may not be consistent with outi
	ex	af, af'
	exx
	z80loop

	; $ED A8 ldd
	z80_exall2	ldd, {ldd.s}

	; $ED A9 cpd
	z80_exall2	cpd, {cpd.s}

	; $ED AA ind
	; TODO testing
z80_ind:
	exx
	push	bc
	ld	b, IO_SEGMENT
	in	a, (bc)
	ld.s	(hl), a
	dec	hl
	pop	bc
	ex	af, af'
	dec	b		; flag may not be consistent with ini
	ex	af, af'
	exx
	z80loop

	; $ED AB outd
	; TODO testing
z80_outd:
	exx
	push	bc
	ld	b, IO_SEGMENT
	ld.s	a, (hl)
	out	(bc), a
	dec	hl
	pop	bc
	ex	af, af'
	dec	b		; flag may not be consistent with outi
	ex	af, af'
	exx
	z80loop

	; $ED $B0
z80_ldir:
	exx
	ex	af, af'
ldirnext:
	ldi.s				; ldir.s seems to have some
	jp	pe, ldirnext		; kind of compatibility issue
	ex	af, af'
	exx
	z80loop

	; $ED B1
	z80_exall2	cpir, {cpir.s}

	; $ED B2 inir
z80_inir:
	ex	af, af'
	exx
inir1:
	ex	af, af'
	push	bc
	ld	b, IO_SEGMENT

	io_rate_start
	in	a, (bc)
	ld.s	(hl), a
	io_rate_end

	pop	bc
	inc	hl
	ex	af, af'
	djnz	inir1
	ex	af, af'
	exx
	z80loop

	; $ED B3 otir
z80_otir:
	ex	af, af'
	exx
otir1:
	ex	af, af'

	push	bc
	ld	b, IO_SEGMENT

	io_rate_start
	ld.s	a, (hl)
	out	(bc), a
	io_rate_end

	pop	bc
	inc	hl
	ex	af, af'
	djnz	otir1
	ex	af, af'
	exx

	z80loop

	; $ED d8 lddr
z80_lddr:
	exx
	ex	af, af'
lddrnext:
	ldd.s
	jp	pe, lddrnext
	ex	af, af'
	exx
	z80loop

	; $ED B9 cpdr
	z80_exall2	cpdr, {cpdr.s}

z80_indr:
z80_otdr:
z80_inimr:
z80_inirx:
z80_otdrx:
z80_leabciyd:
z80_in0l_n_:
z80_in0h_n_:
z80_in0c_n_:
z80_in0b_n_:
z80_ldix_hl_:
z80_tstaa:
z80_indmr:
z80_tstab:
z80_otirx:
z80_tstac:
z80_slp:
z80_tstad:
z80_indrx:
z80_tstae:
z80_leahliyd:
z80_tstion:
z80_leaiyiyd:
z80_tstah:
z80_peaiyd:
z80_otim:
z80_ldiy_hl_:
z80_mltde:
z80_tstal:
z80_ordmr:
z80_ind2r:
z80_tstan:
z80_ld_hl_de:
z80_ldbc_hl_:
z80_ld_hl_ix:
z80_ld_hl_iy:
z80_indm:
z80_outi2:
z80_in0e_n_:
z80_out0_n_b:
z80_out0_n_c:
z80_tsta_hl_:
z80_out0_n_d:
z80_outd2:
z80_leadeixd:
z80_leaiyixd:
z80_ini2:
z80_out0_n_h:
z80_leaixixd:
z80_otimr:
z80_peaixd:
z80_mltbc:
z80_out0_n_l:
z80_oti2r:
z80_ld_hl_bc:
z80_leabcixd:
z80_mlthl:
z80_in0d_n_:
z80_otd2r:
z80_ldde_hl:
z80_inim:
z80_ini2r:
z80_ld_hl_hl:
z80_leadeiyd:
z80_ldhl_hl_:
z80_leaixiyd:
z80_otdm:
z80_ind2:
z80_mltsp:
.if EN_EZ80_INSTR
	jp	not_implemented
.else

.if EN_NOP_UNKNOWN
	z80loop
.else
	jp	not_implemented
.endif

.endif


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

z80_load_all_registers:
	ld	bc, (ix+z80_reg_aaf)
	push	bc
	pop	af
	ex	af, af'

	ld	bc, (ix+z80_reg_abc)
	ld	de, (ix+z80_reg_ade)
	ld	hl, (ix+z80_reg_ahl)
	exx

	ld	iy, (ix+z80_reg_iy)
	ld	ix, (ix+z80_reg_ix)
	ret

z80_save_all_registers:
	ld	(_reg_ix), ix
	ld	ix, z80_regs
	ld	(ix+z80_reg_iy), iy

	exx
	ld	(ix+z80_reg_abc), bc
	ld	(ix+z80_reg_ade), de
	ld	(ix+z80_reg_ahl), hl
	ex	af, af'
	push	af
	pop	bc
	ld	(ix+z80_reg_aaf), bc
	ret

z80_set_int_state:
	LD	A, I
	JP	PO, z80_set_di
	res	1, (ix+z80_flags)
	ret
z80_set_di:
	set	1, (ix+z80_flags)
	ret


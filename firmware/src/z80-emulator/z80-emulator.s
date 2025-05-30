	include	"..\config.inc"
	include	"z80-emulator-macros.inc"

	.assume	adl=1

; ez80's alt version of BC, DE, HL contain the emulated z80's main registers
; the ez80's alt registers are stored in reg_aXX below:

	section	BSS

	global	z80_regs
	global	_reg_ahl
	global	_reg_ade
	global	_reg_iy
	global z80_reg_ix
	global z80_reg_iy
; registers
z80_regs:
_reg_aaf:	ds	3
z80_reg_aaf	equ	0

_reg_abc:	ds	3
z80_reg_abc	equ	3

_reg_ade:	ds	3
z80_reg_ade	equ	6

_reg_ahl:	ds	3
z80_reg_ahl	equ	9

_reg_ix:	ds	3
z80_reg_ix	equ	12

_reg_iy:	ds	3
z80_reg_iy	equ	15

	global	_reg_spl

_reg_spl:	ds	3
z80_reg_spl	equ	18

_z80_flags:	db 	1		; bit 0 -> ei-state, bit 1 -> int-pending
z80_flags	equ	21		; true if maskable ints enabled

	section	CODE
	global	z80_invoke

; test_poi:
; 	push	iy
; 	pop	hl
; 	ld	de, %f398
; 	or	a
; 	sbc.s	hl, de
; 	ret	nz

; 	ld	hl, (ix+z80_reg_ix)
; 	ld	de, %66
; 	or	a
; 	sbc.s	hl, de
; 	ret	nz
; poi:
; 	nop
; 	ret

	section	INTERNAL_RAM_ROM
z80_int_request:
	xor	a
	ld	(ix+z80_flags), a
	push.s	iy
	ld	iy, %0038
	jr	z80_loop2

z80_loop:
	ld	a, (ix+z80_flags)
	bit	0, a			; ei enabled?
	jr	z, z80_loop2

	bit	1, a			; int pending?
	jr	nz, z80_int_request

z80_loop2:
	z80_byte_jump	z80_instr_table

	z80_byte_jump	z80_instr_table
	section CODE

; start executing z80 code at location: MBASE/IY
z80_invoke:
	ld	ix, z80_regs

	global	z80_nop

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
	jp	z80_jppnn		; E2
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
	ret

	; $00 nop
z80_nop:
	z80loop

	; $01 ld bc, nn
	z80_exmain2	ldbcnn, {ld.s bc, (iy)}, {inc iy}, {inc iy}

	; $02 ld (bc), a
	z80_exall2	ld_bc_a, {ld.s (bc), a}

	; $03 inc bc
	z80_exmain2	incbc, {inc bc}

	; $04 inc b
	z80_exall2	incb, {inc b}

	; $05 dec b
	z80_exall2	decb, {dec b}

	; $06 ld b, n
z80_ldbn:
	z80_exmain	{ld.s	b, (iy)}, {inc iy}

	; $07 rlca
z80_rlca:
	z80_exaf	{rlca}

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
z80_ldcn:
	z80_exmain	{ld.s c, (iy)}, {inc iy}

	; $0F rrca
z80_rrca:
	z80_exaf	{rrca}

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

	; $10 djnz d
z80_djnzd:
	z80_jrccd	djnz

	; $11 ld de, nn
z80_lddenn:
	z80_exmain	{ld.s de, (iy)}, {inc iy}, {inc iy}

	; $12 ld (de), a
	z80_exall2	ld_de_a, {ld.s (de), a}

	; $13 inc de
	z80_exmain2	incde, {inc de}

	; $14 inc d
	z80_exall2	incd, {inc d}

	; $15 dec d
	z80_exall2	decd, {dec d}

	; $16 ld d, n
z80_lddn:
	z80_exmain	{ld.s d, (iy)}, {inc iy}

	; $17 rla
	z80_exaf2	rla, {rla}

	; $18 JR d
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

	; $19 add hl, de
	z80_exall2	addhlde, {add.s hl, de}

	; $1A ld a, (de)
z80_lda_de_:
	z80_exall	{ld.s a, (de)}

	; $1B dec de
z80_decde:
	z80_exmain	{dec de}

	; $1D inc e
	z80_exall2	ince, {inc e}

	; $1D dec e
	z80_exall2	dece, {dec e}

	; $1E ld e, n
	z80_exmain2  	lden, {ld.s e, (iy)}, {inc iy}

	; $1F rra
	z80_exaf2	rra, {rra}

	; $20 jr nz, d
z80_jrnzd:
	z80_jrccd	{jr nz,}

	; $21 ld hl, nn
z80_ldhlnn:
	z80_exmain	{ld.s hl, (iy)}, {inc iy}, {inc iy}

	; $22 ld (nn), hl
z80_ld_nn_hl:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
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
z80_ldhn:
	exx
	ld.s	h, (iy)
	inc	iy
	exx
	z80loop

	; $27 daa
	z80_exaf2	daa, {daa}

	; $28 jr z, d
z80_jrzd:
	inc	iy
	ex	af, af'
	jr	z, z80_jrzd1
	ex	af, af'
	z80loop

z80_jrzd1:
	ex	af, af'
	ld.s	a, (iy-1)
	jp	_z80_add_a_to_pc

	; $29 add hl, hl
	z80_exall2	addhlhl, {add.s	hl, hl}

	; $2A ld hl, (nn)
z80_ldhl_nn_:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
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
z80_dechl:
	exx
	dec	hl
	exx
	z80loop

	; $2C inc l
	z80_exall2	incl, {inc l}

	; $2D dec l
	z80_exall2	decl, {dec l}

	; $2e ld l, n
z80_ldln:
	exx
	ld.s	l, (iy)
	inc	iy
	exx
	z80loop

	; $2F cpl
	z80_exaf2	cpl, {cpl}

	; $30 jr nc, d
z80_jrncd:
	inc	iy
	ex	af, af'
	jr	nc, z80_jrncd1
	ex	af, af'
	z80loop

z80_jrncd1:
	ex	af, af'
	ld.s	a, (iy-1)
	jp	_z80_add_a_to_pc

	; $31 ld sp, nn
z80_ldspnn:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld.s	sp, hl
	z80loop

	; $32 ld (nn), a
z80_ld_nn_a:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
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
z80_jrcd:
	inc	iy
	ex	af, af'
	jr	c, z80_jrcd1
	ex	af, af'
	z80loop

z80_jrcd1:
	ex	af, af'
	ld.s	a, (iy-1)
	jp	_z80_add_a_to_pc

	; $39 add hl, sp
	z80_exall2	addhlsp, {add.s hl, sp}

	; $3A ld a, (nn)
z80_lda_nn_:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
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

	; $40 ld b,b
z80_ldbb:
	exx
	; ld	b,b
	exx
	z80loop


z80_ldbc:
	call	not_implemented
	z80loop

	; $42 ld b, d
	z80_exmain2	ldbd, {ld b, d}


z80_ldbe:
	call	not_implemented
	z80loop

	; %44 ld b, h
z80_ldbh:
	exx
	ld	b, h
	exx
	z80loop

	; $45 ld b, l
z80_ldbl:
	exx
	ld	b, l
	exx
	z80loop

	; $46 ld b, (hl)
	z80_exmain2	ldb_hl_, {db %52}, {ld b, (hl)}	; bug in assembler ld.s b, (hl) not supported??

	; $47 ld b, a
z80_ldba:
	exx
	ex	af, af'
	ld	b, a
	ex	af, af'
	exx
	z80loop

	; $48 ld c, b
z80_ldcb:
	exx
	ld	c, b
	exx
	z80loop

	; 49 ld c, c
z80_ldcc:
	ld.s	a, (iy)
	cp	%D7		; RST.L %10
	jr	z, rst_l10
	cp	%C9		; RET.L
	jr	z, ret_l
	call	not_implemented
	z80loop
ret_l:
rst_l10:
	nop	; RST.L %10
	nop
	nop
	jr	rst_l10

	; $4A ld c, d
z80_ldcd:
	exx
	ld	c, d
	exx
	z80loop

	; $4B ld c, e
z80_ldce:
	exx
	ld	c, e
	exx
	z80loop

	; $4C ld c, h
z80_ldch:
	exx
	ld	c, h
	exx
	z80loop

	; $4D ld c, l
z80_ldcl:
	exx
	ld	c, l
	exx
	z80loop

	; $4E ld c, (hl)
	z80_exmain2	ldc_hl_, {db %52}, {ld c, (hl)}	; bug in assembler ld.s c, (hl) not supported??

	; $4F ld c, a
z80_ldca:
	exx
	ex	af, af'
	ld	c, a
	ex	af, af'
	exx
	z80loop

	; $50 ld d, b
	z80_exmain2	lddb, {ld d, b}

	; $51 ld d, c
	z80_exmain2	lddc, {ld d, c}

	; $52 ld d, d
z80_lddd:
	call	not_implemented
	z80loop

	; $53 ld d, e
	z80_exmain2	ldde, {ld d, e}

	; $54 ld d, h
	z80_exmain2	lddh, {ld d, h}

	; $55 ld d, l
	z80_exmain2	lddl, {ld d, l}


z80_ldd_hl_:
	call	not_implemented
	z80loop

	; $57 ld d, a
z80_ldda:
	z80_exall 	{ld d, a}

	; $58 ld e, b
z80_ldeb:
	z80_exmain 	{ld e, b}

	; $59 ld e, c
z80_ldec:
	z80_exmain 	{ld e, c}

	; $5A ld e, d
z80_lded:
	z80_exmain 	{ld e, d}

	; $5B ld e, e
z80_ldee:
	call	not_implemented
	; z80_exmain 	{ld e, e}
	z80loop

	; $5C ld e, h
z80_ldeh:
	z80_exmain 	{ld e, h}

	; $5D ld e, l
z80_ldel:
	z80_exmain 	{ld e, l}

	; $5E ld e, (hl)
	z80_exmain2	lde_hl_, {db %52}, {ld e, (hl)}	; bug in assembler ld.s e, (hl) not supported??

	; $5f	ld e, a
z80_ldea:
	z80_exall	{ld e, a}

z80_ldhb:
	call	not_implemented
	z80loop


z80_ldhc:
	call	not_implemented
	z80loop

	; $62 ld h, d
z80_ldhd:
	exx
	ld	h, d
	exx
	z80loop

	; $63 ld h, e
z80_ldhe:
	exx
	ld	h, e
	exx
	z80loop


z80_ldhh:
	call	not_implemented
	z80loop


z80_ldhl:
	call	not_implemented
	z80loop

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

z80_ldll:
	call	not_implemented
	z80loop


z80_ldl_hl_:
	call	not_implemented
	z80loop

	; $6F ld l,a
z80_ldla:
	exx
	ex	af, af'
	ld	l, a
	ex	af, af'
	exx
	z80loop

	; $70 ld (hl), b
z80_ld_hl_b:
	exx
	ld.s	(hl), b
	exx
	z80loop

	; $72 ld (hl), c
z80_ld_hl_c:
	exx
	ld.s	(hl), c
	exx
	z80loop

	; $72 ld (hl), d
z80_ld_hl_d:
	exx
	ld.s	(hl), d
	exx
	z80loop

	; $73 ld (hl), e
z80_ld_hl_e:
	exx
	ld.s	(hl), e
	exx
	z80loop

	; $74 ld (hl), h
z80_ld_hl_h:
	exx
	ld.s	(hl), h
	exx
	z80loop

	; $75 ld (hl), e
z80_ld_hl_l:
	exx
	ld.s	(hl), l
	exx
	z80loop


z80_halt:
	bit	1, (ix+z80_flags)		; int pending?
	jr	z, z80_halt
	nop
	nop
	nop
	nop
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


z80_ldaa:
	call	not_implemented
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
z80_popbc:
	exx
	pop.s	bc
	exx
	z80loop

z80_jpccnn	macro j, cc
z80_jp&cc&nn:
	ex	af, af'
	j	cc, $$skip
	ex	af, af'
	inc	iy
	inc	iy
	z80loop

$$skip:
	ex	af, af'
	ld.s	iy, (iy)
	z80loop
	endmacro

	; $C2 jp nz, nn
	z80_jpccnn	jr, nz

	; $C3 JP nn
z80_jpnn:
	ld.s	iy, (iy)
	z80loop

z80_callccnn	macro	cc
z80_call&cc&nn:
	ex	af, af'
	jp	cc, $$skip
	ex	af, af'
	inc	iy
	inc	iy
	z80loop
$$skip:
	ex	af, af'
	pea.s	iy+2
	ld.s	iy, (iy)
	z80loop

	endmacro

	; $ C4 call nz, nn
	z80_callccnn	nz

	; $c5 push bc
z80_pushbc:
	exx
	push.s	bc
	exx
	z80loop

	; $C6 add a, n
z80_addan:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	add	a, b
	ex	af, af'
	z80loop

z80_rst00:
	call	not_implemented
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
z80_bit:
	ld.s	a, (iy)
	inc	iy
	cp	%31		;
	jp	z, z80_switch_to_native
	ld	(bit_instr), a
	exx
	ex	af, af'
	jp	z80_bit2

	section	INTERNAL_RAM_ROM
z80_bit2:
	db	%52	; .S prefix
	db	%CB	; bit operand
bit_instr:
	db	%00
	ex	af, af'
	exx
	z80loop

	section	CODE
z80_switch_to_native:
	; need to load all registers correctly
	; then jump.s to original value of iy

	ld	a, iyl
	ld	(.switch_addr+2), a
	ld	a, iyh
	ld	(.switch_addr+3), a

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

	jp	.switch_addr

	section	INTERNAL_RAM_ROM
.switch_addr:
	jp.s	%0000
	section	CODE


	; $CC call z, nn
	z80_callccnn	z

	; $cd nn
z80_callnn:
	pea.s	iy+2
	ld.s	iy, (iy)
	z80loop

	; $CE adc a, n
	z80_exaf2	adcan, {adc.s a, (iy)}, {inc iy}


z80_rst08:
	call	not_implemented
	z80loop

	; $D0 ret nc
	z80_retcc	jr, nc

	; $d1 pop de
z80_popde:
	exx
	pop.s	de
	exx
	z80loop

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

	; $d5 push de
z80_pushde:
	exx
	push.s	de
	exx
	z80loop

	; $d6 sub a, n
z80_subn:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	sub	a, b
	ex	af, af'
	z80loop


z80_rst10:
	call	not_implemented
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
z80_ina_n_:
	ld.s	c, (iy)
	inc	iy
	ld	b, IO_SEGMENT
	ex	af, af'
	in	a, (bc)
	ex	af, af'
	z80loop

	; $DC call c, nn
	z80_callccnn	c

	; $DD .....
	XREF	z80_ix

	; $DE sbc a, n
	z80_exaf2	sbcan, {sbc.s a, (iy)}, {inc iy}


z80_rst18:
	push.s	iy
	ld	iy, %18
	z80loop

	; $E0 ret po
	z80_retcc	jp, po

	; $e1 pop hl
z80_pophl:
	exx
	pop.s	hl
	exx
	z80loop

	; $E2 jp po, nn
	z80_jpccnn	jp, po

z80_ex_sp_hl:
	exx
	ex.s	(sp), hl
	exx
	z80loop

	; $E4 call po, nn
	z80_callccnn	po

	; $e5 push hl
z80_pushhl:
	exx
	push.s	hl
	exx
	z80loop

	; $e6 and a, n
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
z80_exdehl:
	exx
	ex	de, hl
	exx
	z80loop

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
z80_popaf:
	ex	af, af'
	pop.s	af
	ex	af, af'
	z80loop

	; $F2 jp p, nn
	z80_jpccnn	jp, p

	; $F3 DI
z80_di:
	res	0, (ix+z80_flags)
	di
	z80loop

	; $F4 call p, nn
	z80_callccnn	p

	; $f5 push af
z80_pushaf:
	ex	af, af'
	push.s	af
	ex	af, af'
	z80loop

	; $f6 or a, n
z80_oran:
	ld.s	b, (iy)
	inc	iy
	ex	af, af'
	or	a, b
	ex	af, af'
	z80loop


z80_rst30:
	call	not_implemented
	z80loop

	; $f8 ret m
	z80_retcc	jp, m

	; $F9 ld sp, hl
z80_ldsphl:
	exx
	ld.s	sp, hl
	exx
	z80loop

	; $FA jp m, nn
	z80_jpccnn	jp, m


z80_ei:
	set	0, (ix+z80_flags)
	ei
	jp	z80_loop2

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
	section	INTERNAL_RAM_ROM
	; $ED
z80_misc:
	z80_byte_jump	z80_instr_misc_table
	section CODE

z80_instr_misc_table:
	jp	z80_nop		; ED 00 in0 b,(n)
	jp	z80_nop		; ED 01 out0 (n), b
	jp	z80_nop		; ED 02 lea bc, ix+d
	jp	z80_nop		; ED 03 lea bc, iy+d
	jp	z80_nop		; ED 04 tst a, b
	jp	z80_nop		; ED 05
	jp	z80_nop		; ED 06
	jp	z80_nop		; ED 07 ld bc, (hl)
	jp	z80_nop		; ED 08 in0 c, (n)
	jp	z80_nop		; ED 09 out0 (n),c
	jp	z80_nop		; ED 0A
	jp	z80_nop		; ED 0B
	jp	z80_nop		; ED 0C tst a, c
	jp	z80_nop		; ED 0D
	jp	z80_nop		; ED 0E
	jp	z80_nop		; ED 0F ld (hl), bc
	jp	z80_nop		; ED 10 in0 d, (n)
	jp	z80_nop		; ED 11 out0 (n), d
	jp	z80_nop		; ED 12 lea de, ix+d
	jp	z80_nop		; ED 13 lea de, iy+d
	jp	z80_nop		; ED 14 tst a, d
	jp	z80_nop		; ED 15
	jp	z80_nop		; ED 16
	jp	z80_nop		; ED 17 ld de, (hl)
	jp	z80_nop		; ED 18 in0 e, (n)
	jp	z80_out0_n_e	; ED 19 out0 (n), e
	jp	z80_nop		; ED 1A
	jp	z80_nop		; ED 1B
	jp	z80_nop		; ED 1C tst a, e
	jp	z80_nop		; ED 1D
	jp	z80_nop		; ED 1E
	jp	z80_nop		; ED 1F ld (hl), de
	jp	z80_nop		; ED 20 in0 h, (n)
	jp	z80_nop		; ED 21 out0 (n), h
	jp	z80_nop		; ED 22 lea hl, ix+d
	jp	z80_nop		; ED 23 lea hl, iy+d
	jp	z80_nop		; ED 24 tst a, h
	jp	z80_nop		; ED 25
	jp	z80_nop		; ED 26
	jp	z80_nop		; ED 27 ld hl, (hl)
	jp	z80_nop		; ED 28 in0 l, (n)
	jp	z80_nop		; ED 29 out0 (n), l
	jp	z80_nop		; ED 2A
	jp	z80_nop		; ED 2B
	jp	z80_nop		; ED 2C tst a, l
	jp	z80_nop		; ED 2D
	jp	z80_nop		; ED 2E
	jp	z80_nop		; ED 2F ld (hl), hl
	jp	z80_nop		; ED 30
	jp	z80_nop		; ED 31 ld iy, (hl)
	jp	z80_nop		; ED 32 lea ix, ix+d
	jp	z80_nop		; ED 33 lea iy, iy+d
	jp	z80_nop		; ED 34 tst a, (hl)
	jp	z80_nop		; ED 35
	jp	z80_nop		; ED 36
	jp	z80_nop		; ED 37 ld ix, (hl)
	jp	z80_in0a_n_	; ED 38 in0 a, (n)
	jp	z80_out0_n_a	; ED 39 out0 (n), a
	jp	z80_nop		; ED 3A
	jp	z80_nop		; ED 3B
	jp	z80_nop		; ED 3C tst a, a
	jp	z80_nop		; ED 3D
	jp	z80_nop		; ED 3E ld (hl), iy
	jp	z80_nop		; ED 3F ld (hl), ix
	jp	z80_inb_c       ; ED 40 in b, (bc)
	jp	z80_out_c_b     ; ED 41 out (bc), b
	jp	z80_sbchlbc   ; ED 42 sbc hl, bc
	jp	z80_ld_nn_bc    ; ED 43 ld (nn), bc
	jp	z80_neg		; ED 44 neg
	jp	z80_retn        ; ED 45 retn
	jp	z80_im0		; ED 46 im 0
	jp	z80_ld_i_a      ; ED 47 ld i, a
	jp	z80_in_c_c      ; ED 48 in c, (bc)
	jp	z80_out_c_c     ; ED 49 out (bc), c
	jp	z80_adchlbc	; ED 4A adc hl, bc
	jp	z80_ld_bc_nn    ; ED 4B ld bc, (nn)
	jp	z80_nop		; ED 4C mlt bc
	jp	z80_reti	; ED 4D reti
	jp	z80_nop		; ED 4E
	jp	z80_ld_r_a      ; ED 4F ld r,a
	jp	z80_in_d_c      ; ED 50 in d, (bc)
	jp	z80_out_c_d     ; ED 51 out (bc), d
	jp	z80_sbchlde	; ED 52 sbc hl, de
	jp	z80_ld_nn_de    ; ED 53 ld (nn), de
	jp	z80_nop		; ED 54 lea ix, iy+d
	jp	z80_nop	        ; ED 55 lea iy, ix+d
	jp	z80_im1		; ED 56 im 1
	jp	z80_ldai	; ED 57 ld a, i
	jp	z80_in_e_c      ; ED 58 in e, (bc)
	jp	z80_out_c_e     ; ED 59 out (bc), e
	jp	z80_adchlde	; ED 5A adc hl, de
	jp	z80_ld_de_nn    ; ED 5B ld de, (nn)
	jp	z80_nop		; ED 5C mlt de
	jp	z80_nop		; ED 5D
	jp	z80_im2		; ED 5E im 2
	jp	z80_ld_a_r      ; ED 5F ld a, r
	jp	z80_in_h_c      ; ED 60 in h, (bc)
	jp	z80_out_c_h     ; ED 61 out (bc), h
	jp	z80_sbchlhl	; ED 62 sbc hl, hl
	jp	z80_nop		; ED 63 ld (nn), hl
	jp	z80_nop		; ED 64 tst a, n
	jp	z80_nop		; ED 65 pea ix+d
	jp	z80_nop		; ED 66 pea iy+d
	jp	z80_rrd_hl_	; ED 67 rrd (hl)
	jp	z80_in_l_c      ; ED 68 in l, (bc)
	jp	z80_out_c_l     ; ED 69 out (bc), l
	jp	z80_adchlhl	; ED 6A adc hl, hl
	jp	z80_nop		; ED 6B ld hl, (nn)
	jp	z80_nop		; ED 6C mlt hl
	jp	z80_nop		; ED 6D ld mb, a
	jp	z80_nop		; ED 6E ld a, mb
	jp	z80_rld_hl_	; ED 6F rld (hl)
	jp	z80_nop		; ED 70
	jp	z80_nop		; ED 71
	jp	z80_sbchlsp   ; ED 72 sbc hl, sp
	jp	z80_ld_nn_sp    ; ED 73 ld (nn), sp
	jp	z80_nop		; ED 74 tstio n
	jp	z80_nop		; ED 75
	jp	z80_nop		; ED 76 slp
	jp	z80_nop		; ED 77
	jp	z80_in_a_c      ; ED 78 in a, (bc)
	jp	z80_out_c_a     ; ED 79 out (bc), a
	jp	z80_adchlsp   ; ED 7A adc hl, sp
	jp	z80_ldsp_nn_    ; ED 7B ld sp, (nn)
	jp	z80_nop		; ED 7C mlt sp
	jp	z80_nop		; ED 7D stmix
	jp	z80_nop		; ED 7E rsmix
	jp	z80_nop		; ED 7F
	jp	z80_nop		; ED 80
	jp	z80_nop		; ED 81
	jp	z80_nop		; ED 82 inim
	jp	z80_nop		; ED 83 otim
	jp	z80_nop		; ED 84 ini2
	jp	z80_nop		; ED 85
	jp	z80_nop		; ED 86
	jp	z80_nop		; ED 87
	jp	z80_nop		; ED 88
	jp	z80_nop		; ED 89
	jp	z80_nop		; ED 8A indm
	jp	z80_nop		; ED 8B otdm
	jp	z80_nop		; ED 8C ind2
	jp	z80_nop		; ED 8D
	jp	z80_nop		; ED 8E
	jp	z80_nop		; ED 8F
	jp	z80_nop		; ED 90
	jp	z80_nop		; ED 91
	jp	z80_nop		; ED 92 inimr
	jp	z80_nop		; ED 93 otimr
	jp	z80_nop		; ED 94 ini2r
	jp	z80_nop		; ED 95
	jp	z80_nop		; ED 96
	jp	z80_nop		; ED 97
	jp	z80_nop		; ED 98
	jp	z80_nop		; ED 99
	jp	z80_nop		; ED 9A indmr
	jp	z80_nop		; ED 9B otdmr
	jp	z80_nop		; ED 9C ind2r
	jp	z80_nop		; ED 9D
	jp	z80_nop		; ED 9E
	jp	z80_nop		; ED 9F
	jp	z80_ldi		; ED A0 ldi
	jp	z80_cpi		; ED A1 cpi
	jp	z80_ini		; ED A2 ini
	jp	z80_outi        ; ED A3 outi
	jp	z80_nop		; ED A4 outi2
	jp	z80_nop		; ED A5
	jp	z80_nop		; ED A6
	jp	z80_nop		; ED A7
	jp	z80_ldd		; ED A8 ldd
	jp	z80_cpd		; ED A9 cpd
	jp	z80_ind		; ED AA ind
	jp	z80_outd        ; ED AB outd
	jp	z80_nop		; ED AC outd2
	jp	z80_nop		; ED AD
	jp	z80_nop		; ED AE
	jp	z80_nop		; ED AF
	jp	z80_ldir        ; ED B0 ldir
	jp	z80_cpir        ; ED B1 cpir
	jp	z80_inir        ; ED B2 inir
	jp	z80_otir        ; ED B3 otir
	jp	z80_nop		; ED B4 oti2r
	jp	z80_nop		; ED B5
	jp	z80_nop		; ED B6
	jp	z80_nop		; ED B7
	jp	z80_lddr        ; ED B8 lddr
	jp	z80_cpdr        ; ED B9 cpdr
	jp	z80_indr        ; ED BA indr
	jp	z80_otdr        ; ED BB otdr
	jp	z80_nop		; ED BC otd2r
	jp	z80_nop		; ED BD
	jp	z80_nop		; ED BE
	jp	z80_nop		; ED BF
	jp	z80_nop		; ED C0
	jp	z80_nop		; ED C1
	jp	z80_nop		; ED C2 inirx
	jp	z80_nop		; ED C3 otirx
	jp	z80_nop		; ED C4
	jp	z80_nop		; ED C5
	jp	z80_nop		; ED C6
	jp	z80_nop		; ED C7 ld i, hl
	jp	z80_nop		; ED C8
	jp	z80_nop		; ED C9
	jp	z80_nop		; ED CA indrx
	jp	z80_nop		; ED CB otdrx
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

	; $ED $38 in0 a, (n)
z80_in0a_n_:
	ld	b, 0
	ld.s	c, (iy)
	inc	iy
	ex	af, af'
	in	a, (bc)
	ex	af, af'
	z80loop

	; $ED $39 out (n), a
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
	inc	iy
	inc	iy
	exx
	push	bc
	exx
	pop	bc
	ld.s	(hl), bc
	z80loop

z80_neg:
	call	not_implemented
	jp	z80_nop

z80_retn:
	pop.s	iy
	z80loop

z80_im0:
	call	not_implemented
	jp	z80_nop

z80_ld_i_a:
	call	not_implemented
	jp	z80_nop

z80_in_c_c:
	call	not_implemented
	jp	z80_nop

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
	inc	iy
	inc	iy
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


z80_ld_r_a:
	call	not_implemented
	jp	z80_nop

z80_in_d_c:
	call	not_implemented
	jp	z80_nop

	; $ED 59 out (c), d
	z80_out_c_jj	d

	; $ED 52 add hl, de
	z80_exall2	sbchlde, {sbc.s hl, de}

	; $ED 53 ld (nn), de
z80_ld_nn_de:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	exx
	push	de
	exx
	pop	bc
	ld.s	(hl), bc
	z80loop

z80_im1:
	z80loop

	; $ED 57 ld a, i
	z80_exaf2	ldai, {ld a, i}

z80_in_e_c:
	call	not_implemented
	jp	z80_nop

	; $ED 59 out (c), e
	z80_out_c_jj	e

	; $ED 5A adc hl, de
	z80_exall2	adchlde, {adc.s hl, de}

	; $ED 5B ld de, (nn)
z80_ld_de_nn:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld.s	hl, (hl)
	push	hl
	exx
	pop	de
	exx
	z80loop

z80_im2:
	call	not_implemented
	jp	z80_nop

z80_ld_a_r:
	call	not_implemented
	jp	z80_nop

z80_in_h_c:
	call	not_implemented
	jp	z80_nop

	; $ED 61 out (c), h
	z80_out_c_jj	h

	; $ED 62 add hl, hl
	z80_exall2	sbchlhl, {sbc.s hl, hl}

	; $ED 67 rrd (hl)
	z80_exall2	rrd_hl_, {db %52}, {rrd (hl)}

z80_in_l_c:
	call	not_implemented
	jp	z80_nop

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
	inc	iy
	inc	iy
	ex	de, hl
	ld.s	(hl), de
	z80loop

z80_in_a_c:
	call	not_implemented
	jp	z80_nop

	; $ED 79 out (c), a
z80_out_c_a:
	exx
	ex	af, af'
	push	bc
	ld	b, IO_SEGMENT
	out	(bc), a
	pop	bc
	ex	af, af'
	exx
	z80loop

	; $ED $7A adc hl, sp
	z80_exall2	adchlsp, {adc.s hl, sp}

	; $ED 7B ld sp, (nn)
z80_ldsp_nn_:
	ld.s	hl, (iy)
	inc	iy
	inc	iy
	ld.s	hl, (hl)
	ld.s	sp, hl
	jp	z80_nop

z80_ldi:
	call	not_implemented
	jp	z80_nop

	; $ED A1
	z80_exall2	cpi, {cpi.s}

z80_ini:
	call	not_implemented
	jp	z80_nop

z80_outi:
	call	not_implemented
	jp	z80_nop

z80_ldd:
	call	not_implemented
	jp	z80_nop

	; $ED A9 cpd
	z80_exall2	cpd, {cpd.s}


z80_ind:
	call	not_implemented
	jp	z80_nop


z80_outd:
	call	not_implemented
	jp	z80_nop

	; $ED $B0
	z80_exall2	ldir, {ldir.s}

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
	in	a, (bc)
	pop	bc
	ld.s	(hl),a
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
	ld.s	a, (hl)
	inc	hl
	push	bc
	ld	b, IO_SEGMENT
	out	(bc), a
	pop	bc
	RST.L	%18	; this delay is too much!
	ex	af, af'
	djnz	otir1
	ex	af, af'
	exx
	z80loop

	; $ED d8 lddr
	z80_exall2	lddr, {lddr.s}

	; $ED B9 cpdr
	z80_exall2	cpdr, {cpdr.s}

z80_indr:
	call	not_implemented
	jp	z80_nop

z80_otdr:
	call	not_implemented
	jp	z80_nop



	global	z80_marshall_isr
z80_marshall_isr:
	push	af
	ld	a, (_z80_flags)
	set	1, a
	ld	(_z80_flags), a
	pop	af

	RET.L			; WE SHOULD BE RETURNING INTO ADL MODE

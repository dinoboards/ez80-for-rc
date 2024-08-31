
; RST.L $10
#DEFINE RST_L_10	DB	$49, $D7

	SECTION code_compiler

;
; extern void i2c_init(const uint8_t t, i2c_clk_divider*) __sdcccall(1);
; 	A = t
;	DE = i2c_clk_divider*
;
	PUBLIC	_i2c_init
_i2c_init:
	PUSH	IX
	PUSH	DE
	LD	C, A
	LD	A, 4
	LD	B, 0
	RST_L_10
	POP	IX
	LD	(IX+0), C	; I2C clock divider scalar value
	LD	(IX+1), B	; I2C clock divider exponent
	LD	(IX+2), L	; Actual Frequency (32 bit, E=0)
	LD	(IX+3), H
	LD	(IX+4), E
	LD	(IX+5), D
	POP	IX
	RET

	PUBLIC	_i2c_write
_i2c_write:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	E, (IX+4)	; address
	LD	D, 0
	LD	C, (IX+5)	; length
	LD	HL, (IX+6)	; data  16bit address within the MBASE segment

	LD	A, 0		; Convert HL to MB:HL
	LD	B, 8
	RST_L_10

	LD	A, 4
	LD	B, 1
	RST_L_10
	LD	L, A

	LD	SP, IX
	POP	IX
	RET

	PUBLIC	_i2c_probe
_i2c_probe:
	LD	E, L
	LD	A, 4
	LD	B, 3
	RST_L_10
	LD	L, A
	RET


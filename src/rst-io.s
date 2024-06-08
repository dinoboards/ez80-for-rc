
	SECTION CODE

	.assume adl=1

INSTR_OUT_NN_A	EQU	%D3
INSTR_INOUT_R_C	EQU	%ED

	PUBLIC	_rst_io
_rst_io:
	PUSH	IX
	LD	IX, 3
	ADD	IX, SP

	PUSH	IY
	PUSH	HL
	PUSH	BC
	PUSH	AF

	;LD	A, (IX+0) 			; SHOULD BE 2H FOR Z80 MODE
	LD	A, (IX+1)
ifdef _SIMULATE
	LD	IY, %B70000
endif
	LD	IYL, A
	LD	A, (IX+2)
	LD	IYH, A				; IY SHOULD BE RETURN ADDRESS, THE ADDRESS OF THE NEXT INSTR.
ifdef _SIMULATE
	LD 	A, (IY)  			; IS THIS THE FIST BYTE AFTER THE RST.L 8 INSTRUCTION.
else
	LD.S	A, (IY)
endif

	CP	INSTR_OUT_NN_A
	JR	Z, out_nn_a_hook

	CP	INSTR_INOUT_R_C
	JR	Z, inout_r_c_hook

rst_io_resume:
	POP	AF
rst_io_resume2:
	POP	BC
	POP	HL
	POP	IY
	POP	IX
	RET.L

out_nn_a_hook:
	LD.S	C, (IY+1)			; THE REQUEST PORT NUMBER
	LD	B, %FF

	POP	AF				; RETIEVE VALUE OF A BEFORE HOOK WAS INVOKED
	PUSH	AF
	; LD	A, (IX+5)			; OR GET IT VIA THE STACK VALUE

	OUT	(C), A

	INC	IY
	INC	IY

	LD	A, IYL
	LD	(IX+1), A			; UPDATE THE RETURN ADDRESS TO SKIP INTERPRETED
	LD	A, IYH				; INSTRUCTION
	LD	(IX+2), A

	jr	rst_io_resume

inout_r_c_hook:
	INC	IY				; SET RETURN ADDRESS TO SKIP 2 BYTES
	INC	IY
	LD	A, IYL
	LD	(IX+1), A			; UPDATE THE RETURN ADDRESS TO SKIP INTERPRETED
	LD	A, IYH				; INSTRUCTION
	LD	(IX+2), A

ifdef _SIMULATE
	LD	A, (IY+(1-2))			; THE REGISTER TO BE UPDATED
else
	LD.S	A, (IY+(1-2))			; THE REGISTER TO BE UPDATED
endif
	LD	B, %FF

	CP	%78				; IN A, (C)
	JR	Z, in_a_c_hook

	cp	%79				; A
	JR	Z, out_a_c_hook

	CP	%40				; B
	JR	Z, in_b_c_hook

	CP	%48				; C
	JR	Z, in_c_c_hook

	CP	%50				; D
	JR	Z, in_d_c_hook

	CP	%58				; E
	JR	Z, in_e_c_hook

	CP	%60				; H
	JR	Z, in_h_c_hook

	CP	%68				; L
	JR	Z, in_l_c_hook

	NOP
	RST.L	00H				; ILLEGAL

in_a_c_hook:
	IN	A, (C)

	INC	SP				; REMOVE PUSHED AF FROM STACK FRAME
	INC	SP
	INC	SP

	jr	rst_io_resume2

out_a_c_hook:
	LD	A, (IX-14)
	OUT	(C), A
	jr	rst_io_resume

in_b_c_hook:
	IN	B, (C)
	LD	(IX-11), B
	jr	rst_io_resume

in_c_c_hook:
in_d_c_hook:
in_e_c_hook:
in_h_c_hook:
in_l_c_hook:
	NOP
	RST.L	00H				; ILLEGAL



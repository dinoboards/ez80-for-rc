;
; RST_IO - RST 8 INSTRUCTION HANDLER
; interpret the expected IN/OUT insturction proceeding the RST 8
; ensuring that the I/O address is set to range of 0xFFxx

	SECTION CODE

	.assume adl=1

INSTR_OUT_NN_A	EQU	%D3		; OUT (nn), A
INSTR_IN_A_NN	EQU	%DB		; IN A, (nn)
INSTR_INOUT_R_C	EQU	%ED		; IN r, (BC) & OUT (BC), r

	PUBLIC	_rst_io
_rst_io:
	PUSH	IX
	LD	IX, 3
	ADD	IX, SP

	PUSH	IY
	PUSH	BC				; B = IX-8, C = IX-9
	PUSH	AF				; A = IX-11


ifdef _SIMULATE
	LD	IY, %B70000			; SIMULATOR HAS FAULT AND DOES
						; NOT SIMULATE LD.S A, (IY) CORRECTLY
endif
						; (IX+0) => SHOULD BE 02H FOR Z80 MODE
	LD	A, (IX+1)			; (IX+1) =>
	LD	IYL, A
	LD	A, (IX+2)
	LD	IYH, A				; IY SHOULD BE RETURN ADDRESS, THE ADDRESS OF THE NEXT INSTR.

ifdef _SIMULATE
	LD 	A, (IY)
else
	LD.S	A, (IY)				; MBASE WITH LD.S A, (IY)
						; OP-CODE OF I/O OPERATION, AFTER THE RST.L 8
endif

	CP	INSTR_OUT_NN_A
	JR	Z, out_nn_a_hook

	CP	INSTR_IN_A_NN
	JR	Z, in_a_nn_hook

	CP	INSTR_INOUT_R_C
	JR	Z, inout_r_c_hook

rst_io_resume:
	POP	AF
rst_io_resume2:
	POP	BC
	POP	IY
	POP	IX
	RET.L

out_nn_a_hook:
	LD.S	C, (IY+1)			; THE REQUEST PORT NUMBER
	LD	B, %FF

	LD	A, (IX-11)			; RETRIEVE ORIGINAL A VALUE

	OUT	(BC), A

	INC	IY
	INC	IY

	LD	A, IYL
	LD	(IX+1), A			; UPDATE THE RETURN ADDRESS TO SKIP INTERPRETED
	LD	A, IYH				; INSTRUCTION
	LD	(IX+2), A

	JR	rst_io_resume


in_a_nn_hook:
	LD.S	C, (IY+1)			; THE REQUEST PORT NUMBER
	LD	B, %FF

	INC	IY
	INC	IY

	LD	A, IYL
	LD	(IX+1), A			; UPDATE THE RETURN ADDRESS TO SKIP INTERPRETED
	LD	A, IYH				; INSTRUCTION
	LD	(IX+2), A

	IN	A, (BC)

	INC	SP				; REMOVE PUSHED AF FROM STACK FRAME
	INC	SP
	INC	SP

	JR	rst_io_resume2

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

	CP	%78				; IN A, (BC)
	JR	Z, in_a_c_hook

	cp	%79				; OUT (BC), A
	JR	Z, out_a_c_hook

	CP	%40				; IN B, (BC)
	JR	Z, in_b_c_hook

	CP	%41				; OUT (BC), B
	JR	Z, out_b_c_hook

	CP	%48				; IN C, (BC)
	JR	Z, in_c_c_hook

	CP	%49				; OUT (BC), C
	JR	Z, out_c_c_hook

	CP	%50				; IN D, (BC)
	JR	Z, in_d_c_hook

	CP	%51				; OUT (BC), D
	JR	Z, out_d_c_hook

	CP	%58				; IN E, (BC)
	JR	Z, in_e_c_hook

	CP	%59				; OUT (BC), E
	JR	Z, out_e_c_hook

	CP	%60				; IN H, (BC)
	JR	Z, in_h_c_hook

	CP	%61				; OUT (BC), H
	JR	Z, out_h_c_hook

	CP	%68				; IN L, (BC)
	JR	Z, in_l_c_hook

	CP	%69				; OUT (BC), L
	JR	Z, out_l_c_hook

	NOP
	RST.L	00H				; ILLEGAL

in_a_c_hook:
	IN	A, (C)

	INC	SP				; REMOVE PUSHED AF FROM STACK FRAME
	INC	SP
	INC	SP

	JR	rst_io_resume2

out_a_c_hook:
	LD	A, (IX-11)
	OUT	(BC), A
	JR	rst_io_resume

in_b_c_hook:
	IN	B, (C)
	LD	(IX-8), B
	JR	rst_io_resume

out_b_c_hook:
	LD	A, (IX-8)			; B's VALUE FROM STACK
	OUT	(BC), A
	JR	rst_io_resume

in_c_c_hook:
	IN	A, (BC)
	LD	(IX-9), A
	JR	rst_io_resume

out_c_c_hook:
	OUT	(BC), C
	JR	rst_io_resume

in_d_c_hook:
	IN	D, (BC)
	JR	rst_io_resume

out_d_c_hook:
	OUT	(BC), D
	JR	rst_io_resume

in_e_c_hook:
	IN	E, (BC)
	JR	rst_io_resume

out_e_c_hook:
	OUT	(BC), E
	JR	rst_io_resume

in_h_c_hook:
	IN	H, (BC)
	JR	rst_io_resume

out_h_c_hook:
	OUT	(BC), H
	JR	rst_io_resume

in_l_c_hook:
	IN	L, (BC)
	JR	rst_io_resume

out_l_c_hook:
	OUT	(BC), L
	JR	rst_io_resume

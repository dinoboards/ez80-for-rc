
CH_CMND: MACRO CMD
	ld	b, %ff

$$S1:
	ld	a, +(_CH376_COMMAND_PORT / 256)
	in	a, (_CH376_COMMAND_PORT & %FF)
	bit	4, a
	jr	nz, $$S3
$$S2:
	ld	bc, _CH376_COMMAND_PORT
	ld	a, CMD
	out	(bc), a
	jr	$$S4

$$S3:
	djnz	$$S1
	jr	$$S2
$$S4:
	ENDMAC	CH_CMND

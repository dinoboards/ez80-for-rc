
	.assume	adl = 1

	section	.text,"ax",@progbits

	.global	_get_random_seed

_get_random_seed:
	LD	A, R
	LD	L, A
	RET

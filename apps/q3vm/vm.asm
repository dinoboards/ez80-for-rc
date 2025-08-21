	.assume	adl=1

	section	.text, "ax", @progbits

	.global	_VM_Create
_VM_Create:
	xor	a
	ld	b, 20
	JP	$000010

	.global	_VM_Call
_VM_Call:
	xor	a
	ld	b, 21
	JP	$000010

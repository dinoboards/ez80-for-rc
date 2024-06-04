
DEFINE JUMPTBL,SPACE=ROM
SEGMENT JUMPTBL
	.assume adl=1

	org	1FF00h
	extern 	uart_preinit
	extern 	uart_init

JT_UART_PREINIT:
	JP	uart_preinit

JT_UART_INIT:
	JP	uart_init


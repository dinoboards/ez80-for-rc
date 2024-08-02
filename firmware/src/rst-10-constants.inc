;
; Constants for the RST-10 firmware functions
;
ERR_NOHW	.EQU	-8				; HARDWARE NOT PRESENT
UART_ERR_RECEIVEFIFOEMPTY	.EQU	%0F		; The error code returned when the software receive FIFO buffer is empty.

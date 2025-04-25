DELAY_FACTOR		.EQU	640

CMD01_RD_USB_DATA0	.EQU	%27		; Read data block from current USB interrupt endpoint buffer or host endpoint receive buffer
						; output: length, data stream

CMD10_WR_HOST_DATA	.EQU	%2C 		; Write a data block to the send buffer of the USB host endpoint
						; input: length, data stream

CMD20_WRITE_VAR8	.EQU	%0B		; Set the specified 8-bit file system variable

CH_CMD_RD_USB_DATA0	.EQU	CMD01_RD_USB_DATA0
CH_CMD_WR_HOST_DATA	.EQU	CMD10_WR_HOST_DATA
CH_CMD_WRITE_VAR8    	.EQU	CMD20_WRITE_VAR8

CH_VAR_RETRY_TIMES	.EQU	%25		; The number of retries for the USB transaction operation
CH_NAK_RETRY_DONT 	.EQU	0
CH_NAK_RETRY_INDEFINITE	.EQU	2
CH_NAK_RETRY_3S		.EQU	3

_CH376_DATA_PORT	.EQU	%ff88
_CH376_COMMAND_PORT	.EQU	%ff89
_USB_MODULE_LEDS	.EQU	%ff8a

USB_IS_KEYBOARD		.EQU	4

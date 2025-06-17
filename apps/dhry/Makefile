all: DHRYEZ80.COM DHRYHB.COM DHRYMSX.COM

include ../common.mk

ZCC_EXTRA = --vc -clib=sdcc_iy -pragma-include:zpragma.inc

# z80: cpm with hbios for timing
$(BIN)DHRYHB.COM: dhry_1.c dhry_2.c $(HBIOS_LIB) | $(BIN)
	$(compile-cpm-hbios)

# z80: msx - using msx jiffy timer
$(BIN)DHRYMSX.COM: dhry_1.c dhry_2.c jiffy.c | $(BIN)
	$(compile-msxdos2)

# ez80: cpm with ez80-firmware
$(BIN)DHRYEZ80.COM: | $(BIN)
	@ez80-cc --optimise size --output-dir ../bin DHRYEZ80.COM dhry_1.c dhry_2.c nanoprintf.c

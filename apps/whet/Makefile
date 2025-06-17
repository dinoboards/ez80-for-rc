all: WHETEZ80.COM WHETHB.COM WHETMSX.COM

include ../common.mk
ZCC_EXTRA += -clib=sdcc_iy -pragma-include:zpragma.inc

# z80: cpm with hbios for timing
$(BIN)WHETHB.COM: whetstone.c $(HBIOS_LIB) | $(BIN)
	$(compile-cpm-hbios)

# z80: msx - using msx jiffy timer
$(BIN)WHETMSX.COM: whetstone.c | $(BIN)
	$(compile-msxdos2)

# ez80: cpm with ez80-firmware
$(BIN)WHETEZ80.COM: | $(BIN)
	@ez80-cc --optimise size --output-dir ../bin WHETEZ80.COM whetstone.c nanoprintf.c

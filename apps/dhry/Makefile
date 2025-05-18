APPS := DHRY

all: DHRY.EXE DHRY.COM
include ../common.mk

ZCC_EXTRA := -clib=sdcc_iy -pragma-include:zpragma.inc

$(BIN)$(APPS).COM: $(BIN)$(APPS)/dhry_1.c.o $(BIN)$(APPS)/dhry_2.c.o dhry.h $(HBIOS_LIB)

.PHONY: DHRY.EXE
DHRY.EXE:
	@ez80-cc --optimise size --mem-model 2MB --output-dir ../bin DHRY.EXE dhry_1.c dhry_2.c nanoprintf.c

APPS := DHRY

include ../common.mk

ZCC_EXTRA := -clib=sdcc_iy -pragma-include:zpragma.inc

$(BIN)$(APPS).COM: $(BIN)$(APPS)/dhry_1.c.o $(BIN)$(APPS)/dhry_2.c.o dhry.h $(HBIOS_LIB)

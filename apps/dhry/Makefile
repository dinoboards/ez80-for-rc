APPS := dhry

include ../common.mk

ZCC_EXTRA := -clib=sdcc_iy -SO3 --max-allocs-per-node200000 -pragma-include:zpragma.inc

$(BIN)$(APPS).com: $(BIN)$(APPS)/dhry_1.c.o $(BIN)$(APPS)/dhry_2.c.o dhry.h $(HBIOS_LIB)



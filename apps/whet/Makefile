APPS := WHET

all: WHET.EXE WHET.COM

include ../common.mk

ZCC_EXTRA := -clib=sdcc_iy -SO3 --max-allocs-per-node200000 -pragma-include:zpragma.inc

$(BIN)$(APPS).COM: $(BIN)$(APPS)/whetstone.c.o $(HBIOS_LIB)


.PHONY: WHET.EXE
WHET.EXE:
	@ez80-cc --optimise size --mem-model 2MB --output-dir ../bin WHET.EXE whetstone.c nanoprintf.c

APPS := whet

include ../common.mk

ZCC_EXTRA := -clib=sdcc_iy -SO3 --max-allocs-per-node200000 -pragma-include:zpragma.inc

$(BIN)$(APPS).com: $(BIN)$(APPS)/whetstone.c.o $(BIN)common/hbios_sysget_tick.lib



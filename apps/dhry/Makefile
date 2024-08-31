APPS := dhry

include ../common.mk

ZCC := zcc +cpm -clib=sdcc_iy -SO3 --max-allocs-per-node200000 -lm -DPRINTF -m -pragma-include:zpragma.inc -I../common  -create-app

$(BIN)dhry.com: dhry_1.c dhry_2.c dhry.h ../common/hbios_sysget_tick.asm



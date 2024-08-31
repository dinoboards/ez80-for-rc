APPS := whet

include ../common.mk

ZCC := zcc +cpm -clib=sdcc_iy -SO3 --max-allocs-per-node200000 -lm -DCOMMAND -DSTATIC -DTIMER  -DTIMEFUNC -m -pragma-include:zpragma.inc -I../common  -create-app

$(BIN)whet.com: whetstone.c ../common/hbios_sysget_tick.asm



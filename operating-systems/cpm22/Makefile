SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

all: debug/cpm22img.sys cbios.s

ccp.bin: ../tools/zxcc/zxcc CCP.ASM
	@./build.sh ccp

bdos.bin: ../tools/zxcc/zxcc BDOS.ASM
	@./build.sh bdos

.PHONY: debug/cpm22-cbios.hex
debug/cpm22-cbios.hex:
	@wine make-debug.bat

debug/cbios.o: cbios.asm
	@mkdir -p debug
	@ez80-as -march=ez80+full cbios.asm -o debug/cbios.o -al="debug/cbios.lst"

cbios.bin: debug/cbios.o
	@ez80-ld -T cbios.ld debug/cbios.o --oformat binary -o "cbios.bin" --Map "debug/cbios.map"

.PHONY: cbios.s
cbios.s: cbios.bin
	@./bin-to-zds.sh cbios
	@mv cbios.s ../../firmware/src/fbios/

debug/loader.o: loader.asm
	@mkdir -p debug
	@ez80-as -march=ez80+full loader.asm -o debug/loader.o

loader.bin: debug/loader.o
	@ez80-ld -T loader.ld debug/loader.o --oformat binary -o "loader.bin" --Map "debug/loader.map"

../tools/zxcc/zxcc:
	@make -C ../tools -s

debug/cpm22img.sys: loader.bin ccp.bin bdos.bin cbios.bin
	@cat loader.bin ccp.bin bdos.bin cbios.bin > debug/cpm22img.sys

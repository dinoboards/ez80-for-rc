MAKEFLAGS += --no-builtin-rules -B
SHELL=/bin/bash

all: Z80ALL.COM zexall.asm

Z80ALL.COM:
	@mkdir -p bin
	@ez80-as -march=ez80+full --defsym BUILD_TYPE_CPM=1 -a=../bin/z80all.lst ./zexall.asm -o ./bin/z80all.o
	@ez80-ld -T ./linker.ld --relax -O1 --strip-discarded --orphan-handling=error  "./bin/z80all.o"  --oformat binary -o "../bin/Z80ALL.COM"
	@echo "Built Z80ALL.COM"


# this target is for including within the firmware to test for Z80 emulation compliance
zexall.asm:
	@mkdir -p bin
	@ez80-as -march=ez80+full --defsym BUILD_TYPE_EZ80EMBEDDED=1 -a=../bin/ez80all.lst ./zexall.asm -o ./bin/ez80all.o
	@ez80-ld -T ./linker.ld --relax -O1 --strip-discarded --orphan-handling=error  "./bin/ez80all.o"  --oformat binary -o "./bin/z80all.bin"
	@./export_to_dbs.sh
	@echo "Built zexall.asm"


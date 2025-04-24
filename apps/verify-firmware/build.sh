#!/usr/bin/env bash

set -e

echo "Compiling hello-world.c -> hello-world.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S hello-world.c -c -o hello-world.s

echo "Compiling hello-world.s -> hello-world.o"
ez80-as -march=ez80+full -a=./hello-world.lst ./hello-world.s -o ./hello-world.o

ez80-ld -T /opt/ez80-clang/linker-scripts/cpm-2mb.ld --relax -O1 --strip-discarded -Map=./hladl.map --orphan-handling=error -L/opt/ez80-clang/lib/ --whole-archive -lcrtexe --no-whole-archive -lcrt hello-world.o  --start-group -llibc  -lcrt -lcpm -lv99x8-hdmi -lez80 -lmalloc -lio -lez80rc --end-group --oformat binary -o ../bin/HWADL.EXE

#!/usr/bin/env bash

source ~/.ez80-clang

SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=$(cd "${SCRIPT_DIR}/" && pwd)/../../clang-tool-chain/

set -e

echo "Compiling main.c -> main.s"
ez80-clang -DEXE=1 ${INCLUDE_PATHS}  -DEZ80CLANG=1  -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S main.c -c -o main.s

echo "Compiling main.s -> main.o"
ez80-as  -march=ez80+full -a=./main.lst ./main.s -o ./main.o

echo "Compiling test_getopt.c -> test_getopt.s"
ez80-clang -DEXE=1 ${INCLUDE_PATHS}  -DEZ80CLANG=1  -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S test_getopt.c -c -o test_getopt.s

echo "Compiling test_getopt.s -> test_getopt.o"
ez80-as  -march=ez80+full -a=./test_getopt.c.lst ./test_getopt.s -o ./test_getopt.o

ez80-ld -T /opt/ez80-clang/linker-scripts/cpm-64k.ld -Map test.map --relax -O1 --strip-discarded --orphan-handling=error -L/opt/ez80-clang/lib/ -lcrt main.o test_getopt.o -lmalloc --start-group -llibc -lcrt -lcpm -lv99x8-standard -lez80 --end-group --oformat binary -o ../bin/TEST.COM

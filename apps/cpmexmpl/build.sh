#!/usr/bin/env bash

source ~/.ez80-clang

SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=$(cd "${SCRIPT_DIR}/" && pwd)/../../clang-tool-chain/

set -e

echo "Compiling main.c -> main.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S main.c -c -o main.s

echo "Compiling main.s -> main.o"
ez80-as -march=ez80+full -a=./main.lst ./main.s -o ./main.o

# ez80-ld -T /opt/ez80-clang/linker-scripts/cpm-64k.ld --relax -O1 --strip-discarded --orphan-handling=error -L/opt/ez80-clang/lib/ -lcrt main.o --start-group -llibc -lcrt -lcpm -lv99x8-standard -lez80 --end-group --oformat binary -o ../bin/TEST.COM

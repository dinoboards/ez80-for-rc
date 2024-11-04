#!/usr/bin/env bash

source ~/.ez80-clang

SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=$(cd "${SCRIPT_DIR}/" && pwd)/../../clang-tool-chain/

# export EZ80_CLANG_SYSTEM_INCLUDE_PATH=${SCRIPT_DIR}/include
# export EZ80_CLANG_LIB_PATH=${SCRIPT_DIR}/lib/
# export EZ80_CLANG_LS_PATH=${SCRIPT_DIR}/linker-scripts/

# export INCLUDE_PATHS="-isystem ${SCRIPT_DIR}/include -isystem ${SCRIPT_DIR}/working/llvm-project/build/lib/clang/15.0.0/include/"
# PATH="${SCRIPT_DIR}working/llvm-project/build/bin:${SCRIPT_DIR}working/opt/ez80-none-elf/bin/:${PATH}"

# shopt -s expand_aliases
# alias ez80-clang="clang -nostdinc"
# alias ez80-as="ez80-none-elf-as"
# alias ez80-ld="ez80-none-elf-ld"


set -e

echo "Compiling cube.c -> cube.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S cube.c -c -o cube.s

echo "Compiling cube.s -> cube.o"
ez80-as -march=ez80+full -a=./cube.lst ./cube.s -o ./cube.o

ez80-ld -T /opt/ez80-clang/linker-scripts/cpm-64k.ld --relax -O1 --strip-discarded --orphan-handling=error -L/opt/ez80-clang/lib/ -lcrt cube.o --start-group -llibc -lcrt -lcpm -lv99x8-standard -lez80 --end-group --oformat binary -o ../bin/CUBE.COM

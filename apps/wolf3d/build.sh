#!/usr/bin/env bash

set -e

echo "Compiling id_vh.c -> id_vh.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_vh.c -c -o id_vh.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S id_vh.c -c -o id_vh.s

echo "Compiling id_vh.s -> id_vh.o"
ez80-as -march=ez80+full -a=./id_vh.lst ./id_vh.s -o ./id_vh.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 id_vh.c -c -o /tmp/tmp.Q98vFkNTp0/id_vh.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_vh.c -c -o id_vh.s

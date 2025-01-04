#!/usr/bin/env bash

set -e

echo "Compiling id_vl.c -> id_vl.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_vl.c -c -o id_vl.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S id_vl.c -c -o id_vl.s

echo "Compiling id_vl.s -> id_vl.o"
ez80-as -march=ez80+full -a=./id_vl.lst ./id_vl.s -o ./id_vl.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 id_vl.c -c -o /tmp/tmp.Q98vFkNTp0/id_vl.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_vl.c -c -o id_vl.s

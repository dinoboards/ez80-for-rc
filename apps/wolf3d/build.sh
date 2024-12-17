#!/usr/bin/env bash

set -e

echo "Compiling id_ca.c -> id_ca.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_ca.c -c -o id_ca.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S id_ca.c -c -o id_ca.s

echo "Compiling id_ca.s -> id_ca.o"
ez80-as -march=ez80+full -a=./id_ca.lst ./id_ca.s -o ./id_ca.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 id_ca.c -c -o /tmp/tmp.Q98vFkNTp0/id_ca.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_ca.c -c -o id_ca.s

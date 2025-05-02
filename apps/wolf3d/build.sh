#!/usr/bin/env bash

set -e

echo "Compiling id_in.c -> id_in.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_in.c -c -o id_in.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S id_in.c -c -o id_in.s

# echo "Compiling id_in.s -> id_in.o"
# ez80-as -march=ez80+full -a=./id_in.lst ./id_in.s -o ./id_in.o

# # ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 id_in.c -c -o /tmp/tmp.Q98vFkNTp0/id_in.c.
# # ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_in.c -c -o id_in.s

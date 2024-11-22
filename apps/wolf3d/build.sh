#!/usr/bin/env bash

set -e

echo "Compiling id_us_1.c -> id_us_1.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_us_1.c -c -o id_us_1.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 -S id_us_1.c -c -o id_us_1.s

echo "Compiling id_us_1.s -> id_us_1.o"
ez80-as -march=ez80+full -a=./id_us_1.lst ./id_us_1.s -o ./id_us_1.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 id_us_1.c -c -o /tmp/tmp.Q98vFkNTp0/id_us_1.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_us_1.c -c -o id_us_1.s

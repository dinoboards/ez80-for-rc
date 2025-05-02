#!/usr/bin/env bash

set -e

echo "Compiling wl_menu.c -> wl_menu.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_menu.c -c -o wl_menu.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S wl_menu.c -c -o wl_menu.s

# echo "Compiling wl_menu.s -> wl_menu.o"
# ez80-as -march=ez80+full -a=./wl_menu.lst ./wl_menu.s -o ./wl_menu.o

# # ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 wl_menu.c -c -o /tmp/tmp.Q98vFkNTp0/wl_menu.c.
# # ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_menu.c -c -o wl_menu.s

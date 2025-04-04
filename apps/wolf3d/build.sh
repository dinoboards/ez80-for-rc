#!/usr/bin/env bash

set -e

echo "Compiling wl_draw_calc_height.c -> wl_draw_calc_height.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_draw_calc_height.c -c -o wl_draw_calc_height.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S wl_draw_calc_height.c -c -o wl_draw_calc_height.s

echo "Compiling wl_draw.s -> wl_draw.o"
ez80-as -march=ez80+full -a=./wl_draw.lst ./wl_draw.s -o ./wl_draw.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 wl_draw.c -c -o /tmp/tmp.Q98vFkNTp0/wl_draw.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_draw.c -c -o wl_draw.s

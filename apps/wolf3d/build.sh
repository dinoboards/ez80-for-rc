#!/usr/bin/env bash

set -e

echo "Compiling wl_draw_scale_post.c -> wl_draw_scale_post.s"
set -x
# ez80-clang ${INCLUDE_PATHS} -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_draw_scale_post.c -c -o wl_draw_scale_post.s
ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S wl_draw_scale_post.c -c -o wl_draw_scale_post.s

echo "Compiling wl_draw_scale_post.s -> wl_draw_scale_post.o"
ez80-as -march=ez80+full -a=./wl_draw_scale_post.lst ./wl_draw_scale_post.s -o ./wl_draw_scale_post.o

# ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O3 wl_draw_scale_post.c -c -o /tmp/tmp.Q98vFkNTp0/wl_draw_scale_post.c.
# ez80-clang -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S wl_draw_scale_post.c -c -o wl_draw_scale_post.s

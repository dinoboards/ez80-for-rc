#!/usr/bin/env bash

set -e

echo "Compiling wl_main.c -> wl_main.s"
set -x

ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S wl_main.c -c -o wl_main.s


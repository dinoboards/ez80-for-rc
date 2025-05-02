#!/usr/bin/env bash

set -e

echo "Compiling ez80.c -> ez80.s"
set -x

ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S ez80.c -c -o ez80.s


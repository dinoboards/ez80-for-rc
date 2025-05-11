#!/usr/bin/env bash

set -e

echo "Compiling id_mm.c -> id_mm.s"
set -x

ez80-clang -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -Oz -S id_mm.c -c -o id_mm.s


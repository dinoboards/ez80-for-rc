#!/usr/bin/env bash

set -e

echo "Compiling exe.c -> exe.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S exe.c -c -o exe.s


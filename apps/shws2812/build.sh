#!/usr/bin/env bash

set -e

echo "Compiling shws2812.c -> shws2812.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S shws2812.c -c -o shws2812.s

SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: WOLF3D.EXE

# Find all .c files in the current directory
SRC_FILES := $(wildcard *.c)

# remove cal_huff_expand.c from the list of files to compile
# and replace with .s
SRC_FILES := $(filter-out cal_huff_expand.c wl_draw_calc_height.c wl_draw_scale_post.c, $(SRC_FILES))
SRC_FILES += cal_huff_expand.asm wl_draw_calc_height.asm fixed_mul.asm wl_draw_scale_post.asm fixed_rounded_down.asm

CFLAGS := -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O2

.PHONY: WOLF3D.EXE
WOLF3D.EXE:
	@ez80-clang ${CFLAGS}  -S wl_draw_scale_post.c -c -o wl_draw_scale_post.asm
	@ez80-clang ${CFLAGS}  -S wl_draw_calc_height.c -c -o wl_draw_calc_height.asm
	@ez80-clang ${CFLAGS}  -S cal_huff_expand.c -c -o cal_huff_expand.asm
	@ez80-cc --mem-model 2MB --lib v99x8-hdmi --output-dir ../bin --optimise size WOLF3D.EXE $(SRC_FILES)

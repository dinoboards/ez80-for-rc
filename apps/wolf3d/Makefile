SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: WOLF3D.EXE WOLFS3D.EXE

# Find all .c files in the current directory
SRC_FILES := $(wildcard *.c)
OPTIMISED_C_FILES := cal_huff_expand.c wl_draw_calc_height.c wl_draw_scale_post.c
OPTIMISED_ASM_FILES := $(OPTIMISED_C_FILES:.c=.asm)
ASM_SRC_FILES := $(filter-out $(OPTIMISED_ASM_FILES), $(wildcard *.asm) $(wildcard v99x8-hdmi/*.asm))

SHIFT_SRC_FILES := $(wildcard shift_functions/*.asm)

# remove cal_huff_expand.c from the list of files to compile
# and replace with .s
SRC_FILES := $(filter-out $(OPTIMISED_C_FILES), $(SRC_FILES))
SRC_FILES += $(OPTIMISED_ASM_FILES) $(SHIFT_SRC_FILES) $(ASM_SRC_FILES)

CFLAGS := -mllvm -z80-print-zero-offset -target ez80-none-elf -nostdinc -isystem /opt/ez80-clang/include/ -Wall -Wextra -Wunreachable-code -Werror -Wa,-march=ez80 -O2

%.asm: %.c FORCE
	@echo "Compiling $< to $@"
	@ez80-clang ${CFLAGS}  -S $< -c -o $@

.PHONY: FORCE
FORCE:

.PHONY: WOLF3D.EXE
WOLF3D.EXE: $(OPTIMISED_ASM_FILES)
	@ez80-cc --defsym VDP_SUPER_HDMI=1 --mem-model 2MB --lib v99x8-hdmi -DVDP_SUPER_HDMI --output-dir ../bin --optimise size WOLF3D.EXE $(SRC_FILES)

.PHONY: WOLFS3D.EXE
WOLFS3D.EXE: $(OPTIMISED_ASM_FILES)
	@ez80-cc --defsym VDP_V9958=1 --mem-model 2MB --lib v99x8-standard -DVDP_V9958 --output-dir ../bin --optimise size WOLFS3D.EXE $(SRC_FILES)

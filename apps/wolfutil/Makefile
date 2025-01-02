SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

SHIFT_SRC_FILES := $(wildcard ../wolf3d/shift_functions/*.asm)

all: WOLFUTIL.EXE

.PHONY: WOLFUTIL.EXE
WOLFUTIL.EXE:
	@ez80-cc --mem-model 2MB --optimise size --lib v99x8-standard --output-dir ../bin WOLFUTIL.EXE wolfutil.c id_mm.c quit.c test_rendering_tiles.c tiles.c palette.c signon.c display_img_file.c create_signon_image.c test_mm_functions.c test_fixed_point.c fixed_mul.asm test_key_serial_to_event.c $(SHIFT_SRC_FILES)

# signon.c
#
# c:exe a:w -S=C:SIGNON.IMG

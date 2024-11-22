SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: WOLFUTIL.EXE

.PHONY: WOLFUTIL.EXE
WOLFUTIL.EXE:
	@ez80-cc --mem-model 2MB --optimise speed --lib v99x8-hdmi --output-dir ../bin WOLFUTIL.EXE wolfutil.c id_mm.c quit.c

# signon.c

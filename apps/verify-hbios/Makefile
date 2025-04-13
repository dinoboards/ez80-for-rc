SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: VHBIOS.COM VHBIOS.EXE

.PHONY: VHBIOS.COM
VHBIOS.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin VHBIOS.COM put-char.asm verify-hbios.c

.PHONY: VHBIOS.EXE
VHBIOS.EXE:
	@mkdir -p ../bin
	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --output-dir ../bin VHBIOS.EXE put-char.asm verify-hbios.c

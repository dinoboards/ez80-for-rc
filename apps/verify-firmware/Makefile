SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: VFIRM.COM VFIRM.EXE

.PHONY: VFIRM.COM
VFIRM.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin VFIRM.COM put-char.asm verify-firmware.c

.PHONY: VFIRM.EXE
VFIRM.EXE:
	@mkdir -p ../bin
	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --output-dir ../bin VFIRM.EXE put-char.asm verify-firmware.c

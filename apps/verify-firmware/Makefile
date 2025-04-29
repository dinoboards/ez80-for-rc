SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: VFIRM.COM

.PHONY: VFIRM.COM
VFIRM.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin VFIRM.COM put-char.asm verify-firmware.c

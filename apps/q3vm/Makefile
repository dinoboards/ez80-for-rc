SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: Q3VM.COM

.PHONY: Q3VM.COM
Q3VM.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise speed --output-dir ../bin Q3VM.COM q3vm.c tests-byte-code.c vm.asm


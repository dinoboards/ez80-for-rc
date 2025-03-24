SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EXE.COM

.PHONY: EXE.COM
EXE.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise speed --output-dir ../bin EXE.COM exe.c marshaller.asm

#TODO: add support to auto discover the available memory, and allocate application appropriately

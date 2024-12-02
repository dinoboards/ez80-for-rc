SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EXE.COM

.PHONY: EXE.COM
EXE.COM:
	@ez80-cc --optimise speed --output-dir ../bin EXE.COM exe.c marshaller.asm

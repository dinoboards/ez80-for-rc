SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EXE.COM

.PHONY: EXE.COM
EXE.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise speed --output-dir ../bin EXE.COM exe.c marshaller.asm discover_installed_ram.c

#TODO: add support to auto discover the available memory, and allocate application appropriately

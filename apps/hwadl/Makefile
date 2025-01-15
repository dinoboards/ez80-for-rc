SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: HWADL.COM HWADL.EXE

.PHONY: HWADL.COM
HWADL.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin HWADL.COM hello-world.c

.PHONY: HWADL.EXE
HWADL.EXE:
	@mkdir -p ../bin
	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --output-dir ../bin HWADL.EXE hello-world.c


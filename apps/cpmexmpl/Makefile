SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: CPMEXMPL.COM CPMEXMPL.EXE

.PHONY: CPMEXMPL.COM
CPMEXMPL.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin --optimise size CPMEXMPL.COM main.c

.PHONY: CPMEXMPL.EXE
CPMEXMPL.EXE:
	@mkdir -p ../bin
	@ez80-cc --mem-model 2MB --output-dir ../bin --optimise size CPMEXMPL.EXE main.c

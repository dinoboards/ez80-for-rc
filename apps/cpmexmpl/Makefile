SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: CPMEXMPL.COM

.PHONY: CPMEXMPL.COM
CPMEXMPL.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin --optimise size CPMEXMPL.COM main.c


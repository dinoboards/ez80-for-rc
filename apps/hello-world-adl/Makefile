SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: HWADL.COM

.PHONY: HWADL.COM
HWADL.COM:
	@ez80-cc --output-dir ../bin HWADL.COM hello-world.c

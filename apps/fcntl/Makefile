SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: FCNTL.COM FCNTL.EXE

.PHONY: FCNTL.COM
FCNTL.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin FCNTL.COM fcntl.c

.PHONY: FCNTL.EXE
FCNTL.EXE:
	@mkdir -p ../bin
	@ez80-cc --mem-model 2MB --output-dir ../bin FCNTL.EXE fcntl.c

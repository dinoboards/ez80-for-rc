SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: POSXFILE.COM POSXFILE.EXE

.PHONY: POSXFILE.COM
POSXFILE.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin POSXFILE.COM posxfile.c

.PHONY: POSXFILE.EXE
POSXFILE.EXE:
	@mkdir -p ../bin
	@ez80-cc --mem-model 2MB --output-dir ../bin POSXFILE.EXE posxfile.c



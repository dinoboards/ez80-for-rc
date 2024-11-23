SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: POSXFILE.COM

.PHONY: POSXFILE.COM
POSXFILE.COM:
	@ez80-cc --output-dir ../bin POSXFILE.COM posxfile.c


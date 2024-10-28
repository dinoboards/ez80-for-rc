SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: hello-world.com

.PHONY: hello-world.com
hello-world.com:
	@ez80-cc --output-dir ../bin hello-world.c

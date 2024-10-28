SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: mbrot.com

.PHONY: mbrot.com
mbrot.com:
	@ez80-cc --output-dir ../bin mbrot.c config_request.c v9958.asm

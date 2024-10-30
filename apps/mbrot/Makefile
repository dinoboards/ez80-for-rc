SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: MBROT.COM

.PHONY: MBROT.COM
MBROT.COM:
	@ez80-cc --output-dir ../bin MBROT.COM mbrot.c config_request.c v9958.asm v9958.c

SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: MBROT.COM

.PHONY: MBROT.COM
MBROT.COM:
	@ez80-cc --optimise speed --output-dir ../bin MBROT.COM mbrot.c ../common/config_request.c ../common/v9958.asm ../common/v9958.c

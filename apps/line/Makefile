SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: LINE.COM

.PHONY: LINE.COM
LINE.COM:
	@ez80-cc --optimise speed --output-dir ../bin LINE.COM line.c ../common/config_request.c ../common/v9958.asm ../common/v9958.c random.asm

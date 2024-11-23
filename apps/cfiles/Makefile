SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: CFILES.COM

.PHONY: CFILES.COM
CFILES.COM:
	@ez80-cc --output-dir ../bin --optimise size CFILES.COM cfiles.c


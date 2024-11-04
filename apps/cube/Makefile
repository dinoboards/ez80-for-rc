SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: CUBE.COM

.PHONY: CUBE.COM
CUBE.COM:
	@ez80-cc --lib v99x8-standard --output-dir ../bin CUBE.COM cube.c

# Change lib to v99x8-hdmi for tang nano V9958

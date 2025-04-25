SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: USB.COM

SRC_FILES := $(wildcard *.c)

.PHONY: USB.COM
USB.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin USB.COM $(SRC_FILES)

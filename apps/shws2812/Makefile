SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: SHWS2812.COM

.PHONY: SHWS2812.COM
SHWS2812.COM:
	@mkdir -p ../bin
	@ez80-cc  --output-dir ../bin SHWS2812.COM shws2812.c colour_conversion.c

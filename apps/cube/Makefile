SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: HCUBE.COM CUBE.COM

.PHONY: CUBE.COM
CUBE.COM:
	@ez80-cc -DVDP_V9958 --lib v99x8-standard --output-dir ../bin CUBE.COM cube.c

.PHONY: HCUBE.COM
HCUBE.COM:
	@ez80-cc -DVDP_SUPER_HDMI --lib v99x8-hdmi --output-dir ../bin HCUBE.COM cube.c large_palette.c

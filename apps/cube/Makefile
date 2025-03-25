SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: HCUBE.COM CUBE.COM CUBE.EXE

.PHONY: CUBE.COM
CUBE.COM:
	@ez80-cc -DVDP_V9958 --lib v99x8-standard --output-dir ../bin CUBE.COM cube.c

.PHONY: HCUBE.COM
HCUBE.COM:
	@ez80-cc -DVDP_SUPER_HDMI --lib v99x8-hdmi --output-dir ../bin HCUBE.COM cube.c

.PHONY: CUBE.EXE
CUBE.EXE:
	@ez80-cc -DVDP_SUPER_HDMI -DMEM_MODEL=2MB --mem-model 2MB --lib v99x8-hdmi --output-dir ../bin CUBE.EXE cube.c


# Change lib to v99x8-hdmi for tang nano V9958

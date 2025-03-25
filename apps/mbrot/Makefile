SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: MBROT.EXE
# MBROT.COM

.PHONY: MBROT.COM
MBROT.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise speed --output-dir ../bin MBROT.COM mbrot.c ../common/config_request.c

.PHONY: MBROT.EXE
MBROT.EXE:
	@mkdir -p ../bin
	@ez80-cc -DVDP_SUPER_HDMI -DMEM_MODEL=2MB --mem-model 2MB --lib v99x8-hdmi --optimise speed --output-dir ../bin MBROT.EXE mbrot.c ../common/config_request.c large_palette.c wait_for_key.c

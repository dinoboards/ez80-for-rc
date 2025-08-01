SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: SUPHDMI.COM V99X8.COM

C_FILES=$(wildcard ./*.c)

.PHONY: SUPHDMI.COM
SUPHDMI.COM:
	@mkdir -p ../bin
	@ez80-cc --defsym VDP_SUPER_HDMI=1 -DVDP_SUPER_HDMI --optimise speed --lib v99x8-hdmi --output-dir ../bin SUPHDMI.COM $(C_FILES)

.PHONY: V99X8.COM
V99X8.COM:
	@mkdir -p ../bin
	@ez80-cc --defsym VDP_V9958=1 -DVDP_V9958 --optimise speed --lib v99x8-standard --output-dir ../bin V99X8.COM $(C_FILES)

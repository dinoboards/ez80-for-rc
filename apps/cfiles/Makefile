SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

 all: CFILES.EXE CFILES.COM CFILESZ.COM

.PHONY: CFILES.COM
CFILES.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin --optimise size CFILES.COM cfiles.c


.PHONY: CFILES.EXE
CFILES.EXE:
	@mkdir -p ../bin
	@ez80-cc --mem-model 2MB  --output-dir ../bin --optimise size CFILES.EXE cfiles.c

include ../common.mk

# CP/M
$(BIN)CFILESZ.COM: cfiles.c
	$(compile)

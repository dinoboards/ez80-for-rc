SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

.PHONY: updatevmengine
updatevmengine:
	@cd src/q3vm/tools && ./grab-compilers.sh

.PHONY: debug
debug: updatevmengine src/q3vm/vm-vdu/spike.c
	@wine make-debug.bat

.PHONY: release
release: updatevmengine src/q3vm/vm-vdu/spike.c
	@wine make-release.bat


src/q3vm/vm-vdu/spike.c src/q3vm/vm-vdu/spike.h: src/q3vm/vm-vdu/main.c src/q3vm/vm-vdu/bytecode.q3asm src/q3vm/tools/*
	@cd src/q3vm/vm-vdu && ./build.sh


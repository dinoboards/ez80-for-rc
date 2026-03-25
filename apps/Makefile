SHELL := /bin/bash
BIN=./bin/
MAKEFLAGS += --no-print-directory

# Define the sub directories to be built
BINARIES := CFILES VHBIOS CPMEXMPL CUBE DHRY EXE EZ80 HWADL HWZ80 I2CSCAN LINE MATRIX8 MBROT VFYADL VFYADL VFYZ80 WHET WOLF3D SUPHDMI SHWS2812 XR XS

# Default target
all: $(BINARIES:%=$(BIN)%.COM)
	@

# Pattern rule to build each binary
.PHONY: $(BIN)%.COM
$(BIN)%.COM: common-libs
	@LOWERCASE_STEM=$(shell echo $* | tr '[:upper:]' '[:lower:]') && \
	$(MAKE) -C $${LOWERCASE_STEM}

.PHONY: bin/VFYADL.COM
bin/VFYADL.COM :
	$(MAKE) -C verify-lib-references

.PHONY: bin/VHBIOS.COM
.PHONY: bin/VHBIOS.EXE
bin/VHBIOS.COM bin/VHBIOS.EXE :
	$(MAKE) -C verify-hbios

.PHONY: bin/VFYZ80.COM
bin/VFYZ80.COM: bin/VFYADL.COM

.PHONY: common-libs
common-libs:
	@$(MAKE) -C common

z88dk:
	@docker run --rm -w /host/${PWD} -v /:/host/ -u $(USER_ID):$(GROUP_ID) -it z88dk/z88dk

bin/XR.COM:
	@if ! command -v z80asm &> /dev/null; then \
		echo -e "\033[0;31mError: to assemble the XR and XS utils, you need to have z80asm installed. Please install z80asm (eg: sudo apt install z80asm).\033[0m" >&2; \
		exit 0; \
	fi; \
	cd xmodem80 && z80asm xr.z80 --list --output ../bin/XR.COM 2> ../bin/XR.COM.lst

bin/XS.COM:
	@if ! command -v z80asm &> /dev/null; then \
		echo -e "\033[0;31mError: to assemble the XR and XS utils, you need to have z80asm installed. Please install z80asm (eg: sudo apt install z80asm).\033[0m" >&2; \
		exit 0; \
	fi;
	cd xmodem80 && z80asm xs.z80 --list --output ../bin/XS.COM 2> ../bin/XS.COM.lst

clean:
	@rm -rf $(BIN)

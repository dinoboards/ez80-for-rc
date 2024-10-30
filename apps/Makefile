SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

# Define the binaries
BINARIES := matrix8 i2cscan dhry whet fwupdate fwmain fwstatus ez80

# Default target
all: $(BINARIES:%=$(BIN)%.com)
	@

# Pattern rule to build each binary
.PHONY: $(BIN)%.com
$(BIN)%.com: common-libs
	@$(MAKE) -C $*

.PHONY: common-libs
common-libs:
	@$(MAKE) -C common

z88dk:
	@docker run -w /host/${PWD} -v /:/host/ -u $(USER_ID):$(GROUP_ID) -it z88dk/z88dk

clean:
	@rm -rf $(BIN)

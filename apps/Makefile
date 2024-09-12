BIN=./bin/
MAKEFLAGS += --no-print-directory

all: clean
	@docker run -v .:/src/ -it z88dk/z88dk make apps -j --output-sync=recurse

z88dk:
	@docker run -v .:/src/ -it z88dk/z88dk

apps: $(BIN)matrix8.com $(BIN)i2cscan.com $(BIN)dhry.com $(BIN)whet.com $(BIN)fwupdate.com $(BIN)fwmain.com $(BIN)fwstatus.com $(BIN)ez80.com

clean:
	@rm -rf $(BIN)

common-libs:
	@$(MAKE) -C common

.PHONY: $(BIN)i2cscan.com
$(BIN)i2cscan.com: common-libs
	@$(MAKE) -C i2cscan

.PHONY: $(BIN)matrix8.com
$(BIN)matrix8.com: common-libs
	@$(MAKE) -C matrix8

.PHONY: $(BIN)dhry.com
$(BIN)dhry.com: common-libs
	@$(MAKE) -C dhry

.PHONY: $(BIN)whet.com
$(BIN)whet.com: common-libs
	@$(MAKE) -C whet

.PHONY: $(BIN)fwupdate.com
$(BIN)fwupdate.com: common-libs
	@$(MAKE) -C fwupdate

.PHONY: $(BIN)fwmain.com
$(BIN)fwmain.com: common-libs
	@$(MAKE) -C fwmain

.PHONY: $(BIN)fwstatus.com
$(BIN)fwstatus.com: common-libs
	@$(MAKE) -C fwstatus

.PHONY: $(BIN)ez80.com
$(BIN)ez80.com: common-libs
	@$(MAKE) -C ez80

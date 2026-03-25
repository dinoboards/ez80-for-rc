SHELL := /bin/bash
BIN=./bin/
MAKEFLAGS += --no-print-directory

all: hd1k_cpm22.img

.PHONY: ../cpm22/debug/cpm22img.sys
../cpm22/debug/cpm22img.sys:
	@$(MAKE) -C ../cpm22

cpm22/u0/CLRDIR.COM:
	@echo "Retrieving CLRDIR.COM FROM ROMWBW"
	@wget -q https://github.com/wwarthen/RomWBW/raw/refs/heads/master/Source/Images/Common/All/u15/CLRDIR.COM -O ./cpm22/u0/CLRDIR.COM

cpm22/u0/EZ80.COM:
	@$(MAKE) -C ../../apps/ez80 > /dev/null
	@cp ../../apps/bin/EZ80.COM ./cpm22/u0/EZ80.COM

cpm22/u0/XS.COM:
	@$(MAKE) -C ../../apps bin/XS.COM > /dev/null
	@if [[ -e ../../apps/bin/XS.COM ]]; then cp ../../apps/bin/XS.COM ./cpm22/u0/XS.COM; fi

cpm22/u0/XR.COM:
	@$(MAKE) -C ../../apps bin/XR.COM > /dev/null
	@if [[ -e ../../apps/bin/XS.COM ]]; then cp ../../apps/bin/XR.COM ./cpm22/u0/XR.COM; fi

debug/hd1k_prefix.dat: hd1k_prefix.dat.xz
	@mkdir -p debug
	@xz -dc hd1k_prefix.dat.xz > debug/hd1k_prefix.dat

.PHONY: debug/cpm22.img
debug/cpm22.img: ../cpm22/debug/cpm22img.sys cpm22/u0/CLRDIR.COM cpm22/u0/EZ80.COM cpm22/u0/XS.COM cpm22/u0/XR.COM
	@./make-cpm22.sh

hd1k_cpm22.img: debug/hd1k_prefix.dat debug/cpm22.img
	@echo "Building multi-slice disk image: hd1k_cpm22.img"
	@cat debug/hd1k_prefix.dat debug/cpm22.img > hd1k_cpm22.img
	@echo -e "Done.  \n\nYou can now flash the file hd1k_cpm22.img using your favourite tool (eg: dd, balenaEtcher)"

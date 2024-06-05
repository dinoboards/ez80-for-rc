SHELL = cmd.exe

#
# ZDS II Make File - rc2014-firmware project, release configuration
#
# Generated by: ZDS II - eZ80Acclaim! 5.3.5 (Build 23020901)
#   IDE component: d:5.3.0:23020901
#   Install Path: Z:\ZDS\
#

RM = del

# ZDS base directory
ZDSDIR = Z:\ZDS
ZDSDIR_ESCSPACE = Z:\ZDS

# ZDS bin directory
BIN = $(ZDSDIR)\bin

# ZDS include base directory
INCLUDE = Z:\ZDS\include
INCLUDE_ESCSPACE = Z:\ZDS\include

# ZTP base directory
ZTPDIR = Z:\ZDS\ZTP2.5.1
ZTPDIR_ESCSPACE = Z:\ZDS\ZTP2.5.1

# project directory
PRJDIR = Z:\rc2014-ez80-firmware
PRJDIR_ESCSPACE = Z:\rc2014-ez80-firmware

# intermediate files directory
WORKDIR = Z:\rc2014-ez80-firmware\release
WORKDIR_ESCSPACE = Z:\rc2014-ez80-firmware\release

# output files directory
OUTDIR = Z:\rc2014-ez80-firmware\release\
OUTDIR_ESCSPACE = Z:\rc2014-ez80-firmware\release\

# tools
CC = @"$(BIN)\eZ80cc"
AS = @"$(BIN)\eZ80asm"
LD = @"$(BIN)\eZ80link"
AR = @"$(BIN)\eZ80lib"
WEBTOC = @"$(BIN)\mkwebpage"

CFLAGS =  \
-define:NDEBUG -define:_EZ80F92 -define:_EZ80ACCLAIM! -genprintf  \
-NOkeepasm -keeplst -NOlist -NOlistinc -NOmodsect -optsize  \
-promote -NOreduceopt  \
-stdinc:"\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog;$(INCLUDE)\zilog\""  \
-usrinc:"\"..;\"" -NOmultithread -NOpadbranch -NOdebug  \
-cpu:eZ80F92  \
-asmsw:"   \
	-define:_EZ80ACCLAIM!=1  \
	-include:\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog\" -list  \
	-NOlistmac -pagelen:0 -pagewidth:80 -quiet -sdiopt -warn -NOdebug  \
	-NOigcase -cpu:eZ80F92"

ASFLAGS =  \
-define:_EZ80ACCLAIM!=1  \
-include:"\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog\"" -list  \
-NOlistmac -name -pagelen:0 -pagewidth:80 -quiet -sdiopt -warn  \
-NOdebug -NOigcase -cpu:eZ80F92

LDFLAGS = @.\rc2014-firmware_Release.linkcmd
build: rc2014-firmware

buildall: clean rc2014-firmware

relink: deltarget rc2014-firmware

deltarget: 
	@if exist "$(WORKDIR)\rc2014-firmware.lod"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.lod"
	@if exist "$(WORKDIR)\rc2014-firmware.hex"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.hex"
	@if exist "$(WORKDIR)\rc2014-firmware.map"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.map"

clean: 
	@if exist "$(WORKDIR)\rc2014-firmware.lod"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.lod"
	@if exist "$(WORKDIR)\rc2014-firmware.hex"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.hex"
	@if exist "$(WORKDIR)\rc2014-firmware.map"  \
            $(RM) "$(WORKDIR)\rc2014-firmware.map"
	@if exist "$(WORKDIR)\console.obj"  \
            $(RM) "$(WORKDIR)\console.obj"
	@if exist "$(WORKDIR)\console.lis"  \
            $(RM) "$(WORKDIR)\console.lis"
	@if exist "$(WORKDIR)\console.lst"  \
            $(RM) "$(WORKDIR)\console.lst"
	@if exist "$(WORKDIR)\console.src"  \
            $(RM) "$(WORKDIR)\console.src"
	@if exist "$(WORKDIR)\cstartup.obj"  \
            $(RM) "$(WORKDIR)\cstartup.obj"
	@if exist "$(WORKDIR)\cstartup.lis"  \
            $(RM) "$(WORKDIR)\cstartup.lis"
	@if exist "$(WORKDIR)\cstartup.lst"  \
            $(RM) "$(WORKDIR)\cstartup.lst"
	@if exist "$(WORKDIR)\init_params_f92.obj"  \
            $(RM) "$(WORKDIR)\init_params_f92.obj"
	@if exist "$(WORKDIR)\init_params_f92.lis"  \
            $(RM) "$(WORKDIR)\init_params_f92.lis"
	@if exist "$(WORKDIR)\init_params_f92.lst"  \
            $(RM) "$(WORKDIR)\init_params_f92.lst"
	@if exist "$(WORKDIR)\main.obj"  \
            $(RM) "$(WORKDIR)\main.obj"
	@if exist "$(WORKDIR)\main.lis"  \
            $(RM) "$(WORKDIR)\main.lis"
	@if exist "$(WORKDIR)\main.lst"  \
            $(RM) "$(WORKDIR)\main.lst"
	@if exist "$(WORKDIR)\rst-io.obj"  \
            $(RM) "$(WORKDIR)\rst-io.obj"
	@if exist "$(WORKDIR)\rst-io.lis"  \
            $(RM) "$(WORKDIR)\rst-io.lis"
	@if exist "$(WORKDIR)\rst-io.lst"  \
            $(RM) "$(WORKDIR)\rst-io.lst"
	@if exist "$(WORKDIR)\rst-rc2014-bank-switch.obj"  \
            $(RM) "$(WORKDIR)\rst-rc2014-bank-switch.obj"
	@if exist "$(WORKDIR)\rst-rc2014-bank-switch.lis"  \
            $(RM) "$(WORKDIR)\rst-rc2014-bank-switch.lis"
	@if exist "$(WORKDIR)\rst-rc2014-bank-switch.lst"  \
            $(RM) "$(WORKDIR)\rst-rc2014-bank-switch.lst"
	@if exist "$(WORKDIR)\rst-rc2014-fnc.obj"  \
            $(RM) "$(WORKDIR)\rst-rc2014-fnc.obj"
	@if exist "$(WORKDIR)\rst-rc2014-fnc.lis"  \
            $(RM) "$(WORKDIR)\rst-rc2014-fnc.lis"
	@if exist "$(WORKDIR)\rst-rc2014-fnc.lst"  \
            $(RM) "$(WORKDIR)\rst-rc2014-fnc.lst"
	@if exist "$(WORKDIR)\test.obj"  \
            $(RM) "$(WORKDIR)\test.obj"
	@if exist "$(WORKDIR)\test.lis"  \
            $(RM) "$(WORKDIR)\test.lis"
	@if exist "$(WORKDIR)\test.lst"  \
            $(RM) "$(WORKDIR)\test.lst"
	@if exist "$(WORKDIR)\uart.obj"  \
            $(RM) "$(WORKDIR)\uart.obj"
	@if exist "$(WORKDIR)\uart.lis"  \
            $(RM) "$(WORKDIR)\uart.lis"
	@if exist "$(WORKDIR)\uart.lst"  \
            $(RM) "$(WORKDIR)\uart.lst"
	@if exist "$(WORKDIR)\uart.src"  \
            $(RM) "$(WORKDIR)\uart.src"
	@if exist "$(WORKDIR)\vectors16.obj"  \
            $(RM) "$(WORKDIR)\vectors16.obj"
	@if exist "$(WORKDIR)\vectors16.lis"  \
            $(RM) "$(WORKDIR)\vectors16.lis"
	@if exist "$(WORKDIR)\vectors16.lst"  \
            $(RM) "$(WORKDIR)\vectors16.lst"
	@if exist "$(WORKDIR)\zsldevinit.obj"  \
            $(RM) "$(WORKDIR)\zsldevinit.obj"
	@if exist "$(WORKDIR)\zsldevinit.lis"  \
            $(RM) "$(WORKDIR)\zsldevinit.lis"
	@if exist "$(WORKDIR)\zsldevinit.lst"  \
            $(RM) "$(WORKDIR)\zsldevinit.lst"

# pre-4.11.0 compatibility
rebuildall: buildall 

LIBS = 

OBJS =  \
            $(WORKDIR_ESCSPACE)\console.obj  \
            $(WORKDIR_ESCSPACE)\cstartup.obj  \
            $(WORKDIR_ESCSPACE)\init_params_f92.obj  \
            $(WORKDIR_ESCSPACE)\main.obj  \
            $(WORKDIR_ESCSPACE)\rst-io.obj  \
            $(WORKDIR_ESCSPACE)\rst-rc2014-bank-switch.obj  \
            $(WORKDIR_ESCSPACE)\rst-rc2014-fnc.obj  \
            $(WORKDIR_ESCSPACE)\test.obj  \
            $(WORKDIR_ESCSPACE)\uart.obj  \
            $(WORKDIR_ESCSPACE)\vectors16.obj  \
            $(WORKDIR_ESCSPACE)\zsldevinit.obj

rc2014-firmware: $(OBJS)
	 $(LD) $(LDFLAGS)

$(WORKDIR_ESCSPACE)\console.obj :  \
            $(PRJDIR_ESCSPACE)\src\console.c  \
            $(INCLUDE_ESCSPACE)\std\Format.h  \
            $(INCLUDE_ESCSPACE)\std\Stdarg.h  \
            $(INCLUDE_ESCSPACE)\std\Stdio.h  \
            $(INCLUDE_ESCSPACE)\zilog\cio.h  \
            $(INCLUDE_ESCSPACE)\zilog\defines.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80190.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F91.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F92.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F93.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80L92.h  \
            $(INCLUDE_ESCSPACE)\zilog\ez80.h  \
            $(INCLUDE_ESCSPACE)\zilog\gpio.h  \
            $(INCLUDE_ESCSPACE)\zilog\uart.h  \
            $(INCLUDE_ESCSPACE)\zilog\uartdefs.h  \
            $(PRJDIR_ESCSPACE)\src\uart.h
	 $(CC) $(CFLAGS) "$(PRJDIR)\src\console.c"

$(WORKDIR_ESCSPACE)\cstartup.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\cstartup.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\cstartup.asm"

$(WORKDIR_ESCSPACE)\init_params_f92.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\init_params_f92.asm  \
            $(PRJDIR_ESCSPACE)\src\startup\ez80F92.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\init_params_f92.asm"

$(WORKDIR_ESCSPACE)\main.obj :  \
            $(PRJDIR_ESCSPACE)\src\main.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\main.s"

$(WORKDIR_ESCSPACE)\rst-io.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-io.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-io.s"

$(WORKDIR_ESCSPACE)\rst-rc2014-bank-switch.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-rc2014-bank-switch.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-rc2014-bank-switch.s"

$(WORKDIR_ESCSPACE)\rst-rc2014-fnc.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-rc2014-fnc.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-rc2014-fnc.s"

$(WORKDIR_ESCSPACE)\test.obj :  \
            $(PRJDIR_ESCSPACE)\src\test.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\test.s"

$(WORKDIR_ESCSPACE)\uart.obj :  \
            $(PRJDIR_ESCSPACE)\src\uart.c  \
            $(INCLUDE_ESCSPACE)\std\Format.h  \
            $(INCLUDE_ESCSPACE)\std\Stdarg.h  \
            $(INCLUDE_ESCSPACE)\std\Stdio.h  \
            $(INCLUDE_ESCSPACE)\zilog\cio.h  \
            $(INCLUDE_ESCSPACE)\zilog\defines.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80190.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F91.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F92.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80F93.h  \
            $(INCLUDE_ESCSPACE)\zilog\eZ80L92.h  \
            $(INCLUDE_ESCSPACE)\zilog\ez80.h  \
            $(INCLUDE_ESCSPACE)\zilog\gpio.h  \
            $(INCLUDE_ESCSPACE)\zilog\uart.h  \
            $(INCLUDE_ESCSPACE)\zilog\uartdefs.h  \
            $(PRJDIR_ESCSPACE)\src\uart.h
	 $(CC) $(CFLAGS) "$(PRJDIR)\src\uart.c"

$(WORKDIR_ESCSPACE)\vectors16.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\vectors16.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\vectors16.asm"

$(WORKDIR_ESCSPACE)\zsldevinit.obj :  \
            $(PRJDIR_ESCSPACE)\zsldevinit.asm  \
            $(INCLUDE_ESCSPACE)\zilog\intvect.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\zsldevinit.asm"


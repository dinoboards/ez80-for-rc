SHELL = cmd.exe

#
# ZDS II Make File - simplest project, debug configuration
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
PRJDIR = Z:\simplest
PRJDIR_ESCSPACE = Z:\simplest

# intermediate files directory
WORKDIR = Z:\simplest\debug
WORKDIR_ESCSPACE = Z:\simplest\debug

# output files directory
OUTDIR = Z:\simplest\debug\
OUTDIR_ESCSPACE = Z:\simplest\debug\

# tools
CC = @"$(BIN)\eZ80cc"
AS = @"$(BIN)\eZ80asm"
LD = @"$(BIN)\eZ80link"
AR = @"$(BIN)\eZ80lib"
WEBTOC = @"$(BIN)\mkwebpage"

CFLAGS =  \
-define:_DEBUG -define:_EZ80F92 -define:_EZ80ACCLAIM! -genprintf  \
-keepasm -keeplst -list -listinc -NOmodsect -optsize -promote  \
-NOreduceopt  \
-stdinc:"\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog;$(INCLUDE)\zilog\""  \
-usrinc:"\"..;\"" -NOmultithread -NOpadbranch -debug -cpu:eZ80F92  \
-asmsw:"   \
	-define:_EZ80ACCLAIM!=1  \
	-include:\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog\" -list  \
	-NOlistmac -pagelen:0 -pagewidth:80 -quiet -sdiopt -warn -debug  \
	-NOigcase -cpu:eZ80F92"

ASFLAGS =  \
-define:_EZ80ACCLAIM!=1  \
-include:"\"..;Z:\ZDS\include\std;Z:\ZDS\include\zilog\"" -list  \
-NOlistmac -name -pagelen:0 -pagewidth:80 -quiet -sdiopt -warn  \
-debug -NOigcase -cpu:eZ80F92

LDFLAGS = @.\simplest_debug.linkcmd
build: simplest

buildall: clean simplest

relink: deltarget simplest

deltarget: 
	@if exist "$(WORKDIR)\simplest.lod"  \
            $(RM) "$(WORKDIR)\simplest.lod"
	@if exist "$(WORKDIR)\simplest.hex"  \
            $(RM) "$(WORKDIR)\simplest.hex"
	@if exist "$(WORKDIR)\simplest.map"  \
            $(RM) "$(WORKDIR)\simplest.map"

clean: 
	@if exist "$(WORKDIR)\simplest.lod"  \
            $(RM) "$(WORKDIR)\simplest.lod"
	@if exist "$(WORKDIR)\simplest.hex"  \
            $(RM) "$(WORKDIR)\simplest.hex"
	@if exist "$(WORKDIR)\simplest.map"  \
            $(RM) "$(WORKDIR)\simplest.map"
	@if exist "$(WORKDIR)\main.obj"  \
            $(RM) "$(WORKDIR)\main.obj"
	@if exist "$(WORKDIR)\main.lis"  \
            $(RM) "$(WORKDIR)\main.lis"
	@if exist "$(WORKDIR)\main.lst"  \
            $(RM) "$(WORKDIR)\main.lst"
	@if exist "$(WORKDIR)\main.src"  \
            $(RM) "$(WORKDIR)\main.src"
	@if exist "$(WORKDIR)\zsldevinit.obj"  \
            $(RM) "$(WORKDIR)\zsldevinit.obj"
	@if exist "$(WORKDIR)\zsldevinit.lis"  \
            $(RM) "$(WORKDIR)\zsldevinit.lis"
	@if exist "$(WORKDIR)\zsldevinit.lst"  \
            $(RM) "$(WORKDIR)\zsldevinit.lst"
	@if exist "$(WORKDIR)\test.obj"  \
            $(RM) "$(WORKDIR)\test.obj"
	@if exist "$(WORKDIR)\test.lis"  \
            $(RM) "$(WORKDIR)\test.lis"
	@if exist "$(WORKDIR)\test.lst"  \
            $(RM) "$(WORKDIR)\test.lst"
	@if exist "$(WORKDIR)\jump_table.obj"  \
            $(RM) "$(WORKDIR)\jump_table.obj"
	@if exist "$(WORKDIR)\jump_table.lis"  \
            $(RM) "$(WORKDIR)\jump_table.lis"
	@if exist "$(WORKDIR)\jump_table.lst"  \
            $(RM) "$(WORKDIR)\jump_table.lst"
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
	@if exist "$(WORKDIR)\vectors16.obj"  \
            $(RM) "$(WORKDIR)\vectors16.obj"
	@if exist "$(WORKDIR)\vectors16.lis"  \
            $(RM) "$(WORKDIR)\vectors16.lis"
	@if exist "$(WORKDIR)\vectors16.lst"  \
            $(RM) "$(WORKDIR)\vectors16.lst"

# pre-4.11.0 compatibility
rebuildall: buildall 

LIBS = 

OBJS =  \
            $(WORKDIR_ESCSPACE)\main.obj  \
            $(WORKDIR_ESCSPACE)\zsldevinit.obj  \
            $(WORKDIR_ESCSPACE)\test.obj  \
            $(WORKDIR_ESCSPACE)\jump_table.obj  \
            $(WORKDIR_ESCSPACE)\cstartup.obj  \
            $(WORKDIR_ESCSPACE)\init_params_f92.obj  \
            $(WORKDIR_ESCSPACE)\vectors16.obj

simplest: $(OBJS)
	 $(LD) $(LDFLAGS)

$(WORKDIR_ESCSPACE)\main.obj :  \
            $(PRJDIR_ESCSPACE)\src\main.c  \
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
            $(INCLUDE_ESCSPACE)\zilog\uartdefs.h
	 $(CC) $(CFLAGS) "$(PRJDIR)\src\main.c"

$(WORKDIR_ESCSPACE)\zsldevinit.obj :  \
            $(PRJDIR_ESCSPACE)\zsldevinit.asm  \
            $(INCLUDE_ESCSPACE)\zilog\intvect.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\zsldevinit.asm"

$(WORKDIR_ESCSPACE)\test.obj :  \
            $(PRJDIR_ESCSPACE)\src\test.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\test.s"

$(WORKDIR_ESCSPACE)\jump_table.obj :  \
            $(PRJDIR_ESCSPACE)\src\jump_table.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\jump_table.s"

$(WORKDIR_ESCSPACE)\cstartup.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\cstartup.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\cstartup.asm"

$(WORKDIR_ESCSPACE)\init_params_f92.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\init_params_f92.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\init_params_f92.asm"

$(WORKDIR_ESCSPACE)\vectors16.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\vectors16.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\vectors16.asm"


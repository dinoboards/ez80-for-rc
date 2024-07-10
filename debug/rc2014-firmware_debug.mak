SHELL = cmd.exe

#
# ZDS II Make File - rc2014-firmware project, debug configuration
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
WORKDIR = Z:\rc2014-ez80-firmware\debug
WORKDIR_ESCSPACE = Z:\rc2014-ez80-firmware\debug

# output files directory
OUTDIR = Z:\rc2014-ez80-firmware\debug\
OUTDIR_ESCSPACE = Z:\rc2014-ez80-firmware\debug\

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

LDFLAGS = @.\rc2014-firmware_debug.linkcmd
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
	@if exist "$(WORKDIR)\60-hz-counter.obj"  \
            $(RM) "$(WORKDIR)\60-hz-counter.obj"
	@if exist "$(WORKDIR)\60-hz-counter.lis"  \
            $(RM) "$(WORKDIR)\60-hz-counter.lis"
	@if exist "$(WORKDIR)\60-hz-counter.lst"  \
            $(RM) "$(WORKDIR)\60-hz-counter.lst"
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
	@if exist "$(WORKDIR)\real-time-clock.obj"  \
            $(RM) "$(WORKDIR)\real-time-clock.obj"
	@if exist "$(WORKDIR)\real-time-clock.lis"  \
            $(RM) "$(WORKDIR)\real-time-clock.lis"
	@if exist "$(WORKDIR)\real-time-clock.lst"  \
            $(RM) "$(WORKDIR)\real-time-clock.lst"
	@if exist "$(WORKDIR)\rst-08-io.obj"  \
            $(RM) "$(WORKDIR)\rst-08-io.obj"
	@if exist "$(WORKDIR)\rst-08-io.lis"  \
            $(RM) "$(WORKDIR)\rst-08-io.lis"
	@if exist "$(WORKDIR)\rst-08-io.lst"  \
            $(RM) "$(WORKDIR)\rst-08-io.lst"
	@if exist "$(WORKDIR)\rst-10-rc2014-fnc.obj"  \
            $(RM) "$(WORKDIR)\rst-10-rc2014-fnc.obj"
	@if exist "$(WORKDIR)\rst-10-rc2014-fnc.lis"  \
            $(RM) "$(WORKDIR)\rst-10-rc2014-fnc.lis"
	@if exist "$(WORKDIR)\rst-10-rc2014-fnc.lst"  \
            $(RM) "$(WORKDIR)\rst-10-rc2014-fnc.lst"
	@if exist "$(WORKDIR)\rst-18-rc2014-bank-switch.obj"  \
            $(RM) "$(WORKDIR)\rst-18-rc2014-bank-switch.obj"
	@if exist "$(WORKDIR)\rst-18-rc2014-bank-switch.lis"  \
            $(RM) "$(WORKDIR)\rst-18-rc2014-bank-switch.lis"
	@if exist "$(WORKDIR)\rst-18-rc2014-bank-switch.lst"  \
            $(RM) "$(WORKDIR)\rst-18-rc2014-bank-switch.lst"
	@if exist "$(WORKDIR)\test.obj"  \
            $(RM) "$(WORKDIR)\test.obj"
	@if exist "$(WORKDIR)\test.lis"  \
            $(RM) "$(WORKDIR)\test.lis"
	@if exist "$(WORKDIR)\test.lst"  \
            $(RM) "$(WORKDIR)\test.lst"
	@if exist "$(WORKDIR)\uart_control.obj"  \
            $(RM) "$(WORKDIR)\uart_control.obj"
	@if exist "$(WORKDIR)\uart_control.lis"  \
            $(RM) "$(WORKDIR)\uart_control.lis"
	@if exist "$(WORKDIR)\uart_control.lst"  \
            $(RM) "$(WORKDIR)\uart_control.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_add_to.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_add_to.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_add_to.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_add_to.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_add_to.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_add_to.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_empty.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_empty.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_empty.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_empty.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_empty.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_empty.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_full.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_full.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_full.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_full.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_full.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_full.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_get.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_get.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_get.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_get_length.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get_length.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_get_length.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get_length.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_get_length.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_get_length.lst"
	@if exist "$(WORKDIR)\uart_rx_buffer_init.obj"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_init.obj"
	@if exist "$(WORKDIR)\uart_rx_buffer_init.lis"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_init.lis"
	@if exist "$(WORKDIR)\uart_rx_buffer_init.lst"  \
            $(RM) "$(WORKDIR)\uart_rx_buffer_init.lst"
	@if exist "$(WORKDIR)\uart_tx_buffer.obj"  \
            $(RM) "$(WORKDIR)\uart_tx_buffer.obj"
	@if exist "$(WORKDIR)\uart_tx_buffer.lis"  \
            $(RM) "$(WORKDIR)\uart_tx_buffer.lis"
	@if exist "$(WORKDIR)\uart_tx_buffer.lst"  \
            $(RM) "$(WORKDIR)\uart_tx_buffer.lst"
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
            $(WORKDIR_ESCSPACE)\60-hz-counter.obj  \
            $(WORKDIR_ESCSPACE)\cstartup.obj  \
            $(WORKDIR_ESCSPACE)\init_params_f92.obj  \
            $(WORKDIR_ESCSPACE)\main.obj  \
            $(WORKDIR_ESCSPACE)\real-time-clock.obj  \
            $(WORKDIR_ESCSPACE)\rst-08-io.obj  \
            $(WORKDIR_ESCSPACE)\rst-10-rc2014-fnc.obj  \
            $(WORKDIR_ESCSPACE)\rst-18-rc2014-bank-switch.obj  \
            $(WORKDIR_ESCSPACE)\test.obj  \
            $(WORKDIR_ESCSPACE)\uart_control.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_add_to.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_empty.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_full.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_get.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_get_length.obj  \
            $(WORKDIR_ESCSPACE)\uart_rx_buffer_init.obj  \
            $(WORKDIR_ESCSPACE)\uart_tx_buffer.obj  \
            $(WORKDIR_ESCSPACE)\vectors16.obj  \
            $(WORKDIR_ESCSPACE)\zsldevinit.obj

rc2014-firmware: $(OBJS)
	 $(LD) $(LDFLAGS)

$(WORKDIR_ESCSPACE)\60-hz-counter.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\60-hz-counter.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\60-hz-counter.s"

$(WORKDIR_ESCSPACE)\cstartup.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\cstartup.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\cstartup.asm"

$(WORKDIR_ESCSPACE)\init_params_f92.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\init_params_f92.asm  \
            $(PRJDIR_ESCSPACE)\src\config.inc  \
            $(PRJDIR_ESCSPACE)\src\startup\ez80F92.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\init_params_f92.asm"

$(WORKDIR_ESCSPACE)\main.obj :  \
            $(PRJDIR_ESCSPACE)\src\main.s  \
            $(PRJDIR_ESCSPACE)\src\config.inc  \
            $(PRJDIR_ESCSPACE)\src\startup\ez80F92.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\main.s"

$(WORKDIR_ESCSPACE)\real-time-clock.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\real-time-clock.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\real-time-clock.s"

$(WORKDIR_ESCSPACE)\rst-08-io.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-08-io.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-08-io.s"

$(WORKDIR_ESCSPACE)\rst-10-rc2014-fnc.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-rc2014-fnc.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-rc2014-fnc.s"

$(WORKDIR_ESCSPACE)\rst-18-rc2014-bank-switch.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-18-rc2014-bank-switch.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-18-rc2014-bank-switch.s"

$(WORKDIR_ESCSPACE)\test.obj :  \
            $(PRJDIR_ESCSPACE)\src\test.s  \
            $(PRJDIR_ESCSPACE)\src\startup\ez80F92.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\test.s"

$(WORKDIR_ESCSPACE)\uart_control.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_control.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_control.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_add_to.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_add_to.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_add_to.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_empty.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_empty.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_empty.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_full.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_full.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_full.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_get.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_get.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_get.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_get_length.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_get_length.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_get_length.s"

$(WORKDIR_ESCSPACE)\uart_rx_buffer_init.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_rx_buffer_init.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_rx_buffer_init.s"

$(WORKDIR_ESCSPACE)\uart_tx_buffer.obj :  \
            $(PRJDIR_ESCSPACE)\src\rst-10-drivers\uart_tx_buffer.s
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\rst-10-drivers\uart_tx_buffer.s"

$(WORKDIR_ESCSPACE)\vectors16.obj :  \
            $(PRJDIR_ESCSPACE)\src\startup\vectors16.asm
	 $(AS) $(ASFLAGS) "$(PRJDIR)\src\startup\vectors16.asm"

$(WORKDIR_ESCSPACE)\zsldevinit.obj :  \
            $(PRJDIR_ESCSPACE)\zsldevinit.asm  \
            $(INCLUDE_ESCSPACE)\zilog\intvect.inc
	 $(AS) $(ASFLAGS) "$(PRJDIR)\zsldevinit.asm"


# eZ80 Firmware

The eZ80 firmware is for the eZ80 for RCBus Module.  It is designed to work in conjunction with HBIOS/RomWBW developed and maintained by Wayne Warthen.

See [RomWBW](https://github.com/wwarthen/RomWBW)

The firmware will manage startup, configuration of on-chip devices, interrupt marshalling and handover to RomWBW in an external ROM/RAM module.

## Building the firmware code

This is a ZDSII ez80Acclaim (5.3.5) project.

The build process requires a windows installation, and that the source be placed in a specific absolute path:

Recommend process:
* Clone repo to `Z:\ez80-for-rc`
* Install the ZDS IDE to `Z:\ZDS`
* Using ZDSII IDE, open the project file in Z:\ez80-for-rc\firmware\ez80-for-rc-firmware.zdsproj

> Many of the configuration files created and managed by the ZDSII IDE include full absolute paths to files.  So if you try open the project file
under a different local path, many of the project files will be updated to that path.  It may possibly still work -- but if you wish to contribute to this repo, you would need to undo all these path changes.

### Using a local share to create a 'Z:' drive on your PC

If like me, you don't have an actual drive mounted as `Z:`, you can create a network mount to a directory on your computer.

1. Choose/create any empty directory on your computer.
2. Create a network share on this directory (typically right click on windows explorer and choose Properties, then choose 'Share' tab) - call it `EZ80`
3. Mount a drive on your local computer to this newly created network share (you may have to manually enter the network path `\\<your-computer-name>\EZ80`)
4. Mount and ensure you select 'Reconnect at sign-in'

Once you have created a `Z:` drive, you can follow the recommended instruction above to clone this repo.

### using wine on linux

Wine on linux can be used to also build firmware

> Although the ZDS IDE will run just fine under wine - it can not be used to flash or debug with the Zilog Smart Cable as wine has no support for mapping linux usb devices to applications

1. Install wine as per your system requirements

2. Download the ZDS IDE https://zilog.com/index.php?option=com_zcm&task=view&soft_id=54&Itemid=74

3. unzip the file and copy it to your wine's drive

   `cp ~/Downloads/zds2_eZ80Acclaim\!_5.3.5_23020901.exe ~/.wine/drive_c/install.exe`

4. configure Z: drive to point to *parent* directory of this repo, using the `winecfg` utility.
   *This is very important - as ZDS IDE project files all assume a specific root path -- failure to do this will cause file paths to be 'dynamically' changed in the project and make files.*

5. Install the IDE targeting the install directory of: `Z:\ZDS\`

   `wine c:\install.exe`

6. Add the ZDE bin directory to the path by exporting the env var `WINEPATH` in your host (eg: in your .bashrc)

   `export WINEPATH="Z:\\ZDS\\bin"`

Confirm this installation

7. start wine with windows terminal program

   `wine cmd.exe`

8. Then within the wine env, you should now to able to compile the code:

   ```
   cd z:\ez80-for-rc\firmware
   make-debug.bat
   ```

9. To run the gui IDE - required for step-by-step debugging:

    `wine Zds2Ide.exe`


### Installing within a VM

I have used kvm to host a windows virtual machine (windows 10 pro), installing ZDS within that environment.

I also setup virtiofs to map a directory on my linux host into the VM.  I can not, at this stage though, run ZDS from the Z: over the mapped drive (due to case sensitivity issues) - so ZDS needs to be installed into the VM's native windows ntfs drive (C:\ZDS)

I had limited success with [winapps](https://nowsci.com/winapps/) to enable running the IDE as as 'normal' linux window (not a fully shared desktop).  Window disappears when attempting to flash or start a debug session.

> running winapps (remoteApp) requires windows Pro - with the following registry edit applied:
> `[HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services] "fAllowUnlistedRemotePrograms"=dword:00000001`



### Zilog Tool set

The Zilog ZDS IDE can be found at: https://zilog.com/index.php?option=com_zcm&task=view&soft_id=54&Itemid=74

As mentioned above, it is recommended you install it into the path `Z:\ZDS`

> You should end up with the executable files within the directory `Z:\ZDS\bin\`

This is a very old tool set.  But it does allow for on-chip debugging.

### Hardware Programming Tool

There are 2 options for loading the firmware onto the eZ80 CPU.
1. Use the proprietary programmer from Zilog, the *Zilog eZ80 Acclaim USB Smart Cable*.
2. Or use, the still experimental, Pi Pico Programmer. See associated project in this repo: [Readme](../programmer/readme.md)

The *Zilog eZ80 Acclaim USB Smart Cable* can be purchased from suppliers such as digikey and mouser.  In addition to allowing for flashing of the on-chip ROM, the Smart Cable also allows for on-chip debugging with the ZDSII IDE.

* Digikey link: https://www.digikey.com.au/en/products/detail/zilog/ZUSBASC0200ZACG/17374332
* Mouser link: https://au.mouser.com/ProductDetail/692-ZUSBASC0200ZACG

### Programming/Flashing

The eZ80 CPU module features a 6-pin header for connecting the Zilog programmer's IDC connector. It is important to note that this header is not keyed, which means the connector can be attached in the incorrect orientation.

The silk screen on the PCB indicates the correct orientation for the IDC connector.

Refer to the following images for the correct installation orientation.

<div style="display: flex; justify-content: space-evenly;">
  <img src="../docs/assets/eZ80-programmer-orientation.jpg" alt="eZ80 on RC2014" style="width: 35%;">
  <img src="../docs/assets/eZ80-programmer-installed.jpg" alt="eZ80 on RC2014" style="width: 35%;">
</div>

<br/>
<br/>

> Incorrect orientation may result in damage to the CPU and or the programmer.  I have yet to do it to find out what happens!

### Memory Map

| Address Range    | Description |
| ---------------- | ----------- |
| 000000 -> 01FFFF | ON-CHIP ROM |
| 02E000 -> 02FFFF | ON-CHIP RAM (__RAM_ADDR_U_INIT_PARAM) |
| 030000 -> 03FFFF | EXTERNAL RAM (CS3) |
| 040000 -> 23FFFF | WIP EXTERNAL FLAT RAM (CS0) 2MB |
| 240000 -> FFFFFF | WIP OTHER (CS1) 14MB |

### Firmware Segments

#### ROM Segments
| Name | Description |
| ----  | --- |
| .RESET   | Hard reset/power on startup @ 0x000000 |
| .IVECTS  | Interrupt jump vectors @ 0x000100 |
| .STARTUP | C Runtime Startup code |
| CODE     | Main C/Assembly code |
| TEXT | read only data/strings |
| STRSECT | read only data/strings |
| DATA | Copied for write access to RAM on startup |
| INTERNAL_HIRAM_RAM |

#### RAM Segments/sequence

RAM is assigned top down.

| Name | Location | Description |
| ----  | --- | --- |
| INTERNAL_HIRAM_RAM | @ 0x02FF00 | Reserved firmware jump vectors |
| BSS                | @ (BASE OF INTERNAL_HIRAM_RAM) - $100 | C uninitialised variables. Zeroed on startup. |
| DATA               | @ (BASE OF BSS) - $10 | C initialised variables |
| INTERNAL_RAM_ROM -> INTERNAL_RAM_RAM   | @ BASE OF DATA - $630 | firmware code to be copied to and run from RAM |
| SPL_BASE           | BASE OF INTERNAL_RAM_RAM | Stack Top |


### I/O Addressing
| Address Range    | Description |
| ---------------- | ----------- |
| 0000 -> 00FF     | Reserved ON-CHIP IO |
| 0100 -> FEFF     | Unassigned |
| FF00 -> FFFF		 | External 8-Bit IO (CS2) |


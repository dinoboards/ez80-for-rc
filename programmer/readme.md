# Pi Pico Programmer

Use a Pi Pico to programme/flash the eZ80 CPU.  Designed for the **eZ80 for RCBus** solution - but may work for other eZ80 solution that have access to the ZDI interface (although I have only tested with my **eZ80 for RCBus**).

## Pre-compiled images

Precompiled images are available at:

* the 'official' releases at [https://github.com/dinoboards/ez80-for-rc/releases](https://github.com/dinoboards/ez80-for-rc/releases)
* the latest releases at [https://github.com/dinoboards/ez80-for-rc/actions](https://github.com/dinoboards/ez80-for-rc/actions)

## Building the code

Under linux, you can use a pre-built docker image that contains the required Raspberry Pi Pico tools and SDK:

Within the `programmer` directory, run this:

```
docker run -v ${PWD}:/programmer/ -u $(id -u ${USER}):$(id -g ${USER}) --privileged=true -it dinoboards/pipico:0.0.2 /bin/bash -c "mkdir -p build && cd build && cmake .. && make"
```

You can also use VS Code, with the official `Raspberry Pi Pico` VS Code extension [https://github.com/raspberrypi/pico-vscode](https://github.com/raspberrypi/pico-vscode)

A successful build will create a file called `programmer.uf2` in the `programmer\build` directory

## Updating your Pi Pico

1. Press and hold the BOOTSEL button on the Pico
2. While holding BOOTSEL, plug the USB cable from the Pico into your PC
3. The Pico should appear as a Mass Storage Device on your computer - look for a new drive letter.
4. Copy the `programmer.uf2` to the root of the mounted storage device.
5. After copying, the device should reboot
6. Confirm you can connect on serial over USB, using your favourite serial terminal application (such as PuTTY).  You will need to find the COMx number that the USB has remounted as.

## Wiring the Pi Pico to your eZ80 ZDI interface

| Pi Pico | ZDI Interface   |
|---------|-----------------|
| GPIO 15 | ZCL             |
| GPIO 14 | ZDA             |
| GPIO 16 | RESET           |
| GPIO 17 | ZDI's PWR (3V3) |
| GND *   | GND *           |

\* Make sure you have short leads, and that the 2 ground pins of the ZDI are connected to ground pins of the Pi Pico


## Operating instructions

Within your serial connection to the Pi Pico Programmer, you can issue a number of commands.

`help` Will show the list of commands available

More details coming soon...

# ![MTspin logo](images/mtspin%20red%20and%20white%20200%20x%2046%20png%20transparent.png)

## MTspin-mcu-firmware

[![Build, test and release](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml/badge.svg)](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml)

Microcontroller firmware for MTspin; a motorised rotating display stand.

## Introduction

### Microcontroller

Arduino UNO is the primary target for this project.

### Programming languages and frameworks

The primary language for this project is the [Arduino programming language](https://www.arduino.cc/reference) (a subset of C/C++).

### Build system and programming environment/toolchain

This project doesn't impose any specific build system, however the preferred method is to make use of the tools provided by the Arduino eco-system such as [Arduino CLI](https://arduino.github.io/arduino-cli) and/or the [Arduino IDE](https://www.arduino.cc/en/software). See the "Setup and build" section for more details.

### Continuous integration/delivery (CI/CD)

[GitHub Actions](https://docs.github.com/en/actions) is used as the CI/CD platform, and the workflow also makes use of the Linux setup script.

### External libraries

The following libraries (available via the [Arduino library manager](https://www.arduino.cc/reference/en/libraries/)) are used within the project:

- [MT-arduino-momentary-button](https://github.com/Morgritech/MT-arduino-momentary-button)
- [MT-arduino-stepper-driver](https://github.com/Morgritech/MT-arduino-stepper-driver)
- [Arduino-Log](https://github.com/thijse/Arduino-Log/)

## Setup and build

As with any other Arduino project, you can simply download the project and open the sketch folder ([src](src)) in the Arduino IDE.

You can manually install the required libraries via the built-in library manager in the Arduino IDE,
OR,
you can run the setup scripts to automatically install the required libraries as described in the following sections.

Building/compiling and uploading the project can be done via the relevant options in the IDE.

> [!NOTE]
> Running the setup scripts will install arduino-cli and other dependencies (Arduino cores and libraries) on your device.

### Setup and build on Windows

**Setup a Windows device ready to build the project.**

Install arduino-cli:

``` shell
scripts\setup-build-windows.cmd -cli
```

Install arduino-cli and add it to the Windows environment path:

``` shell
scripts\setup-build-windows.cmd -cli --path
```

Install arduino cores and libraries:

``` shell
scripts\setup-build-windows.cmd -deps
```

**Build and optionally upload the project.**

Build only:

``` shell
scripts\setup-build-windows.cmd -build
```

Build and upload:

``` shell
scripts\setup-build-windows.cmd -build --port COM3 --upload
```

Replace COM3 in the command with the desired serial port.

### Setup and build on Linux

**Setup a Linux device ready to build the project.**

Install arduino-cli:

``` shell
scripts/setup-build-linux.sh -cli
```

The script does not provide an option to add arduino-cli to the Linux environment path.

Install arduino cores and libraries:

``` shell
scripts/setup-build-linux.sh -deps
```

**Build and optionally upload the project.**

Build only:

``` shell
scripts/setup-build-linux.sh -build
```

Build and upload:

``` shell
scripts/setup-build-linux.sh -build --port /dev/ttyACM0 --upload
```

Replace /dev/ttyACM0 in the command with the desired serial port.

### Running arduino-cli directly (Windows or Linux)

Once arduino-cli is installed as described above, the commands can be used directly in the terminal. This can be useful if more functionality is required, beyond what the setup and build scripts provide. See the official [Arduino CLI](https://arduino.github.io/arduino-cli) website for more information.

If you added arduino-cli to your devices environment path:

``` shell
arduino-cli <commands>
```

If you did not add arduino-cli to your devices environment path, the full path must be given with the command.

For windows:

``` shell
C:\Program Files\arduino-cli <commands>
```

For Linux:

``` shell
~/bin/arduino-cli <commands>
```

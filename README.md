# MTspin-mcu-firmware

[![Build, test and release](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml/badge.svg)](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml)

Microcontroller firmware for MTspin; a motorised rotating display stand.

## Introduction

### Microcontroller

Arduino UNO is the primary target for this project.

### Programming language

The primary language for this project is the [Arduino programming language](https://www.arduino.cc/reference) (a subset of C/C++).

### Build system and continuous integration/delivery (CI/CD)

This project uses [Arduino CLI](https://arduino.github.io/arduino-cli) to compile and upload the embedded code, and [CMake](https://cmake.org) to build and run the unit tests. Setup/build scripts for Linux and Windows have been created to automate the process as much as possible. [GitHub Actions](https://docs.github.com/en/actions) is used as the CI/CD platform, and the workflow also makes use of the Linux setup/build script.

## Setup and build

### Setup on Windows

To setup a Windows machine ready to build the project, run:

``` shell
scripts\setup-build-windows.cmd -setup
```

### Build on Windows

To build the project on Windows, run:

``` shell
scripts\setup-build-windows.cmd -build
```

### Setup and build together on Windows

To setup a Windows machine and immediately build the project, run:

``` shell
scripts\setup-build-windows.cmd -both
```

### Setup on Linux

To setup a Linux machine ready to build the project, run:

``` shell
source scripts/setup-build-linux.sh -setup
```

### Build on Linux

To build the project on Linux, run:

``` shell
scripts/setup-build-linux.sh -build
```

### Setup and build together on Linux

To setup a Linux machine and immediately build the project, run:

``` shell
source scripts/setup-build-linux.sh -both
```

## Additional information on using the scripts

Setup and build scripts can be found in the "scripts" directory.
Note:
The `[optional arduino-cli compile flags...]` in the usage depictions can be found at:

<https://arduino.github.io/arduino-cli>

These flags will only work when a build is performed i.e. with the `-build` or `-both` commands.

### Script usage on Windows

``` shell
scripts\<script_name>.cmd <command> [optional arduino-cli compile flags...]
```

### Script usage on Linux

``` shell
scripts/<script_name>.sh <command> [optional arduino-cli compile flags...]
```

If using the `-setup` or `-both` commands, you should source the script, to allow arduino-cli commands to be directly executable in the terminal.

``` shell
. scripts/<script_name>.sh <command> [optional arduino-cli compile flags...]
```

OR

``` shell
source scripts/<script_name>.sh <command> [optional arduino-cli compile flags...]
```

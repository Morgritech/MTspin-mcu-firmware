# ![MTspin logo](images/mtspin%20red%20and%20white%20200%20x%2046%20png%20transparent.png)

## MTspin-mcu-firmware

[![Build, test and release](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml/badge.svg)](https://github.com/Morgritech/MTspin-mcu-firmware/actions/workflows/build-test-release.yaml)

Microcontroller firmware for MTspin; a motorised rotating display stand.

## Introduction

### Microcontroller

Arduino UNO is the primary target for this project.

### Programming languages and frameworks

The primary language for this project is the [Arduino programming language](https://www.arduino.cc/reference) (a subset of C/C++).

Unit tests are developed using the [GoogleTest](http://google.github.io/googletest/) framework.

### Build system and continuous integration/delivery (CI/CD)

This project uses [CMake](https://cmake.org) to build and run the project. Under the hood, [Arduino CLI](https://arduino.github.io/arduino-cli) is used to compile and upload the embedded code. Setup scripts for Windows and Linux have been created to automate the process of installing system dependencies, except for a C++ programming toolchain on Windows (required for building the unit tests), which must be manually installed. CMake and Arduino CLI are used to automate the process of obtaining other project dependencies.

[GitHub Actions](https://docs.github.com/en/actions) is used as the CI/CD platform, and the workflow also makes use of the Linux setup script.

### Programming environment/toolchain

Arduino CLI contains all the tools required to build the embedded code. For the unit tests, [MinGW](https://en.wikipedia.org/wiki/MinGW) for Windows and the [GNU toolchain](https://en.wikipedia.org/wiki/GNU_toolchain) for Linux are the defaults set by the CMakePresets.json file. However, these can be overridden by creating a CMakeUserPresets.json file in the root folder with settings for your preferred environment.

## Setup and build

> [!NOTE]
> Running the setup scripts will install Arduino CLI, CMake and other dependencies on your device.

### Setup on Windows

To setup a Windows device ready to build the project, run:

``` shell
scripts\setup-windows.cmd
```

### Setup on Linux

To setup a Linux device ready to build the project, run:

``` shell
scripts/setup-linux.sh
```

### Build on Windows or Linux

To build the project (Windows or Linux), run:

``` shell
TBC..
```

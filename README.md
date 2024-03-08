# MTspin-mcu-firmware

Microcontroller firmware for MTspin; a motorised rotating display stand.

## Setup and build

### Setup on Windows

To setup a windows machine ready to build the project, run:

``` shell
scripts\setup_for_windows.cmd
```

### Build on Windows

To build the project on Windows, run:

``` shell
scripts\build_for_windows.cmd
```

### Setup on Linux

To setup a linux machine ready to build the project, run:

``` bash
. scripts/setup-build-linux.sh -setup
```

OR:

``` bash
source scripts/setup-build-linux.sh -setup
```

You may need to first run:

``` bash
chmod +x scripts/setup-build-linux.sh
```

### Build on Linux

To build the project on Linux, run:

``` bash
scripts/setup-build-linux.sh -build
```

You may need to first run:

``` bash
chmod +x scripts/setup-build-linux.sh
```

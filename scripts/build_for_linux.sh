#!/bin/bash

# Specify the sketch directory.
SKETCH_DIR=src #./src
# Specify the build directory.
BUILD_DIR=build
# Specify the directories to files listing the required arduino boards.
BOARDS=arduino_boards.txt
# Build for multiple boards.
IFS=$'\n' # Use new line char as the line break delimiter (incase there are spaces or tabs).
# Carriage return '\r' must be removed in-case file was created in Windows.
for BOARD in $(cat "$BOARDS") ; do
    arduino-cli compile -b ${BOARD%$'\r'} $SKETCH_DIR --build-path $BUILD_DIR
done
unset IFS

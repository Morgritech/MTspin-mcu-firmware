#!/bin/bash

# Specify the sketch directory.
SKETCH_DIR=src #./src
# Specify the build directory.
BUILD_DIR=build
# Specify the directories to files listing the required arduino boards.
BOARDS=arduino_boards.txt
# Build for multiple boards.
IFS=$'\n' # Read one line at a time (No spaces or tabs as delimiters).
for BOARD in $(cat "$BOARDS") ; do
    # Replace ":" with "-" to create the build sub-folder with the board name.
    BUILD_SUBDIR="${BOARD//:/-}"
    # Compile clean (no cache) and use command line args if passed to the script.
    # Carriage return '\r' must be removed in-case file was created in Windows.
    arduino-cli compile -b ${BOARD%$'\r'} $SKETCH_DIR --clean --build-path $BUILD_DIR/$BUILD_SUBDIR ${@:2}
done
unset IFS

echo command line args in build: 
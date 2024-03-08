#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e 

# Check sudo
#if [ "$EUID" -ne 0 ]
#  then echo "Please run as root."
#  exit
#fi

# Command line arguments
CMD_SETUP="-setup"
CMD_BUILD="-build"
CMD_BOTH="-both"

if [ "$1" == "$CMD_SETUP" ] || [ "$1" == "$CMD_BOTH" ]; then

  echo Install and setup dependencies

  pushd $PWD
  cd ~ # Ensure we are in the "home/<username>" directory.
  sudo apt-get --no-install-recommends install -y curl
  curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
  popd
  # If not already added; 
  # add "home/<username>/bin" to linux environment path (this and child scripts) so arduino-cli be executed directly.
  if [ -z "$ARDUINO_CLI_DIR" ]; then
    ARDUINO_CLI_DIR=~/bin
    PATH=$PATH:$ARDUINO_CLI_DIR
    arduino-cli version
  fi

  echo Install arduino platforms/cores

  # Specify the directories to files listing the required arduino cores and libraries.
  CORES=arduino_cores.txt
  LIBS=arduino_libs.txt

  arduino-cli core update-index
  IFS=$'\n' # Read one line at a time (No spaces or tabs as delimiters).
  # Carriage return '\r' must also be removed in-case file was created in Windows.
  for CORE in $(cat $CORES) ; do
    arduino-cli core install ${CORE%$'\r'}
  done

  echo Install arduino libraries

  for LIB in $(cat $LIBS) ; do
    arduino-cli lib install ${LIB%$'\r'}
  done
  unset IFS
fi

if [ "$1" == "$CMD_BUILD" ] || [ "$1" == "$CMD_BOTH" ]; then

  echo Compile the project

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
      # Compile clean (no cache) and use command line args (2nd onwards) if passed to the script.
      # Carriage return '\r' must be removed in-case file was created in Windows.
      arduino-cli compile -b ${BOARD%$'\r'} $SKETCH_DIR --clean --build-path $BUILD_DIR/$BUILD_SUBDIR ${@:2}
  done
  unset IFS
fi

if [ "$1" != "$CMD_SETUP" ] && [ "$1" != "$CMD_BUILD" ] && [ "$1" != "$CMD_BOTH" ]; then
  echo \
  "Invalid input! Options are:
  $CMD_SETUP  Setup the project. 
  $CMD_BUILD  Build the project. 
  $CMD_BOTH   Setup and build the project."
fi
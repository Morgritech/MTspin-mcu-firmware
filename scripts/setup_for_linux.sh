#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
# set -e 

# Check sudo
#if [ "$EUID" -ne 0 ]
#  then echo "Please run as root."
#  exit
#fi

# Install and setup dependencies

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

# Specify the directories to files listing the required arduino cores and libraries.
CORES=arduino_cores.txt
LIBS=arduino_libs.txt

# Install arduino platforms/cores.
arduino-cli core update-index
IFS=$'\n' # Read one line at a time (No spaces or tabs as delimiters).
# Carriage return '\r' must also be removed in-case file was created in Windows.
for CORE in $(cat $CORES) ; do
  arduino-cli core install ${CORE%$'\r'}
done

# Install arduino libraries.
for LIB in $(cat $LIBS) ; do
  arduino-cli lib install ${LIB%$'\r'}
done
unset IFS

#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e 

# Check sudo
#if [ "$EUID" -ne 0 ]
#  then echo "Please run as root."
#  exit
#fi

echo ...Installing and setting up project dependencies...

pushd $PWD
cd ~ # Ensure we are in the "home/<username>" directory.
sudo snap install cmake --classic
sudo apt-get --no-install-recommends install -y curl doxygen graphviz cloc #default-jre
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/bin sh
popd

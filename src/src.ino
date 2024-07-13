// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Main sketch containing the setup() and loop() functions.
/// @file src.ino

#include "src_hardware_config.h"

#include "FTDebouncer.h"
#include "TestLib.h"

FTDebouncer pinDebouncer;

int baudRate = 9600;

int minStartupTime = 1000;

/// @brief The main application entry point for initialisation tasks.
void setup(){

  mtSerial.begin(baudRate); //Begin serial.

  while (!mtSerial); //Wait until a serial connection is made (This is mainly for the arduino due, other boards do this automatically).

  //DEBUGGING.
  mtSerial.println(F("...\n...Setup Start...\n..."));

  // Initialise input pins

  pinMode(kDirectionButtonPin, INPUT);
  pinMode(kSpeedButtonPin, INPUT);
  pinMode(kAngleButtonPin, INPUT);  

  // Initialise output pins

  pinMode(kPulMotorDriverPin, OUTPUT);
  pinMode(kDirMotorDriverPin, OUTPUT);
  pinMode(kEnaMotorDriverPin, OUTPUT);

  delay(minStartupTime); //Delay for the startup/boot time required by motor controllers.

  mtSerial.println(mt::TestLib::testDoubleNumber(2));

  //DEBUGGING.
  mtSerial.println(F("...\n...Setup End...\n..."));
}

/// @brief The continuously running function for repetitive tasks.
void loop(){

  //DEBUGGING.
  mtSerial.println(F("...\n...Loop Start...\n..."));

  //TESTING.
  delay(500);

  //DEBUGGING.
  mtSerial.println(F("...\n...Loop End...\n..."));
}

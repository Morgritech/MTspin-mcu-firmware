// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file src.ino
/// @brief Main sketch containing the setup() and loop() functions.

#include "hardware_config.h"

#include "control_system.h"

#include "FTDebouncer.h"
#include "TestLib.h"

FTDebouncer pinDebouncer;

/// @brief The Control System instance.
mtspin::ControlSystem control_system;

/// @brief The serial communication speed for the Arduino board.
const int kBaudRate = 9600;

int minStartupTime = 1000;

/// @brief The main application entry point for initialisation tasks.
void setup(){

  // Initialise the Serial Port.
  MTSPIN_SERIAL.begin(kBaudRate);

  // Wait until a serial connection is made (This is mainly for the arduino due, other boards do this automatically).
  while (!MTSPIN_SERIAL);

  //DEBUGGING.
  MTSPIN_SERIAL_LOGLN(F("...\n...Setup Start...\n..."));

  // Initialise the input pins
  pinMode(kDirectionButtonPin, INPUT);
  pinMode(kSpeedButtonPin, INPUT);
  pinMode(kAngleButtonPin, INPUT);

  // Initialise the output pins
  pinMode(kPulMotorDriverPin, OUTPUT);
  pinMode(kDirMotorDriverPin, OUTPUT);
  pinMode(kEnaMotorDriverPin, OUTPUT);

  // Delay for the startup/boot time required by motor controllers.
  delay(minStartupTime);

  MTSPIN_SERIAL.println(mt::TestLib::testDoubleNumber(2));

  MTSPIN_SERIAL_LOGLN(F("...\n...Setup End...\n..."));
}

/// @brief The continuously running function for repetitive tasks.
void loop(){

//  MTSPIN_SERIAL_LOGLN(F("...\n...Loop Start...\n..."));

  //TESTING.
  //delay(500);

  // Start the control system.
  control_system.CheckAndProcess();

  //if (digitalRead(2) == HIGH) {
  //  MTSPIN_SERIAL_LOGLN(F("BUTTON PRESSED"));
  //}

//  MTSPIN_SERIAL_LOGLN(F("...\n...Loop End...\n..."));
}

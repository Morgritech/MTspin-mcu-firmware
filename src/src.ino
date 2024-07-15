// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Main sketch containing the setup() and loop() functions.
/// @file src.ino

#include "hardware_config.h"

#include "control_system.h"

#include "FTDebouncer.h"
#include "TestLib.h"

FTDebouncer pinDebouncer;

/// @brief The control system instance.
mtspin::ControlSystem control_system;

int baudRate = 9600;

int minStartupTime = 1000;

/// @brief The main application entry point for initialisation tasks.
void setup(){

  MTSPIN_SERIAL.begin(baudRate); // Begin serial.

  while (!MTSPIN_SERIAL); // Wait until a serial connection is made (This is mainly for the arduino due, other boards do this automatically).

  //DEBUGGING.
  MTSPIN_SERIAL_LOGLN(F("...\n...Setup Start...\n..."));

  // Initialise input pins

  pinMode(kDirectionButtonPin, INPUT);
  pinMode(kSpeedButtonPin, INPUT);
  pinMode(kAngleButtonPin, INPUT);

  // Initialise output pins

  pinMode(kPulMotorDriverPin, OUTPUT);
  pinMode(kDirMotorDriverPin, OUTPUT);
  pinMode(kEnaMotorDriverPin, OUTPUT);

  delay(minStartupTime); // Delay for the startup/boot time required by motor controllers.

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

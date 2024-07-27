// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file src.ino
/// @brief Main sketch containing the setup() and loop() functions.

#include <ArduinoLog.h>

#include "configuration.h"
#include "control_system.h"
#include "TestLib.h"

/// @brief The Control System instance.
mtspin::ControlSystem control_system;

/// @brief The main application entry point for initialisation tasks.
void setup() {

  // Initialise the hardware (Serial port, logging, pins, etc.).
  mtspin::configuration::BeginHardware();
  
  Log.noticeln(F("\n...Setup complete...\n"));
}

/// @brief The continuously running function for repetitive tasks.
void loop() {

  //TESTING.
  //delay(500);

  // Start the control system.
  control_system.CheckAndProcess();

  //if (digitalRead(2) == HIGH) {
  //  Log.noticeln(F("BUTTON PRESSED"));
  //}
}

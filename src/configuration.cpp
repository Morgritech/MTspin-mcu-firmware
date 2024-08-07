// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.cpp
/// @brief Common configuration settings, including serial port and pin definitions, etc.

#include "configuration.h"

#include <ArduinoLog.h>

namespace mtspin {

namespace configuration {

void BeginHardware() {
  // Initialise the serial port.
  MTSPIN_SERIAL.begin(kBaudRate);

  // Wait until a serial connection is made.
  // This is mainly for the arduino due, other boards do this automatically.
  while (!MTSPIN_SERIAL);

  // Initialise logging.
  Log.begin(kDefaultLogLevel, &MTSPIN_SERIAL);

  // Initialise the input pins.
  pinMode(kDirectionButtonPin, INPUT);
  pinMode(kSpeedButtonPin, INPUT);
  pinMode(kAngleButtonPin, INPUT);

  // Initialise the output pins
  pinMode(kPulMotorDriverPin, OUTPUT);
  pinMode(kDirMotorDriverPin, OUTPUT);
  pinMode(kEnaMotorDriverPin, OUTPUT);

  // Delay for the startup time required by stepper driver.
  delay(kMinStartupTime_ms);
}

} // namespace configuration

} // namespace mtspin
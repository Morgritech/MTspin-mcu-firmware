// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.cpp
/// @brief Class to setup common configuration settings, including serial port and pin definitions, etc.

#include "configuration.h"

#include <ArduinoLog.h>

namespace mtspin {

Configuration& Configuration::GetInstance() {
  static Configuration instance;
  return instance;
}

void Configuration::BeginHardware() const {
  // Initialise the serial port.
  MTSPIN_SERIAL.begin(kBaudRate);

  // Wait until a serial connection is made if debugging is enabled.
  // This is mainly for the arduino due, other boards do this automatically.
  if (debug_enabled_ == true) {
    while (!MTSPIN_SERIAL);
  }

  // Initialise logging.
  Log.begin(kDefaultLogLevel, &MTSPIN_SERIAL);

  // Initialise the input pins.
  pinMode(kDirectionButtonPin, INPUT);
  pinMode(kAngleButtonPin, INPUT);
  pinMode(kSpeedButtonPin, INPUT);

  // Initialise the output pins.
  pinMode(kPulPin, OUTPUT);
  pinMode(kDirPin, OUTPUT);
  pinMode(kEnaPin, OUTPUT);

  // Delay for the startup time.
  delay(kStartupTime_ms);
}

Configuration::Configuration() {}

Configuration::~Configuration() {}

} // namespace mtspin
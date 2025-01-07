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
  MTSPIN_SERIAL.begin(kBaudRate_);

  // Initialise logging.
  Log.begin(log_level_, &MTSPIN_SERIAL);

  // Initialise the input pins.
  pinMode(kDirectionButtonPin_, INPUT);
  pinMode(kAngleButtonPin_, INPUT);
  pinMode(kSpeedButtonPin_, INPUT);

  // Initialise the output pins.
  pinMode(kPulPin_, OUTPUT);
  pinMode(kDirPin_, OUTPUT);
  pinMode(kEnaPin_, OUTPUT);

  // Delay for the startup time.
  delay(kStartupTime_ms_);
}

void Configuration::ToggleLogs() {
  // Toggle log messages.
  if (log_level_ == LOG_LEVEL_SILENT) {
    log_level_ = LOG_LEVEL_VERBOSE;
  }
  else {
    Log.noticeln(F("Log messages disabled"));
    log_level_ = LOG_LEVEL_SILENT;
  }

  Log.begin(log_level_, &MTSPIN_SERIAL);
  if (log_level_ == LOG_LEVEL_VERBOSE) Log.noticeln(F("Log messages enabled"));
}

void Configuration::ReportFirmwareVersion() {
  String version = kName;
  version += F("-");
  version += String(kMajor);
  version += F(".");
  version += String(kMinor);
  version += F(".");
  version += String(kPatch);
  if (strlen(kSuffix) > 0) version += F("-");
  version += kSuffix;
  MTSPIN_SERIAL.println(version);
}

Configuration::Configuration() {}

Configuration::~Configuration() {}

} // namespace mtspin
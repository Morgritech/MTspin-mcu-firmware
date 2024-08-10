// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.h
/// @brief Common configuration settings, including serial port and pin definitions, etc.

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>
#include <ArduinoLog.h>

/// @brief Macro to define Serial port
#ifndef MTSPIN_SERIAL
#define MTSPIN_SERIAL Serial // "Serial" for programming port, "SerialUSB" for native port (Due and Zero only).
#endif

namespace mtspin {

namespace configuration {

/// @brief Initialise the hardware (Serial port, logging, pins, etc.).
void BeginHardware();

//GPIO pins

//Notes

//Interrupt pins:
//Uno, Nano, Mini, other 328-based   2, 3
//Mega, Mega2560, MegaADK            2, 3, 18, 19, 20, 21
//Due                                all digital pins

/// @{
/// @brief Button input pins.
inline constexpr uint8_t kDirectionButtonPin = 2; ///< Input pin for the button controlling motor direction.
inline constexpr uint8_t kSpeedButtonPin = 3; ///< Input pin for the button controlling motor speed.
inline constexpr uint8_t kAngleButtonPin = 4; ///< Input pin for the button controlling motor angle.
/// @}

/// @{
/// @brief Stepper driver output pins.
inline constexpr uint8_t kPulMotorDriverPin = 11; ///< Output pin for the stepper driver PUL/STP/CLK (pulse/step) interface.
inline constexpr uint8_t kDirMotorDriverPin = 12; ///< Output pin for the stepper driver DIR/CW (direction) interface.
inline constexpr uint8_t kEnaMotorDriverPin = 13; ///< Output pin for the stepper driver ENA/EN (enable) interface.
/// @}

// Serial

/// @brief The serial communication speed.
inline constexpr int kBaudRate = 9600;

// Logging

/// @brief The default log level for debugging and system reporting.
inline constexpr int kDefaultLogLevel =  LOG_LEVEL_VERBOSE;

// Hardware properties/characteristics

/// @brief Minimum startup/boot time in milliseconds (ms) for the stepper driver.
inline constexpr uint16_t kMinStartupTime_ms = 1000;

} // namespace configuration

} // namespace mtspin

#endif // CONFIGURATION_H_
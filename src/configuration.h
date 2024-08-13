// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.h
/// @brief Class to setup common configuration settings, including serial port and pin definitions, etc.

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>
#include <ArduinoLog.h>
#include <momentary_button.h>

#include "stepper_driver.h"

/// @brief Macro to define Serial port
#ifndef MTSPIN_SERIAL
#define MTSPIN_SERIAL Serial // "Serial" for programming port, "SerialUSB" for native port (Due and Zero only).
#endif

namespace mtspin {

/// @brief The Configuration class using the singleton pattern i.e., only one single instance can exist.
struct Configuration {

  /// @brief Static method to get the single instance.
  /// @return The Configuration instance. 
  static Configuration& GetInstance();

  /// @{
  /// @brief Delete the copy constructor and assignment operator to prevent copying of the single instance.
  Configuration(const Configuration&) = delete;
  Configuration& operator=(const Configuration&) = delete;
  /// @}

  /// @brief Initialise the hardware (Serial port, logging, pins, etc.).
  void BeginHardware() const;

  // GPIO pins

  // Notes

  // Interrupt pins:
  // Uno, Nano, Mini, other 328-based   2, 3
  // Mega, Mega2560, MegaADK            2, 3, 18, 19, 20, 21
  // Due                                all digital pins

  /// @{
  /// @brief Button input pins.
  const uint8_t kDirectionButtonPin = 2; ///< Input pin for the button controlling motor direction.
  const uint8_t kSpeedButtonPin = 3; ///< Input pin for the button controlling motor speed.
  const uint8_t kAngleButtonPin = 4; ///< Input pin for the button controlling motor angle.
  /// @}

  /// @{
  /// @brief Stepper motor driver output pins.
  const uint8_t kPulStepperDriverPin = 11; ///< Output pin for the stepper driver PUL/STP/CLK (pulse/step) interface.
  const uint8_t kDirStepperDriverPin = 12; ///< Output pin for the stepper driver DIR/CW (direction) interface.
  const uint8_t kEnaStepperDriverPin = 13; ///< Output pin for the stepper driver ENA/EN (enable) interface.
  /// @}

  // Hardware properties/characteristics

  // Serial properties

  /// @brief The serial communication speed.
  const int kBaudRate = 9600;

  // Button properties

  /// @{
  /// @brief Unpressed button pin states.
  const mt::MomentaryButton::PinState kDirectionButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kSpeedButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kAngleButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  /// @}

  /// @{
  /// @brief Debounce periods.
  const uint16_t kDirectionButtonDebouncePeriod = 20;
  const uint16_t kSpeedButtonDebouncePeriod = 20;
  const uint16_t kAngleButtonDebouncePeriod = 20;
  /// @}

  /// @{
  /// @brief Short press periods.
  const uint16_t kDirectionButtonShortPressPeriod = 500;
  const uint16_t kSpeedButtonShortPressPeriod = 500;
  const uint16_t kAngleButtonShortPressPeriod = 500;
  /// @}

  /// @{
  /// @brief Long press periods.
  const uint16_t kDirectionButtonLongPressPeriod = 1000;
  const uint16_t kSpeedButtonLongPressPeriod = 1000;
  const uint16_t kAngleButtonLongPressPeriod = 1000;
  /// @}

  // Stepper motor properties

  /// @brief 
  const float kFullStepAngleDegrees = 1.8;

  /// @brief 
  const double kGearRatio = 1; 

  // Stepper driver properties

  /// @brief 
  const uint8_t kStepMode = 1;

  /// @brief Minimum startup/boot time in milliseconds (ms) for the stepper driver.
  const uint16_t kMinStartupTime_ms = 1000;

  // Other properties/characteristics

  // Logging

  /// @brief The default log level for debugging and system reporting.
  const int kDefaultLogLevel =  LOG_LEVEL_VERBOSE;  

 private:

  /// @{  
  /// @brief Private constructor and destructor so objects cannot be manually instantiated. 
  Configuration();
  ~Configuration();
  /// @}
};

} // namespace mtspin

#endif // CONFIGURATION_H_
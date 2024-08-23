// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.h
/// @brief Structure to setup common configuration settings, including serial port and pin definitions, etc.

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

/// @brief The Configuration structure using the singleton pattern i.e., only a single instance can exist.
struct Configuration {

  /// @brief Enum of motor speed settings.
  enum class SpeedSettings {
    kOne = 1,
    kTwo,
    kThree,
    kFour,
  };

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

  // Notes

  // Interrupt pins:
  // Uno, Nano, Mini, other 328-based   2, 3
  // Mega, Mega2560, MegaADK            2, 3, 18, 19, 20, 21
  // Due                                all digital pins

  /// @{
  /// @brief GPIO pins.
  /// Input pins for the buttons.
  const uint8_t kDirectionButtonPin = 2; ///< For the button controlling motor direction.
  const uint8_t kSpeedButtonPin = 3; ///< For the button controlling motor speed.
  const uint8_t kAngleButtonPin = 4; ///< For the button controlling motor angle.
  /// Output pins for the stepper motor driver.
  const uint8_t kPulPin = 11; ///< For the stepper driver PUL/STP/CLK (pulse/step) interface.
  const uint8_t kDirPin = 12; ///< For the stepper driver DIR/CW (direction) interface.
  const uint8_t kEnaPin = 13; ///< For the stepper driver ENA/EN (enable) interface.
  /// @}

  /// @brief Serial properties.
  const int kBaudRate = 9600; ///< The serial communication speed.

  /// @{
  /// @brief Button properties.
  /// Button unpressed pin states.
  const mt::MomentaryButton::PinState kDirectionButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kSpeedButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kAngleButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  /// Button debounce periods.
  const uint16_t kDirectionButtonDebouncePeriod = 20;
  const uint16_t kSpeedButtonDebouncePeriod = 20;
  const uint16_t kAngleButtonDebouncePeriod = 20;
  /// Button short press periods.
  const uint16_t kDirectionButtonShortPressPeriod = 500;
  const uint16_t kSpeedButtonShortPressPeriod = 500;
  const uint16_t kAngleButtonShortPressPeriod = 500;
  /// Button long press periods.
  const uint16_t kDirectionButtonLongPressPeriod = 1000;
  const uint16_t kSpeedButtonLongPressPeriod = 1000;
  const uint16_t kAngleButtonLongPressPeriod = 1000;
  /// @}

  /// @{
  /// @brief Stepper motor/drive system properties. 
  const float kFullStepAngleDegrees = 1.8F; ///< The full step angle in degrees.
  const double kGearRatio = 1.0; ///< The gear ratio.
  /// @}

  /// @{
  /// @brief Stepper driver properties.
  const uint8_t kStepMode = 1; ///< Micro-stepping/step mode.
  /// Minimum time (us) to delay after changing the state of a pin.
  const float kPulDelay_us = 2.5F; ///< For the PUL pin.
  const float kDirDelay_us = 5.0F; ///< For the Dir pin.
  const float kEnaDelay_us = 5.0F; ///< For the Ena pin.
  /// Speed and acceleration.
  const double kMinSpeed_RPM = 3.0; ///< Minimum rotation speed (RPM).
  const double kDefaultSpeed_RPM = 2.0 * kMinSpeed_RPM; ///< Default rotation speed (RPM).
  const double kAcceleration_rads_per_s_per_s = 0.0; ///< Acceleration (Radians per second-squared).
  /// @}

  /// @brief Logger properties (for debugging and system reporting).
  const int kDefaultLogLevel =  LOG_LEVEL_VERBOSE; ///< The default log level.

  /// @{
  /// @brief Other properties.
  const uint16_t kStartupTime_ms = 1000; ///< Minimum startup/boot time in milliseconds (ms); based on the stepper driver.
  /// @}

 private:

  /// @{  
  /// @brief Private constructor and destructor so objects cannot be manually instantiated. 
  Configuration();
  ~Configuration();
  /// @}
};

} // namespace mtspin

#endif // CONFIGURATION_H_
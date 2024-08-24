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

  /// @brief Enum of control system modes.
  enum class ControlMode {
    kContinuous = 1,
    kOscillate,
  };

  enum class SerialMessages {
    kToggleMotionMessage = 'm',

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

  /// @{
  /// @brief Control system properties
  const ControlMode kDefaultControlMode = ControlMode::kContinuous; ///< The default/initial control mode. 
  /// @}

  /// @{
  /// @brief Serial properties.
  const int kBaudRate = 9600; ///< The serial communication speed.
  /// Serial messages.
  static const char kToggleMotionMessage = 'm'; ///< Toggle (start/stop) the motor.
  static const char kToggleDirectionMessage = 'd'; ///< Change to full rotation mode or change motor direction.
  static const char kCycleSpeedMessage = 's'; ///< Cycle through motor speed settings.
  static const char kCycleAngleMessage = 'a'; ///< Change to sweep angle mode or cycle through sweep angles.
  /// @}

  /// @{
  /// @brief Button properties.
  /// Button unpressed pin states.
  const mt::MomentaryButton::PinState kDirectionButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kSpeedButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  const mt::MomentaryButton::PinState kAngleButtonUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  /// Button debounce periods (ms).
  const uint16_t kDirectionButtonDebouncePeriod_ms = 20;
  const uint16_t kSpeedButtonDebouncePeriod_ms = 20;
  const uint16_t kAngleButtonDebouncePeriod_ms = 20;
  /// Button short press periods (ms).
  const uint16_t kDirectionButtonShortPressPeriod_ms = 500;
  const uint16_t kSpeedButtonShortPressPeriod_ms = 500;
  const uint16_t kAngleButtonShortPressPeriod_ms = 500;
  /// Button long press periods (ms).
  const uint16_t kDirectionButtonLongPressPeriod_ms = 1000;
  const uint16_t kSpeedButtonLongPressPeriod_ms = 1000;
  const uint16_t kAngleButtonLongPressPeriod_ms = 1000;
  /// @}

  /// @{
  /// @brief Stepper motor/drive system properties.
  const float kFullStepAngleDegrees = 1.8F; ///< The full step angle in degrees.
  const double kGearRatio = 1.0; ///< The gear ratio.
  /// @}

  /// @{
  /// @brief Stepper driver properties.
  const uint8_t kMicrostepMode = 16; ///< Microstep mode.
  /// Minimum time (us) to delay after changing the state of a pin.
  const float kPulDelay_us = 2.5F; ///< For the PUL pin.
  const float kDirDelay_us = 5.0F; ///< For the Dir pin.
  const float kEnaDelay_us = 5.0F; ///< For the Ena pin.
  /// Motion direction during continuous operation.
  const mt::StepperDriver::MotionDirection kDefaultMotionDirection = mt::StepperDriver::MotionDirection::kPositive; // Clockwise (CW).
  /// Sweep angle during oscillation.
  static const uint8_t kSizeOfSweepAngles = 4; // No. of sweep angles in the lookup table.
  const float kSweepAnglesDegrees[kSizeOfSweepAngles] = {45.0F, 90.0F, 180.0F, 360.0F}; // Lookup table for sweep angles (degrees).
  const uint8_t kDefaultSweepAngleIndex = 0; // Index of initial/default sweep angle, i.e., 45 degrees.
  /// Speed and acceleration.
  static const uint8_t kSizeOfSpeeds = 4; // No. of speeds in the lookup table.
  const float kSpeedsRPM[kSizeOfSpeeds] = {3.0F, 6.0F, 12.0F, 24.0F}; // Lookup table for rotation speeds (RPM).
  const uint8_t kDefaultSpeedIndex = 1; // Index of initial/default sweep angle, i.e., 6 RPM.
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
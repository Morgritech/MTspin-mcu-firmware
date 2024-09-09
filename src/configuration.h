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

/// @brief The Configuration class using the singleton pattern i.e., only a single instance can exist.
class Configuration {
 public:

  /// @brief Enum of control system modes.
  enum class ControlMode {
    kContinuous = 1,
    kOscillate,
  };

  /// @brief Enum of control actions.
  enum class ControlAction {
    kToggleDirection = 'd',
    kCycleAngle = 'a',
    kCycleSpeed = 's',
    kToggleMotion = 'm',
    kIdle = '0',
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

  /// @{
  /// @brief GPIO pins.
  /// Input pins for the buttons.
  const uint16_t kDirectionButtonPin = 2; ///< For the button controlling motor direction.
  const uint16_t kAngleButtonPin = 3; ///< For the button controlling motor angle.
  const uint16_t kSpeedButtonPin = 4; ///< For the button controlling motor speed.
  /// Output pins for the stepper motor driver.
  const uint16_t kPulPin = 11; ///< For the stepper driver PUL/STP/CLK (pulse/step) interface.
  const uint16_t kDirPin = 12; ///< For the stepper driver DIR/CW (direction) interface.
  const uint16_t kEnaPin = 13; ///< For the stepper driver ENA/EN (enable) interface.
  /// @}

  /// @{
  /// @brief Control system properties.
  const ControlMode kDefaultControlMode = ControlMode::kOscillate; //ControlMode::kContinuous; ///< The default/initial control mode. 
  /// @}

  /// @brief Serial properties.
  const int kBaudRate = 9600; ///< The serial communication speed.

  /// @{
  /// @brief Button properties.
  /// Button unpressed pin states.
  const mt::MomentaryButton::PinState kUnpressedPinState = mt::MomentaryButton::PinState::kLow;
  /// Button debounce periods (ms).
  const uint16_t kDebouncePeriod_ms = 20;
  /// Button short press periods (ms).
  const uint16_t kShortPressPeriod_ms = 500;
  /// Button long press periods (ms).
  const uint16_t kLongPressPeriod_ms = 1000;
  /// Button long press options.
  const mt::MomentaryButton::LongPressOption kLongPressOption = mt::MomentaryButton::LongPressOption::kDetectWhileHolding;
  /// @}

  /// @{
  /// @brief Stepper motor/drive system properties.
  const float kFullStepAngle_degrees = 1.8F; ///< The full step angle in degrees.
  const float kGearRatio = 1.0F; ///< The gear ratio.
  /// @}

  /// @{
  /// @brief Stepper driver properties.
  const uint16_t kMicrostepMode = 16; ///< Microstep mode.
  /// Minimum time (us) to delay after changing the state of a pin.
  const float kPulDelay_us = 2.5F; ///< For the PUL pin.
  const float kDirDelay_us = 5.0F; ///< For the Dir pin.
  const float kEnaDelay_us = 5.0F; ///< For the Ena pin.
  /// Motion direction during continuous operation.
  const mt::StepperDriver::MotionDirection kDefaultMotionDirection = mt::StepperDriver::MotionDirection::kPositive; // Clockwise (CW).
  /// Sweep angle during oscillation.
  static const uint8_t kSizeOfSweepAngles = 4; // No. of sweep angles in the lookup table.
  const float kSweepAngles_degrees[kSizeOfSweepAngles] = {3600.0F, 90.0F, 180.0F, 360.0F}; //{45.0F, 90.0F, 180.0F, 360.0F}; // Lookup table for sweep angles (degrees).
  const uint8_t kDefaultSweepAngleIndex = 0; // Index of initial/default sweep angle, i.e., 45 degrees.
  /// Speed and acceleration.
  static const uint8_t kSizeOfSpeeds = 4; // No. of speeds in the lookup table.
  const float kSpeeds_RPM[kSizeOfSpeeds] = {150.0F, 6.0F, 12.0F, 24.0F}; //{3.0F, 6.0F, 12.0F, 24.0F}; // Lookup table for rotation speeds (RPM).
  const uint8_t kDefaultSpeedIndex = 0; // Index of initial/default sweep angle, i.e., 6 RPM.
  const float kAcceleration_microsteps_per_s_per_s = 3000.0; //7000.0; ///< Acceleration (microsteps per second-squared).
  const mt::StepperDriver::AccelerationAlgorithm kAccelerationAlgorithm = mt::StepperDriver::AccelerationAlgorithm::kMorgridge24;
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
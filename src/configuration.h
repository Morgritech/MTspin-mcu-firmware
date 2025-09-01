// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file configuration.h
/// @brief Class to setup common configuration settings, including serial port and pin definitions, etc.

#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <momentary_button.h>
#include <stepper_driver.h>

#include "version.h"

/// @brief Macro to define Serial port.
#ifndef MTSPIN_SERIAL
#define MTSPIN_SERIAL Serial // "Serial" for programming port, "SerialUSB" for native port (Due and Zero only).
#endif

namespace mtspin {

/// @brief The Configuration class using the singleton pattern i.e., only a single instance can exist.
class Configuration {
 public:

  /// @brief Enum of control system modes.
  enum class ControlMode : uint8_t {
    kContinuous = 1,
    kOscillate,
  };

  /// @brief Enum of control actions.
  enum class ControlAction : uint8_t {
    kToggleDirection = 'd',
    kCycleAngle = 'a',
    kCycleSpeed = 's',
    kToggleMotion = 'm',
    kToggleTurbo = 't',
    kToggleLogReport = 'r',
    kLogGeneralStatus = 'l',
    kReportFirmwareVersion = 'v',
    kIdle = '0',
  };

  /// @brief Static method to get the single instance.
  /// @return The Configuration instance. 
  static Configuration& GetInstance();

  /// @brief Delete the copy constructor to prevent copying of the single instance.
  Configuration(const Configuration&) = delete;

  /// @brief Delete the assignment operator to prevent copying of the single instance.
  Configuration& operator=(const Configuration&) = delete;

  /// @brief Initialise the hardware (Serial port, logging, pins, etc.).
  void BeginHardware(); ///< This must be called only once.

  /// @brief Toggle log messages.
  void ToggleLogs();

  /// @brief Report the firmware version.
  void ReportFirmwareVersion();

  // GPIO pins.
  const uint8_t kDirectionButtonPin_ = 9; ///< Input pin for the button controlling motor direction.
  const uint8_t kAngleButtonPin_ = 10; ///< Input pin for the button controlling motor angle.
  const uint8_t kSpeedButtonPin_ = 11; ///< Input pin for the button controlling motor speed.
  const uint8_t kPulPin_ = 4; ///< Output pin for the stepper driver PUL/STP/CLK (pulse/step) interface.
  const uint8_t kDirPin_ = 7; ///< Output pin for the stepper driver DIR/CW (direction) interface.
  const uint8_t kEnaPin_ = 8; ///< Output pin for the stepper driver ENA/EN (enable) interface.

  // Control system properties.
  const ControlMode kDefaultControlMode_ = ControlMode::kContinuous; ///< The default/initial control mode. 

  // Serial properties.
  const int kBaudRate_ = 9600; ///< The serial communication speed.

  // Button properties.
  const mt::MomentaryButton::PinState kUnpressedPinState_ = mt::MomentaryButton::PinState::kLow; ///< Button unpressed pin states.
  const uint16_t kDebouncePeriod_ms_ = 70; ///< Button debounce periods (ms).
  const uint16_t kShortPressPeriod_ms_ = 500; ///< Button short press periods (ms).
  const uint16_t kLongPressPeriod_ms_ = 1000; ///< Button long press periods (ms).
  const mt::MomentaryButton::LongPressOption kLongPressOption_ = mt::MomentaryButton::LongPressOption::kDetectWhileHolding; ///< Button long press options.

  // Stepper motor/drive system properties.
  const float kFullStepAngle_degrees_ = 1.8F; ///< The stepper motor full step angle (degrees).
  const float kGearRatio_ = 1.0F; ///< The system/stepper motor gear ratio.

  // Stepper driver properties.
  const uint16_t kMicrostepMode_ = 32; ///< Stepper driver microstep mode.
  const float kPulDelay_us_ = 1.0; ///< Minimum delay (us) for the stepper driver PUL pin.
  const float kDirDelay_us_ = 5.0F; ///< Minimum delay (us) for the stepper driver Dir pin.
  const float kEnaDelay_us_ = 5.0F; ///< Minimum delay (us) for the stepper driver Ena pin.
  const mt::StepperDriver::MotionDirection kDefaultMotionDirection_ = mt::StepperDriver::MotionDirection::kPositive; ///< Initial/default motion direction (Clockwise (CW)).
  inline static constexpr uint8_t kSizeOfSweepAngles_ = 4; ///< No. of sweep angles in the lookup table.
  const float kSweepAngles_degrees_[kSizeOfSweepAngles_] = {45.0F, 90.0F, 180.0F, 360.0F}; ///< Lookup table for sweep angles (degrees) during oscillation.
  const uint8_t kDefaultSweepAngleIndex_ = 0; ///< Index of initial/default sweep angle.
  inline static constexpr uint8_t kSizeOfSpeeds_ = 4; ///< No. of speeds in the lookup table.
  /// @brief Lookup table for rotation speeds (RPM).
  const float kSpeeds_RPM_[2][kSizeOfSpeeds_] = {{7.0F,  10.5F, 14.0F, 21.0F},  // Row 0: Normal speeds: S, 1.5S, 2S, 3S.
                                                 {35.0F, 42.0F, 56.0F, 77.0F}}; // Row 1: Turbo speeds: 5S, 6S, 8S, 11S.
  //                                        Index: 0       1      2      3
  const uint8_t kDefaultSpeedRow_ = 0; // Row of initial/default speed state.
  const uint8_t kDefaultSpeedIndex_ = 0; // Index of initial/default speed.
  const float kAcceleration_microsteps_per_s_per_s_ = 6000.0; //8000.0; ///< Acceleration (microsteps per second-squared).
  const mt::StepperDriver::AccelerationAlgorithm kAccelerationAlgorithm_ = mt::StepperDriver::AccelerationAlgorithm::kMorgridge24; ///< Acceleration algorithm.

  // Other properties.
  const uint16_t kStartupDelay_ms_ = 1000; ///< Minimum startup/boot delay in milliseconds (ms); based on the stepper driver.

 private:

  /// @brief Private constructor so objects cannot be manually instantiated. 
  Configuration();

  /// @brief Private destructor so objects cannot be manually instantiated. 
  ~Configuration();

  // Debug helpers and logger properties (for debugging and system reporting).
  int log_level_ =  LOG_LEVEL_SILENT; ///< The log level.
};

} // namespace mtspin
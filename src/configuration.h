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
#include <stepper_driver.h>

/// @brief Macro to define Serial port.
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
    kLogGeneralStatus = 'l',
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
  void BeginHardware() const;

  // GPIO pins.
  const uint8_t kDirectionButtonPin = 2; ///< Input pin for the button controlling motor direction.
  const uint8_t kAngleButtonPin = 3; ///< Input pin for the button controlling motor angle.
  const uint8_t kSpeedButtonPin = 4; ///< Input pin for the button controlling motor speed.
  const uint8_t kPulPin = 11; ///< Output pin for the stepper driver PUL/STP/CLK (pulse/step) interface.
  const uint8_t kDirPin = 12; ///< Output pin for the stepper driver DIR/CW (direction) interface.
  const uint8_t kEnaPin = 13; ///< Output pin for the stepper driver ENA/EN (enable) interface.

  // Control system properties.
  const ControlMode kDefaultControlMode = ControlMode::kContinuous; ///< The default/initial control mode. 

  // Serial properties.
  const int kBaudRate = 9600; ///< The serial communication speed.

  // Button properties.
  const mt::MomentaryButton::PinState kUnpressedPinState = mt::MomentaryButton::PinState::kLow; ///< Button unpressed pin states.
  const uint16_t kDebouncePeriod_ms = 20; ///< Button debounce periods (ms).
  const uint16_t kShortPressPeriod_ms = 500; ///< Button short press periods (ms).
  const uint16_t kLongPressPeriod_ms = 1000; ///< Button long press periods (ms).
  const mt::MomentaryButton::LongPressOption kLongPressOption = mt::MomentaryButton::LongPressOption::kDetectWhileHolding; ///< Button long press options.

  // Stepper motor/drive system properties.
  const float kFullStepAngle_degrees = 1.8F; ///< The stepper motor full step angle (degrees).
  const float kGearRatio = 1.0F; ///< The system/stepper motor gear ratio.

  // Stepper driver properties.
  const uint16_t kMicrostepMode = 32; ///< Stepper driver microstep mode.
  const float kPulDelay_us = 1.0; ///< Minimum delay (us) for the stepper driver PUL pin. 2.5 for TB6600 is not required since the library functions are slow enough.
  const float kDirDelay_us = 5.0F; ///< Minimum delay (us) for the stepper driver Dir pin.
  const float kEnaDelay_us = 5.0F; ///< Minimum delay (us) for the stepper driver Ena pin.
  const mt::StepperDriver::MotionDirection kDefaultMotionDirection = mt::StepperDriver::MotionDirection::kPositive; ///< Initial/default motion direction (Clockwise (CW)).
  static const uint8_t kSizeOfSweepAngles = 4; ///< No. of sweep angles in the lookup table.
  const float kSweepAngles_degrees[kSizeOfSweepAngles] = {45.0F, 90.0F, 180.0F, 360.0F}; ///< Lookup table for sweep angles (degrees) during oscillation.
  const uint8_t kDefaultSweepAngleIndex = 0; ///< Index of initial/default sweep angle, i.e., 45 degrees.
  static const uint8_t kSizeOfSpeeds = 4; ///< No. of speeds in the lookup table.
  const float kSpeeds_RPM[kSizeOfSpeeds] = {5.0F, 10.0F, 15.0F, 20.0F}; ///< Lookup table for rotation speeds (RPM).
  const uint8_t kDefaultSpeedIndex = 0; ///< Index of initial/default speed, i.e., 5 RPM.
  const float kAcceleration_microsteps_per_s_per_s = 6000; //8000.0; ///< Acceleration (microsteps per second-squared).
  const mt::StepperDriver::AccelerationAlgorithm kAccelerationAlgorithm = mt::StepperDriver::AccelerationAlgorithm::kMorgridge24; ///< Acceleration algorithm.

  // Other properties.
  const uint16_t kStartupTime_ms = 1000; ///< Minimum startup/boot time in milliseconds (ms); based on the stepper driver.

  // Debug helpers and logger properties (for debugging and system reporting).
  const bool debug_enabled_ = false; ///< Flag to control whether debug outputs are printed.
  //const bool debug_enabled_ = true;  
  const int kDefaultLogLevel =  LOG_LEVEL_SILENT; ///< The default log level.
  //const int kDefaultLogLevel =  LOG_LEVEL_VERBOSE;

 private:

  /// @brief Private constructor so objects cannot be manually instantiated. 
  Configuration();

  /// @brief Private destructor so objects cannot be manually instantiated. 
  ~Configuration();
};

} // namespace mtspin

#endif // CONFIGURATION_H_
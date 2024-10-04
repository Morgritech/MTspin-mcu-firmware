// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file control_system.h
/// @brief Class that links sensor inputs (buttons, serial, etc.) to actuator outputs (steppers, etc.).

#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include <momentary_button.h>
#include <stepper_driver.h>

#include "configuration.h"

namespace mtspin {

/// @brief The Control System class.
class ControlSystem {
 public:
  
  /// @brief Construct a Control System object. 
  ControlSystem();

  /// @brief Destroy the Control System object.
  ~ControlSystem();

  /// @brief Initialise the hardware (Serial port, logging, pins, etc.).
  void Begin() const;

  /// @brief Check inputs and trigger outputs/actions.
  void CheckAndProcess(); ///< This must be called periodically.

 private:

  /// @brief Log/report the general status of the control system.
  void LogGeneralStatus() const;

  /// @brief Configuration settings.
  const Configuration& configuration_ = Configuration::GetInstance();

  // Buttons to control the motor.
  mt::MomentaryButton direction_button_; ///< Button to control motor direction.
  mt::MomentaryButton angle_button_; ///< Button to control motor rotation angles.
  mt::MomentaryButton speed_button_; ///< Button to control motor speed.

  // Stepper motor driver.
  mt::StepperDriver stepper_driver_; ///< Stepper motor driver to control the stepper motor.

  // Control flags and indicator variables.
  bool initial_entry_ = true; ///< Flag to determine if this is the first entry into the control system.
  Configuration::ControlMode control_mode_ = configuration_.kDefaultControlMode; ///< Variable to keep track of the control system mode.
  Configuration::ControlAction control_action_ = Configuration::ControlAction::kIdle; ///< Variable to keep track of the control actions from button presses/serial messages.
  mt::StepperDriver::MotionDirection motion_direction_ = configuration_.kDefaultMotionDirection; ///< Variable to keep track of the motion direction (for continuous operation).
  //mt::StepperDriver::MotionDirection previous_motion_direction_ = configuration_.kDefaultMotionDirection; // Variable to keep track of the previously set motion direction.
  mt::StepperDriver::MotionType motion_type_ = mt::StepperDriver::MotionType::kRelative; ///< Variable to keep track of the motion type (for oscillation).
  float sweep_direction_ = static_cast<float>(motion_direction_); ///< Variable to keep track of the sweep direction.
  uint8_t sweep_angle_index_ = configuration_.kDefaultSweepAngleIndex; ///< Index to keep track of the sweep angle set from the lookup table.
  uint8_t speed_index_ = configuration_.kDefaultSpeedIndex; ///< Index to keep track of the motor speed set from the lookup table.
  mt::StepperDriver::MotionStatus motion_status_ = mt::StepperDriver::MotionStatus::kIdle; ///< Variable to keep track of the motion status.
};

} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
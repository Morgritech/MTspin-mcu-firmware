// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file control_system.h
/// @brief Class that links sensor inputs (buttons, serial, etc.) to actuator outputs (steppers, etc.).

#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include <momentary_button.h>

#include "stepper_driver.h"

namespace mtspin {

/// @brief The Control System class.
class ControlSystem {
 public:
  
  /// @brief Construct a Control System object. 
  ControlSystem();

  /// @brief Destroy the Control System object.
  ~ControlSystem();

  /// @brief Check inputs and trigger outputs/actions.
  void CheckAndProcess() const;

 private:

  /// @{
  /// @brief Buttons to control the motor driver.
  mt::MomentaryButton direction_button_; ///< Button to control motor direction.
  mt::MomentaryButton speed_button_; ///< Button to control motor speed.
  mt::MomentaryButton angle_button_; ///< Button to control motor rotation angles.
  /// @}

  /// @brief Stepper motor driver to control the stepper motor.
  //mt::StepperDriver stepper_driver_;
};

} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
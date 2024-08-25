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

  /// @brief Configuration settings.
  const Configuration& configuration_ = Configuration::GetInstance();

  /// @{
  /// @brief Buttons to control the motor driver.
  mt::MomentaryButton direction_button_; ///< To control motor direction.
  mt::MomentaryButton angle_button_; ///< To control motor rotation angles.
  mt::MomentaryButton speed_button_; ///< To control motor speed.
  /// @}

  /// @brief Stepper motor driver to control the stepper motor.
  mt::StepperDriver stepper_driver_;
  
};

} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
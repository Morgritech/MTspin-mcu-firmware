// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file control_system.h
/// @brief Class that links hardware (buttons) and serial inputs to their respective outputs/actions.

#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include "momentary_button.h"

#include "hardware_config.h"
namespace mtspin {

/// @brief The button class.
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
};

} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
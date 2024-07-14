// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class that links hardware (buttons) and serial inputs to their respective outputs/actions.
/// @file control_system.h

#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include "hardware_config.h"

#include "momentary_button.h"
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
  MomentaryButton direction_button_; ///< Button to control motor direction.
  MomentaryButton speed_button_; ///< Button to control motor speed.
  MomentaryButton angle_button_; ///< Button to control motor rotation angles.
  /// @}
};

} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file stepper_driver.h
/// @brief Class to control stepper motors via stepper motor drivers.

#ifndef STEPPER_DRIVER_H_
#define STEPPER_DRIVER_H_

namespace mt {

/// @brief The Stepper Driver class.
class StepperDriver {
 public:

  /// @brief Enum of power states based on the ENA/EN (enable) pin.
  enum class PowerState {
    kDisabled = 0,
    kEnabled,
  };

  /// @brief Enum of motor motion directions based on the DIR/CW (direction) pin.
  enum class MotionDirection {
    kNegative = -1,
    kPositive = 1,
  };

  /// @brief Construct a Stepper Driver object. 
  StepperDriver();

  /// @brief Destroy the Stepper Driver object.
  ~StepperDriver();

  /// @brief Check inputs and trigger outputs/actions.
  void MoveOneStep() const;

  /// @brief Enable or disable the motor.
  /// @param power_state The power state to set.
  void set_power_state(PowerState power_state);

  /// @brief Set the motor motion direction.
  /// @param motion_direction The motor motion direction to set.
  void set_motion_direction(MotionDirection motion_direction);

 private:

  /// @{
  /// @brief Output pins.
  uint8_t pul_pin_; ///< PUL/STP/CLK (pulse/step) pin.
  uint8_t dir_pin_; ///< DIR/CW (direction) pin.
  uint8_t ena_pin_; ///< ENA/EN (enable) pin.
  /// @}

  /// @{
  /// @brief Stepper Driver properties.
  uint8_t step_mode_; ///< Micro-stepping/step mode.
  uint8_t pul_low_time_; ///< Minimum low-level time in microseconds (us) for the PUL pin.
  uint8_t dir_change_time; ///< Minimum time (us) to delay after changing direction via the DIR pin.
  uint8_t ena_change_time; ///< Minimum time (us) to delay after changing the power state via the ENA pin.
  /// @}

  /// @{
  /// @brief Motor states and targets.
  PowerState power_state_; ///< Current power state based on the ENA pin.
  MotionDirection motion_direction_; ///< Current motor motion direction based on the DIR pin.
  uint64_t steps_to_move_; ///< Current number of steps to move the motor.
  uint32_t step_period_; ///< Desired speed based on the step period T (us) between steps.

  /// @}


};

} // namespace mt

#endif // STEPPER_DRIVER_H_
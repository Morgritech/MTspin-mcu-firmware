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

  /// @brief Enum of power states based on the ENA/EN pin.
  enum class PowerState {
    kDisabled = 0,
    kEnabled,
  };

  /// @brief Enum of motor motion directions based on the DIR/CW pin.
  enum class MotionDirection {
    kNegative = -1,
    kPositive = 1,
  };

  /// @brief Construct a Stepper Driver object. 
  /// @param pul_pin PUL/STP/CLK (pulse/step/clock) pin.
  /// @param dir_pin DIR/CW (direction) pin.
  /// @param ena_pin ENA/EN (enable) pin.
  /// @param full_step_angle_degrees Motor full step angle in degrees.
  /// @param step_mode Micro-stepping/step mode (1 = full step, 2 = half step, 4 = quarter step, etc.).
  StepperDriver(uint8_t pul_pin, uint8_t dir_pin, uint8_t ena_pin, float full_step_angle_degrees, uint8_t step_mode);

  /// @brief Destroy the Stepper Driver object.
  ~StepperDriver();

  /// @brief Set the target number of degrees to move relative to the current angular position.
  /// @param relative_degrees_to_move The relative number of degrees.
  void SetRelativeDegreesToMove(float relative_degrees_to_move);

  /// @brief Set the target number of degrees to move relative to the zero/home angular position.
  /// @param absolute_degrees_to_move The absolute number of degrees.
  void SetAbsoluteDegreesToMove(float absolute_degrees_to_move);

  /// @brief Pulse the PUL/STP/CLK pin to move the motor by the minimum step based on the micro-stepping mode.
  void MoveByMicroStep() const; ///< This must be called periodically.

  void set_gear_ratio(float gear_ratio);

  /// @brief Set the minimum low-level time (us) for the PUL pin.
  /// @param pul_low_time_us The minimum low-level time (us) for a pulse.
  void set_pul_low_time_us(float pul_low_time_us);

  /// @brief Set the minimum time (us) to delay after changing direction via the DIR pin.
  /// @param dir_change_time_us The minimum direction change time (us).
  void set_dir_change_time_us(float dir_change_time_us);

  /// @brief Set the minimum time (us) to delay after changing the power state via the ENA pin.
  /// @param ena_change_time_us The minimum enable change time (us).
  void set_ena_change_time_us(float ena_change_time_us);

  /// @brief Set the ENA/EN (enable) pin to enable or disable the motor.
  /// @param power_state The power state.
  void set_power_state(PowerState power_state);

  /// @brief Set the DIR/CW (direction) pin to select the motor motion direction.
  /// @param motion_direction The motor motion direction.
  void set_motion_direction(MotionDirection motion_direction);

  /// @brief Set the target speed based on the step period T in microseconds (us) between steps. 
  /// @param step_period The step period T (us).
  void set_step_period_us(float step_period_us);

  /// @brief Set the target number of steps to move relative to the current angular position.
  /// @param relative_steps_to_move The relative number of steps.
  void set_relative_steps_to_move(uint64_t relative_steps_to_move);

 private:

  /// @{
  /// @brief Output pins.
  uint8_t pul_pin_; ///< PUL/STP/CLK (pulse/step/clock) pin.
  uint8_t dir_pin_; ///< DIR/CW (direction) pin.
  uint8_t ena_pin_; ///< ENA/EN (enable) pin.
  /// @}

  /// @{
  /// @brief Motor drive system properties.
  float full_step_angle_degrees_; ///< Motor full step angle in degrees.
  float gear_ratio_ = 1; ///< Gear ratio for motors coupled with a gearbox in the drive system.
  float resultant_step_angle_degrees_; ///< Resultant step angle = full step angle in degrees / (gear ratio x step mode)
  /// @}

  /// @{
  /// @brief Stepper driver properties.
  uint8_t step_mode_; ///< Micro-stepping/step mode.
  float pul_low_time_us_ = 2.5; ///< Minimum low-level time (us) for the PUL pin.
  float dir_change_time_us_ = 5; ///< Minimum time (us) to delay after changing direction via the DIR pin.
  float ena_change_time_us_ = 5; ///< Minimum time (us) to delay after changing the power state via the ENA pin.
  /// @}

  /// @{
  /// @brief Motor states and targets.
  PowerState power_state_ = PowerState::kEnabled; ///< Power state based on the ENA/EN pin.
  MotionDirection motion_direction_; ///< Motor motion direction based on the DIR/CW pin.
  float step_period_us_ = 10000; ///< Target speed based on the step period T (us) between steps.
  uint64_t relative_steps_to_move_ = 0; ///< Target number of steps to move the motor relative to the current angular position.
  /// @}
};

} // namespace mt

#endif // STEPPER_DRIVER_H_
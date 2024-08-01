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

  /// @brief Enum of the types of motion.
  enum class MotionType {
    kAbsolute = 0,
    kRelative,
  };

  /// @brief Enum of angular units.
  enum class AngleUnits {
    kMicroSteps = 0,
    kDegrees,
    kRadians,
    kRevolutions,
  };

  /// @brief Enum of rotational speed unit.
  enum class SpeedUnits {
    kStepsPerSecond = 0,
    kDegreesPerSecond,
    kRadiansPerSecond,
    kRevolutionsPerMinute,
  };

  /// @brief Enum of motion status.
  enum class MotionStatus {
    kIdle = 0,
    kOngoing,
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

  /// @brief Set the target speed at which to move the motor.
  /// @param speed The target speed.
  /// @param speed_units The units of the specified speed.
  void SetSpeed(float speed, SpeedUnits speed_units = SpeedUnits::kRevolutionsPerMinute);

  /// @brief Move to a target angle with respect; to the current angular position (relative), OR; to the zero/home angular position (absolute).
  /// @param angle The target angle.
  /// @param angle_units The units of the specified angle.
  /// @param motion_type The type of motion.
  /// @return The status of the motion operation.
  MotionStatus MoveByAngle(float angle, AngleUnits angle_units = AngleUnits::kDegrees,
                           MotionType motion_type = MotionType::kRelative); ///< This must be called periodically.

  /// @brief Move the motor indefinitely (jogging).
  void MoveByJogging(); ///< This must be called periodically.

  /// @brief The gear ratio if the motor is coupled with a gearbox in the drive system.
  /// @param gear_ratio The gear ratio.
  void set_gear_ratio(float gear_ratio);

  /// @brief Set the minimum time (us) for a low or high-level pulse of the PUL pin.
  /// @param pul_delay_us The minimum PUL low or high-level delay (us).
  void set_pul_delay_us(float pul_delay_us);

  /// @brief Set the minimum time (us) to delay after changing direction via the DIR pin.
  /// @param dir_delay_us The minimum DIR change delay (us).
  void set_dir_delay_us(float dir_delay_us);

  /// @brief Set the minimum time (us) to delay after changing the power state via the ENA pin.
  /// @param ena_delay_us The minimum ENA change delay (us).
  void set_ena_delay_us(float ena_delay_us);

  /// @brief Set the ENA/EN (enable) pin to enable or disable the motor.
  /// @param power_state The power state.
  void set_power_state(PowerState power_state);

 private:

  /// @brief Pulse the PUL/STP/CLK pin to move the motor by the minimum step based on the micro-stepping mode.
  void MoveByMicroStep() const; ///< This must be called periodically.

  /// @brief Set the DIR/CW (direction) pin to select the motor motion direction.
  /// @param motion_direction The motor motion direction.
  void set_motion_direction(MotionDirection motion_direction);

  /// @brief Set the target speed based on the step period T in microseconds (us) between steps. 
  /// @param step_period The step period T (us).
  void set_step_period_us(float step_period_us);

  /// @brief Set the target number of steps to move relative to the current angular position.
  /// @param relative_steps_to_move The relative number of steps.
  void set_relative_steps_to_move(uint64_t relative_steps_to_move);

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
  float pul_delay_us_ = 2.5; ///< Minimum time (us) to delay after a low or high-level pulse of the PUL pin.
  float dir_delay_us_ = 5; ///< Minimum time (us) to delay after changing direction via the DIR pin.
  float ena_delay_us_ = 5; ///< Minimum time (us) to delay after changing the power state via the ENA pin.
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
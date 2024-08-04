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
    kEnabled = 0,
    kDisabled,
  };

  /// @brief Enum of rotational speed unit.
  enum class SpeedUnits {
    kMicrostepsPerSecond = 0,
    kDegreesPerSecond,
    kRadiansPerSecond,
    kRevolutionsPerMinute,
  };

  /// @brief Enum of angular units.
  enum class AngleUnits {
    kMicrosteps = 0,
    kDegrees,
    kRadians,
    kRevolutions,
  };

  /// @brief Enum of the types of motion/motion control.
  enum class MotionType {
    kStopAndReset = 0,
    kPause,
    kResume,
    kAbsolute,
    kRelative,
  };

  /// @brief Enum of motion status.
  enum class MotionStatus {
    kIdle = 0,
    kOngoing,
  };

  /// @brief Enum of motor motion directions based on the DIR/CW pin.
  enum class MotionDirection {
    kNegative = -1,
    kNeutral = 0,
    kPositive = 1,
  };

  /// @brief Construct a Stepper Driver object. 
  /// @param pul_pin PUL/STP/CLK (pulse/step/clock) pin.
  /// @param dir_pin DIR/CW (direction) pin.
  /// @param ena_pin ENA/EN (enable) pin.
  /// @param full_step_angle_degrees Motor full step angle in degrees.
  /// @param step_mode Micro-stepping/step mode (1 = full step, 2 = half step, 4 = quarter step, etc.).
  /// @param gear_ratio Gear ratio for motors coupled with a gearbox in the drive system.
  StepperDriver(uint8_t pul_pin, uint8_t dir_pin, uint8_t ena_pin, float full_step_angle_degrees, uint8_t step_mode,
                double gear_ratio = 1);

  /// @brief Destroy the Stepper Driver object.
  ~StepperDriver();

  /// @brief Set the target speed at which to move the motor.
  /// @param speed The target speed.
  /// @param speed_units The units of the specified speed.
  void SetSpeed(double speed, SpeedUnits speed_units = SpeedUnits::kRevolutionsPerMinute);

  /// @brief Calculate the relative number of microsteps to move to a target angle with respect to the; current angular position (relative), OR; zero/home angular position (absolute).
  /// @param angle The target angle (positive or negative).
  /// @param angle_units The units of the specified angle.
  /// @param motion_type The type of motion.
  /// @return The relative number of microsteps.
  uint64_t CalculateRelativeMicrostepsToMoveByAngle(float angle, AngleUnits angle_units = AngleUnits::kDegrees,
                           MotionType motion_type = MotionType::kRelative);

  /// @brief Move to a target angle with respect to the; current angular position (relative), OR; zero/home angular position (absolute).
  /// @param angle The target angle (positive or negative).
  /// @param angle_units The units of the specified angle.
  /// @param motion_type The type of motion.
  /// @return The status of the motion operation.
  MotionStatus MoveByAngle(float angle, AngleUnits angle_units = AngleUnits::kDegrees,
                           MotionType motion_type = MotionType::kRelative); ///< This must be called periodically.

  /// @brief Move the motor indefinitely (jogging).
  void MoveByJogging(MotionDirection direction); ///< This must be called periodically.

  /// @brief Get the current angular position.
  /// @param angle_units The units required for the angle.
  /// @return The current angular position.
  double GetAngularPosition(AngleUnits angle_units) const;

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

  /// @brief The value of pi for math calculations.
  static const double kPi = 3.14159265358979323846;

  /// @brief Pulse the PUL/STP/CLK pin to move the motor by the minimum step based on the micro-stepping mode.
  void MoveByMicrostep(); ///< This must be called periodically.

  /// @brief Move the motor by the minimum step based on the micro-stepping mode, at speed based on the microstep period (us).
  void MoveByMicrostepAtMicrostepPeriod(); ///< This must be called periodically.

  /// @{
  /// @brief Output pins.
  uint8_t pul_pin_; ///< PUL/STP/CLK (pulse/step/clock) pin.
  uint8_t dir_pin_; ///< DIR/CW (direction) pin.
  uint8_t ena_pin_; ///< ENA/EN (enable) pin.
  /// @}

  /// @{
  /// @brief Motor drive system properties.
  float full_step_angle_degrees_; ///< Motor full step angle in degrees.
  double gear_ratio_; ///< Gear ratio for motors coupled with a gearbox in the drive system.
  double microstep_angle_degrees_; ///< microstep angle = full step angle in degrees / (gear ratio x step mode)
  /// @}

  /// @{
  /// @brief Stepper driver properties.
  uint8_t step_mode_; ///< Micro-stepping/step mode.
  float pul_delay_us_ = 2.5F; ///< Minimum time (us) to delay after a low or high-level pulse of the PUL pin.
  float dir_delay_us_ = 5.0F; ///< Minimum time (us) to delay after changing direction via the DIR pin.
  float ena_delay_us_ = 5.0F; ///< Minimum time (us) to delay after changing the power state via the ENA pin.
  /// @}

  /// @{
  /// @brief Motor states and targets.
  PowerState power_state_ = PowerState::kEnabled; ///< Power state based on the ENA/EN pin.
  double microstep_period_us_ = 0.0; ///< Target speed based on the microstep period (us) between microsteps.
  uint64_t angular_position_microsteps_ = 0; ///< The current angular position (microsteps).
  uint64_t relative_microsteps_to_move_ = 0; ///< Target number of microsteps to move the motor relative to the current angular position.
  int8_t angular_position_updater_microsteps_; ///< Value (microsteps) to increment/decrement the current angular position depending on motor motion direction based on the DIR/CW pin.
  /// @}
};

} // namespace mt

#endif // STEPPER_DRIVER_H_
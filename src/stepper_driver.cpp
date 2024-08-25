// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file stepper_driver.h
/// @brief Class to control stepper motors via stepper motor drivers.

#include "stepper_driver.h"

#include <Arduino.h>

namespace mt {

StepperDriver::StepperDriver(uint8_t pul_pin, uint8_t dir_pin, uint8_t ena_pin, uint8_t microstep_mode,
                             float full_step_angle_degrees, double gear_ratio) {
  pul_pin_ = pul_pin;
  dir_pin_ = dir_pin;
  ena_pin_ = ena_pin;
  full_step_angle_degrees_ = full_step_angle_degrees;
  microstep_mode_ = microstep_mode;
  gear_ratio_ = gear_ratio;
  microstep_angle_degrees_ = full_step_angle_degrees_ / (gear_ratio_ * microstep_mode_);
}

StepperDriver::~StepperDriver() {}

void StepperDriver::SetSpeed(double speed, SpeedUnits speed_units) {
  double speed_microsteps_per_s = 0.0;

  switch (speed_units) {
    case SpeedUnits::kMicrostepsPerSecond: {
      speed_microsteps_per_s = speed;
      break;
    }
    case SpeedUnits::kDegreesPerSecond: {
      speed_microsteps_per_s = speed / microstep_angle_degrees_;
      break;
    }
    case SpeedUnits::kRadiansPerSecond: {
      speed_microsteps_per_s = (180.0 * speed) / (kPi * microstep_angle_degrees_);
      break;
    }
    case SpeedUnits::kRevolutionsPerMinute: {
      speed_microsteps_per_s = (6.0 * speed) / microstep_angle_degrees_;
      break;
    }
  }

  if (speed_microsteps_per_s == 0.0) {
    microstep_period_us_ = 0.0; // (us).
  }
  else {
    microstep_period_us_ = 1000000.0 / (speed_microsteps_per_s); // (us).
  }
}

void StepperDriver::SetAcceleration(double acceleration, AccelerationUnits acceleration_units) {
  double acceleration_microsteps_per_s_per_s = 0.0;

  switch (acceleration_units) {
    case AccelerationUnits::kMicrostepsPerSecondPerSecond: {
      acceleration_microsteps_per_s_per_s = acceleration;
      break;
    }
    case AccelerationUnits::kDegreesPerSecondPerSecond: {
      acceleration_microsteps_per_s_per_s = acceleration / microstep_angle_degrees_;
      break;
    }
    case AccelerationUnits::kRadiansPerSecondPerSecond: {
      acceleration_microsteps_per_s_per_s = (180.0 * acceleration) / (kPi * microstep_angle_degrees_);
      break;
    }
    case AccelerationUnits::kRevolutionsPerMinutePerMinute: {
      acceleration_microsteps_per_s_per_s = (6.0 * acceleration) / microstep_angle_degrees_;
      break;
    }
  }

  if (acceleration_microsteps_per_s_per_s == 0.0) {
    speed_period_us_ = 0.0; // (us).
  }
  else {
    speed_period_us_ = 1000000.0 / acceleration_microsteps_per_s_per_s; // (us).
  }
}

uint64_t StepperDriver::CalculateRelativeMicrostepsToMoveByAngle(float angle, AngleUnits angle_units,
                                                                 MotionType motion_type,
                                                                 CalculationOption calculation_option) {
  double angle_microsteps = 0.0;

  switch (angle_units) {
    case AngleUnits::kMicrosteps: {
      angle_microsteps = angle;
      break;
    }
    case AngleUnits::kDegrees: {
      angle_microsteps = angle / microstep_angle_degrees_;
      break;
    }
    case AngleUnits::kRadians: {
      angle_microsteps = (180.0 * angle) / (kPi * microstep_angle_degrees_);
      break;
    }
    case AngleUnits::kRevolutions: {
      angle_microsteps = (360.0 * angle) / microstep_angle_degrees_;
      break;
    }
  }

  int64_t relative_angle_microsteps = 0; // Always zero for other Motion Types: Stop And Reset, Pause and Resume.

  switch (motion_type) {
    case MotionType::kAbsolute: {
      // Microsteps required to move to given angular position.
      relative_angle_microsteps = round(angle_microsteps) - angular_position_microsteps_;
      break;
    }
    case MotionType::kRelative: {
      // Microsteps required to move by given angular amount.
      relative_angle_microsteps = angle_microsteps;
      break;
    }
  }

  if (calculation_option == CalculationOption::kSetupMotion) {
    if (relative_angle_microsteps < 0) {
      // Negative motion direction.
      angular_position_updater_microsteps_ = -1;
      digitalWrite(dir_pin_, LOW);
    }
    else if (relative_angle_microsteps > 0) {
      // Positive motion direction.
      angular_position_updater_microsteps_ = 1;
      digitalWrite(dir_pin_, HIGH);
    }

    delayMicroseconds(dir_delay_us_);
  }

	return abs(relative_angle_microsteps);
}

StepperDriver::MotionStatus StepperDriver::MoveByAngle(float angle, AngleUnits angle_units, MotionType motion_type) {
  if (power_state_ == PowerState::kDisabled) {
    motion_type = MotionType::kStopAndReset;
  }
  else if (microstep_period_us_ == 0.0) {
    // Pause if the set speed is 0.
    motion_type = MotionType::kPause;
  }

  switch (motion_type) {
    case MotionType::kStopAndReset: {
      relative_microsteps_to_move_ = 0;
      microsteps_after_acceleration_ = 0;
      microsteps_after_constant_speed_ = 0;
      motion_status_ = MotionStatus::kIdle;
      break;
    }
    case MotionType::kPause: {
      motion_status_ = MotionStatus::kPaused;
      break;
    }
    case MotionType::kResume: {
      [[fallthrough]];
    }
    case MotionType::kAbsolute: {
      [[fallthrough]];
    }
    case MotionType::kRelative: {
      if (motion_status_ == MotionStatus::kIdle || motion_status_ == MotionStatus::kPaused) {
        if (motion_status_ == MotionStatus::kIdle) {
          relative_microsteps_to_move_ = CalculateRelativeMicrostepsToMoveByAngle(angle, angle_units, motion_type,
                                                                                CalculationOption::kSetupMotion);
        }
        
        microsteps_after_acceleration_ = 0;
        microsteps_after_constant_speed_ = 0;
        motion_status_ = MotionStatus::kAccelerate;

      }

      break;
    }
  }

  switch (motion_status_) {
    case MotionStatus::kIdle: {
      break;
    }
    case MotionStatus::kPaused: {
      break;
    }
    case MotionStatus::kAccelerate: {
      if (speed_period_us_ == 0.0) {
        // No acceleration/deceleration.
        motion_status_ = MotionStatus::kConstantSpeed;
      }
      else if (microsteps_after_acceleration_ == 0) {
        // Setup a new acceleration.
        // Calculate initial acceleration parameters.
        AccelerateOrDecelerateAtSpeedPeriod(CalculationOption::kCalculateOnly);
        // Calculate the minimum microsteps required to accelerate to; and decelerate from; the set speed.
        uint64_t min_microsteps_for_acceleration = speed_period_us_ 
                                                   / (2000000 * microstep_period_us_ * microstep_period_us_); // (microsteps).

        if (relative_microsteps_to_move_ <= (2 * min_microsteps_for_acceleration)) {
          // Setup triangular speed profile; motor will accelerate to achievable speed (<= set speed) for available microsteps, then decelerate to 0.
          microsteps_after_acceleration_ = relative_microsteps_to_move_ / 2;
        }
        else {
          // Setup trapezoidal speed profile; motor will accelerate to set speed, move at constant speed, then decelerate to 0.
          microsteps_after_acceleration_ = relative_microsteps_to_move_ - min_microsteps_for_acceleration;
          microsteps_after_constant_speed_ = min_microsteps_for_acceleration;
        }
      }
      else {
        // Acceleration already in progress.
        if (relative_microsteps_to_move_ <= microsteps_after_acceleration_) {
          if (microsteps_after_constant_speed_ == 0) {
            // Triangular speed profile.
            motion_status_ = MotionStatus::kDecelerate;
          }
          else {
            // Trapezoidal speed profile.
            motion_status_ = MotionStatus::kConstantSpeed;
          }
        }
        else {
          AccelerateOrDecelerateAtSpeedPeriod(CalculationOption::kSetupMotion);
        }
      }

      break;
    }
    case MotionStatus::kConstantSpeed: {
      if (speed_period_us_ == 0.0) {
        // No acceleration/deceleration.
        if (relative_microsteps_to_move_ == 0) {
          motion_status_ = MotionStatus::kIdle;
        }
        else {
          MoveByMicrostepAtMicrostepPeriod(microstep_period_us_);
        }
      }
      else if (microsteps_after_constant_speed_ != 0) {
        // Trapezoidal speed profile.
        if (relative_microsteps_to_move_ <= microsteps_after_constant_speed_) {
          motion_status_ = MotionStatus::kDecelerate;
        }
        else {
          MoveByMicrostepAtMicrostepPeriod(microstep_period_us_);
        }
      }

      break;
    }
    case MotionStatus::kDecelerate: {
      if (relative_microsteps_to_move_ == 0) {
        motion_status_ = MotionStatus::kIdle;
      }
      else {
        AccelerateOrDecelerateAtSpeedPeriod(CalculationOption::kSetupMotion);
      }

      break;
    }
  }

  return motion_status_;
}

void StepperDriver::MoveByJogging(MotionDirection direction) {
  if (power_state_ == PowerState::kDisabled || microstep_period_us_ == 0.0) return;

  if (jog_direction_ != direction) {
    // Direction has changed.
    jog_direction_ = direction;

    if (jog_direction_ == MotionDirection::kNegative) {
      digitalWrite(dir_pin_, LOW); 
    }
    else if (jog_direction_ == MotionDirection::kPositive) {
      digitalWrite(dir_pin_, HIGH); 
    }

    delayMicroseconds(dir_delay_us_); 
  }

  if (jog_direction_ == MotionDirection::kNeutral) return;
  
  MoveByMicrostepAtMicrostepPeriod(microstep_period_us_);
}

double StepperDriver::GetAngularPosition(AngleUnits angle_units) const {
  double angular_position = 0.0;

  switch (angle_units) {
    case AngleUnits::kMicrosteps: {
      angular_position = static_cast<double>(angular_position_microsteps_);
      break;
    }
    case AngleUnits::kDegrees: {
      angular_position = angular_position_microsteps_ * microstep_angle_degrees_;
      break;
    }
    case AngleUnits::kRadians: {
      angular_position = (angular_position_microsteps_ * kPi * microstep_angle_degrees_) / 180.0;
      break;
    }
    case AngleUnits::kRevolutions: {
      angular_position = (angular_position_microsteps_ * microstep_angle_degrees_) / 360.0;
      break;
    }
  }
}

void StepperDriver::set_pul_delay_us(float pul_delay_us) {
  pul_delay_us_ = pul_delay_us;
}

void StepperDriver::set_dir_delay_us(float dir_delay_us) {
  dir_delay_us_ = dir_delay_us;
}

void StepperDriver::set_ena_delay_us(float ena_delay_us) {
  ena_delay_us_ = ena_delay_us;
}

void StepperDriver::set_power_state(PowerState power_state) {
  power_state_ = power_state;
  digitalWrite(ena_pin_, static_cast<uint8_t>(power_state_));
  delayMicroseconds(ena_delay_us_);
}

StepperDriver::PowerState StepperDriver::power_state() const { return power_state_; }

void StepperDriver::MoveByMicrostep() {
  digitalWrite(pul_pin_, LOW);
  delayMicroseconds(pul_delay_us_);
  digitalWrite(pul_pin_, HIGH);
  delayMicroseconds(pul_delay_us_);

  if (relative_microsteps_to_move_ != 0) {
    // Move by angle (not by jogging) is in operation.
    relative_microsteps_to_move_--;
  }

  angular_position_microsteps_ = angular_position_microsteps_ + angular_position_updater_microsteps_;
}

void StepperDriver::MoveByMicrostepAtMicrostepPeriod(double operating_microstep_period_us) {
  if ((micros() - reference_microstep_time_us_) >= operating_microstep_period_us) {
    MoveByMicrostep();
    reference_microstep_time_us_ = micros();
  }
}

void StepperDriver::AccelerateOrDecelerateAtSpeedPeriod(CalculationOption calculation_option) {
  double speed_microsteps_per_us; // (microsteps/us).

  if(calculation_option == CalculationOption::kCalculateOnly) {
    // Setup a new acceleration.
    speed_microsteps_per_us = (1000000.0 * microstep_period_us_) / speed_period_us_;
    microstep_period_in_flux_us_ = 1.0 / speed_microsteps_per_us;
  }
  else {
    // Acceleration/deceleration already in progress.
    MoveByMicrostepAtMicrostepPeriod(microstep_period_in_flux_us_);

    if ((micros() - reference_speed_time_us_) >= speed_period_us_) {
      // Calculate new speed based on the microstep period (us).
      if (motion_status_ == MotionStatus::kAccelerate) {
        speed_microsteps_per_us = (1.0 / microstep_period_in_flux_us_) 
                                  + ((1000000.0 * microstep_period_in_flux_us_) / speed_period_us_);
      }
      else {
        // Decelerate.
        speed_microsteps_per_us = (1.0 / microstep_period_in_flux_us_) 
                                  - ((1000000.0 * microstep_period_in_flux_us_) / speed_period_us_);
      }

      microstep_period_in_flux_us_ = 1.0 / speed_microsteps_per_us;
      reference_speed_time_us_ = micros();
    }
  }
}

} // namespace mt
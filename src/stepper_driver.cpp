// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file stepper_driver.h
/// @brief Class to control stepper motors via stepper motor drivers.

#include "stepper_driver.h"

#include <Arduino.h>

namespace mt {

StepperDriver::StepperDriver(uint8_t pul_pin, uint8_t dir_pin, uint8_t ena_pin, float full_step_angle_degrees,
                             uint8_t step_mode, double gear_ratio) {
  pul_pin_ = pul_pin;
  dir_pin_ = dir_pin;
  ena_pin_ = ena_pin;
  full_step_angle_degrees_ = full_step_angle_degrees;
  step_mode_ = step_mode;
  gear_ratio_ = gear_ratio;
  resultant_step_angle_degrees_ = full_step_angle_degrees_ / (gear_ratio_ * step_mode_);
}

StepperDriver::~StepperDriver() {}

void StepperDriver::SetSpeed(float speed, SpeedUnits speed_units) {
  double speed_microsteps_per_second = 0.0;
  switch (speed_units) {
    case SpeedUnits::kMicrostepsPerSecond: {
      speed_microsteps_per_second = speed;
      break;
    }
    case SpeedUnits::kDegreesPerSecond: {
      speed_microsteps_per_second = speed / resultant_step_angle_degrees_;
      break;
    }
    case SpeedUnits::kRadiansPerSecond: {
      speed_microsteps_per_second = (180.0 * speed) / (kPi * resultant_step_angle_degrees_);
      break;
    }
    case SpeedUnits::kRevolutionsPerMinute: {
      speed_microsteps_per_second = (6.0 * speed) / resultant_step_angle_degrees_;
      break;
    }
  }

  microstep_period_us_ = 1000000.0 / (speed_microsteps_per_second); // (us).
}



} // namespace mt
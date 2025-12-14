// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file control_system.cpp
/// @brief Class that links sensor inputs (buttons, serial, etc.) to actuator outputs (steppers, etc.).

#include "control_system.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <momentary_button.h>
#include <stepper_driver.h>

#include "configuration.h"

namespace mtspin {

ControlSystem::ControlSystem() {}

ControlSystem::~ControlSystem() {}

void ControlSystem::Begin() {
  configuration_.BeginHardware();
  direction_button_.set_long_press_option(configuration_.kLongPressOption_);
  angle_button_.set_long_press_option(configuration_.kLongPressOption_);
  speed_button_.set_long_press_option(configuration_.kLongPressOption_);
  stepper_driver_.set_pul_delay_us(configuration_.kPulDelay_us_);
  stepper_driver_.set_dir_delay_us(configuration_.kDirDelay_us_);
  stepper_driver_.set_ena_delay_us(configuration_.kEnaDelay_us_);
  stepper_driver_.set_ena_pin_enabled_state(configuration_.kEnergisedPinState_);
  stepper_driver_.set_dir_pin_positive_direction_state(configuration_.kPositiveDirectionPinState);
  stepper_driver_.SetSpeed(configuration_.kSpeeds_RPM_[speed_row_][speed_index_],
                           mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
  stepper_driver_.SetAcceleration(configuration_.kAcceleration_microsteps_per_s_per_s_,
                                  mt::StepperDriver::AccelerationUnits::kMicrostepsPerSecondPerSecond);
  stepper_driver_.set_acceleration_algorithm(configuration_.kAccelerationAlgorithm_);
  stepper_driver_.set_power_state(mt::StepperDriver::PowerState::kDisabled); // Save power when idle.
  LogGeneralStatus(); // Log initial status of control system.
}

void ControlSystem::CheckAndProcess() {
  // Check and process button presses, and serial input; one character at a time.
  using PressType = mt::MomentaryButton::PressType;

  if (PressType direction_button_press_type = direction_button_.DetectPressType();
      direction_button_press_type == PressType::kShortPress) {
    control_action_ = Configuration::ControlAction::kToggleDirection;
    Log.noticeln(F("Direction button short press"));
  }
  else if (PressType angle_button_press_type = angle_button_.DetectPressType();
           angle_button_press_type == PressType::kShortPress) {
    control_action_ = Configuration::ControlAction::kCycleAngle;
    Log.noticeln(F("Angle button short press"));
  }
  else if (PressType speed_button_press_type = speed_button_.DetectPressType();
           speed_button_press_type == PressType::kShortPress) {
    control_action_ = Configuration::ControlAction::kCycleSpeed;
    Log.noticeln(F("Speed button short press"));
  }
  else if (direction_button_press_type == PressType::kLongPress 
           || angle_button_press_type == PressType::kLongPress) {
    control_action_ = Configuration::ControlAction::kToggleMotion;
    Log.noticeln(F("Direction or angle button long press"));
  }
  else if (speed_button_press_type == PressType::kLongPress) {
    control_action_ = Configuration::ControlAction::kToggleTurbo;
    Log.noticeln(F("Speed button long press"));
  }
  else if (MTSPIN_SERIAL.available() > 0) {
    char serial_input = MTSPIN_SERIAL.read();
    control_action_ = static_cast<Configuration::ControlAction>(serial_input);
    Log.noticeln(F("Serial input: %c"), serial_input);
  }
  else {
    control_action_ = Configuration::ControlAction::kIdle;
  }

  // Process control actions.
  switch(control_action_) {
    case Configuration::ControlAction::kToggleDirection: {
      // Start motor, change motor direction, or change to continuous mode.
      if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kDisabled) {
        // Fall through to start motor.
        [[fallthrough]];
      }
      else {
        // change motor direction, or change to continuous mode.
        if (control_mode_ == Configuration::ControlMode::kContinuous) {
          // Change motor direction.
          if (motion_direction_ == mt::StepperDriver::MotionDirection::kPositive) {
            motion_direction_ = mt::StepperDriver::MotionDirection::kNegative;
            Log.noticeln(F("Motion direction: counter-clockwise (CCW)"));
          }
          else {
            motion_direction_ = mt::StepperDriver::MotionDirection::kPositive;
            Log.noticeln(F("Motion direction: clockwise (CW)"));
          }
        }
        else {
          // Change to continuous mode.
          control_mode_ = Configuration::ControlMode::kContinuous;
          Log.noticeln(F("Control mode: continuous"));
        }

        motion_type_ = mt::StepperDriver::MotionType::kStopAndReset;
        break;
      }
    }
    case Configuration::ControlAction::kCycleAngle: {
      // Start motor, cycle through sweep angles, or change to oscillation mode.
      if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kDisabled) {
        // Fall through to start motor.
        [[fallthrough]];
      }
      else {
        // Change sweep angles, or change to oscillation mode.
        if (control_mode_ == Configuration::ControlMode::kOscillate) {
          // Change sweep angle.
          if (sweep_angle_index_ == (configuration_.kSizeOfSweepAngles_ - 1)) {
            sweep_angle_index_ = 0;
          }
          else {
            sweep_angle_index_++;
          }

          Log.noticeln(F("Sweep angle (degrees): %F"), configuration_.kSweepAngles_degrees_[sweep_angle_index_]);
        }
        else {
          // Change to oscillation mode.
          control_mode_ = Configuration::ControlMode::kOscillate;
          Log.noticeln(F("Control mode: oscillate"));
        }

        motion_type_ = mt::StepperDriver::MotionType::kStopAndReset;
        break;
      }
    }
    case Configuration::ControlAction::kCycleSpeed: {
      //  Start motor, or cycle through motor speed settings.
      if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kDisabled) {
        // Fall through to start motor.          
        [[fallthrough]];
      }
      else {
        // Change speed.
        if (speed_index_ == (configuration_.kSizeOfSpeeds_ - 1)) {
          speed_index_ = 0;
        }
        else {
          speed_index_++;
        }
        
        stepper_driver_.SetSpeed(configuration_.kSpeeds_RPM_[speed_row_][speed_index_],
                                 mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
        Log.noticeln(F("Speed (RPM): %F"), configuration_.kSpeeds_RPM_[speed_row_][speed_index_]);
        break;
      }
    }
    case Configuration::ControlAction::kToggleMotion: {
      // Toggle (start/stop) the motor.
      if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kDisabled) {
        // Allow movement.
        stepper_driver_.set_power_state(mt::StepperDriver::PowerState::kEnabled); // Restore power to allow motion.
        Log.noticeln(F("Motion status: started"));     
      }
      else {
        // Disallow movement.
        stepper_driver_.set_power_state(mt::StepperDriver::PowerState::kDisabled); // Save power when idle.
        speed_row_ = configuration_.kDefaultSpeedRow_;
        speed_index_ = configuration_.kDefaultSpeedIndex_;
        stepper_driver_.SetSpeed(configuration_.kSpeeds_RPM_[speed_row_][speed_index_],
                                 mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
        LogGeneralStatus();
        Log.noticeln(F("Motion status: stopped"));       
      }
      
      break;
    }
    case Configuration::ControlAction::kToggleTurbo: {
      // Toggle between (0) normal and (1) turbo (faster speeds) state.
      speed_row_ = speed_row_ == 0 ? 1 : 0;
      speed_index_ = configuration_.kDefaultSpeedIndex_;
      stepper_driver_.SetSpeed(configuration_.kSpeeds_RPM_[speed_row_][speed_index_],
                               mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
      Log.noticeln(F("Speed (RPM): %F"), configuration_.kSpeeds_RPM_[speed_row_][speed_index_]);
      break;
    }
    case Configuration::ControlAction::kToggleLogReport: {
      // Toggle reporting/output of log messages over serial.
      configuration_.ToggleLogs();
      break;
    }
    case Configuration::ControlAction::kLogGeneralStatus: {
      // Log/report the general status of the control system.
      LogGeneralStatus();
      break;
    }
    case Configuration::ControlAction::kReportFirmwareVersion: {
      // Log/report the firmware version.
      configuration_.ReportFirmwareVersion();
    }
    case Configuration::ControlAction::kIdle: {
      // No action.
      //Log.noticeln(F("Idle: no action."));
      break;
    }
    default: {
      Log.errorln(F("Invalid control action"));
      break;
    }
  }

  switch (control_mode_) {
    case Configuration::ControlMode::kContinuous: {      
      if (motion_status_ != mt::StepperDriver::MotionStatus::kConstantSpeed 
          || motion_type_ == mt::StepperDriver::MotionType::kStopAndReset) {
        // Accelerate to constant speed.
        motion_status_ = stepper_driver_.MoveByAngle(static_cast<float>(motion_direction_) * 360.0,
                                                     mt::StepperDriver::AngleUnits::kDegrees, motion_type_);
        if (motion_type_ == mt::StepperDriver::MotionType::kStopAndReset) {
          // Stop and reset issued by user changing direction, restart motion.
          motion_type_ = mt::StepperDriver::MotionType::kRelative;
        }
      }
      else {
        // Continue constant speed motion indefinitely.
        stepper_driver_.MoveByJogging(motion_direction_);
      }

      break;
    }
    case Configuration::ControlMode::kOscillate: {
      mt::StepperDriver::MotionStatus motion_status = stepper_driver_.MoveByAngle(
                                                      sweep_direction_ *
                                                      configuration_.kSweepAngles_degrees_[sweep_angle_index_],
                                                      mt::StepperDriver::AngleUnits::kDegrees, motion_type_);
      if (motion_status == mt::StepperDriver::MotionStatus::kIdle) {
        // Motion completed OR stop and reset issued.
        if (motion_type_ == mt::StepperDriver::MotionType::kStopAndReset) {
          // Stop and reset issued by user changing sweep angle, restart motion.
          motion_type_ = mt::StepperDriver::MotionType::kRelative;
        }
        else if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kEnabled) {
          // Change sweep direction.
          if (motion_direction_ == mt::StepperDriver::MotionDirection::kPositive) {
            motion_direction_ = mt::StepperDriver::MotionDirection::kNegative; 
          }
          else {
            motion_direction_ = mt::StepperDriver::MotionDirection::kPositive;
          }

          sweep_direction_ = static_cast<float>(motion_direction_);
        }
      }

      break;
    }
  }
}

void ControlSystem::LogGeneralStatus() const {
  Log.noticeln(F("General Status"));
  if (control_mode_ == Configuration::ControlMode::kContinuous) {
    Log.noticeln(F("Control mode: continuous"));
  }
  else {
    Log.noticeln(F("Control mode: oscillate"));
  }

  if (motion_direction_ == mt::StepperDriver::MotionDirection::kPositive) {
    Log.noticeln(F("Motion direction: clockwise (CW)"));
  }
  else {
    Log.noticeln(F("Motion direction: counter-clockwise (CCW)"));
  }
  
  Log.noticeln(F("Sweep angle (degrees): %F"), configuration_.kSweepAngles_degrees_[sweep_angle_index_]);
  Log.noticeln(F("Speed (RPM): %F"), configuration_.kSpeeds_RPM_[speed_row_][speed_index_]);
}

} // namespace mtspin
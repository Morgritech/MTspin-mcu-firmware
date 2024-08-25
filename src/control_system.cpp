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

#include "stepper_driver.h"
#include "configuration.h"

namespace mtspin {

ControlSystem::ControlSystem()
    : direction_button_(configuration_.kDirectionButtonPin,
                        configuration_.kUnpressedPinState,
                        configuration_.kDebouncePeriod_ms,
                        configuration_.kShortPressPeriod_ms,
                        configuration_.kLongPressPeriod_ms),
      angle_button_(configuration_.kAngleButtonPin,
                    configuration_.kUnpressedPinState,
                    configuration_.kDebouncePeriod_ms,
                    configuration_.kShortPressPeriod_ms,
                    configuration_.kLongPressPeriod_ms),
      speed_button_(configuration_.kSpeedButtonPin,
                    configuration_.kUnpressedPinState,
                    configuration_.kDebouncePeriod_ms,
                    configuration_.kShortPressPeriod_ms,
                    configuration_.kLongPressPeriod_ms),
      stepper_driver_(configuration_.kPulPin,
                      configuration_.kDirPin,
                      configuration_.kEnaPin,
                      configuration_.kMicrostepMode,
                      configuration_.kFullStepAngleDegrees,
                      configuration_.kGearRatio) {
  direction_button_.set_long_press_option(configuration_.kLongPressOption);
  angle_button_.set_long_press_option(configuration_.kLongPressOption);
  speed_button_.set_long_press_option(configuration_.kLongPressOption);
  stepper_driver_.set_pul_delay_us(configuration_.kPulDelay_us);
  stepper_driver_.set_dir_delay_us(configuration_.kDirDelay_us);
  stepper_driver_.set_ena_delay_us(configuration_.kEnaDelay_us);
  stepper_driver_.SetAcceleration(configuration_.kAcceleration_rads_per_s_per_s,
                                  mt::StepperDriver::AccelerationUnits::kRadiansPerSecondPerSecond);
}

ControlSystem::~ControlSystem() {}

void ControlSystem::Begin() const {
  configuration_.BeginHardware();
}

void ControlSystem::CheckAndProcess() {
  // Save power.
  //if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kEnabled) {
  //  stepper_driver_.set_power_state(mt::StepperDriver::PowerState::kDisabled);
  //}

  // Variable to determine if this is the first entry into the control system.
  static bool initial_entry = true;
  // Variable to keep track of the control system mode.
  static Configuration::ControlMode control_mode = configuration_.kDefaultControlMode;
  // Variable to keep track of the control actions from button presses/serial messages.
  Configuration::ControlAction control_action = Configuration::ControlAction::kIdle;
  // Variable to keep track of the motion direction (for continuous operation).
  static mt::StepperDriver::MotionDirection motion_direction = configuration_.kDefaultMotionDirection;
  // Variable to keep track of the motion type (for oscillation).
  static mt::StepperDriver::MotionType motion_type = mt::StepperDriver::MotionType::kStopAndReset;
  // Index to keep track of the sweep angle set from the lookup table.
  static uint8_t sweep_angle_index = configuration_.kDefaultSweepAngleIndex;
  // Index to keep track of the motor speed set from the lookup table.
  static uint8_t speed_index = configuration_.kDefaultSpeedIndex;
  // Flag to keep track of when to move the motor.
  static bool move_motor = false;

  // Initialise remaining settings and log initial status of control system.
  if (initial_entry == true) {
    initial_entry = false;

    // Initialise the speed.
    stepper_driver_.SetSpeed(configuration_.kSpeedsRPM[speed_index],
                             mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);

    if (control_mode == Configuration::ControlMode::kContinuous) {
      Log.noticeln(F("Control mode: continuous"));
    }
    else {
      Log.noticeln(F("Control mode: oscillate"));
    }

    if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
      Log.noticeln(F("Motion direction: clockwise (CW)"));
    }
    else {
      Log.noticeln(F("Motion direction: counter-clockwise (CCW)"));
    }
    
    Log.noticeln(F("Sweep angle (degrees): %F"), configuration_.kSweepAnglesDegrees[sweep_angle_index]);
    Log.noticeln(F("Speed (RPM): %F"), configuration_.kSpeedsRPM[speed_index]);
  }

  // Check for button presses.
  mt::MomentaryButton::PressType direction_button_press_type = direction_button_.DetectPressType();
  mt::MomentaryButton::PressType angle_button_press_type = angle_button_.DetectPressType();
  mt::MomentaryButton::PressType speed_button_press_type = speed_button_.DetectPressType();

  // Process button presses.
  if (direction_button_press_type == mt::MomentaryButton::PressType::kShortPress) {
    control_action = Configuration::ControlAction::kToggleDirection;
    Log.noticeln(F("Direction button short press."));
  }

  if (angle_button_press_type == mt::MomentaryButton::PressType::kShortPress) {
    control_action = Configuration::ControlAction::kCycleAngle;
    Log.noticeln(F("Angle button short press."));
  }

  if (speed_button_press_type == mt::MomentaryButton::PressType::kShortPress) {
    control_action = Configuration::ControlAction::kCycleSpeed;
    Log.noticeln(F("Speed button short press."));
  }

  if (direction_button_press_type == mt::MomentaryButton::PressType::kLongPress 
      || angle_button_press_type == mt::MomentaryButton::PressType::kLongPress 
      || speed_button_press_type == mt::MomentaryButton::PressType::kLongPress) {
   control_action = Configuration::ControlAction::kToggleMotion;
    Log.noticeln(F("Button long press."));
  }

  // Check for serial messages, one character at a time.
  if (Serial.available() > 0) {
    char serial_message = Serial.read();
    control_action = static_cast<Configuration::ControlAction>(serial_message);
    Log.noticeln(F("Message received: %c"), serial_message);
  }

  // Process control actions.
  switch(control_action) {
    case Configuration::ControlAction::kToggleDirection: {
      // Change to continuous mode or change motor direction.
      if (move_motor == false) {
        // Fall through to start motor.
        [[fallthrough]];
      }
      else {
        if (control_mode == Configuration::ControlMode::kContinuous) {
          // Change direction.
          if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
            motion_direction = mt::StepperDriver::MotionDirection::kNegative;
            Log.noticeln(F("Motion direction: counter-clockwise (CCW)"));
          }
          else {
            motion_direction = mt::StepperDriver::MotionDirection::kPositive;
            Log.noticeln(F("Motion direction: clockwise (CW)"));
          }
        }
        else {
          // Change control mode.
          control_mode = Configuration::ControlMode::kContinuous;
          Log.noticeln(F("Control mode: continuous"));
        }

        break;
      }
    }
    case Configuration::ControlAction::kCycleAngle: {
      // Change to oscillation mode or cycle through sweep angles.
      if (move_motor == false) {
        // Fall through to start motor.
        [[fallthrough]];
      }
      else {
        if (control_mode == Configuration::ControlMode::kOscillate) {
          // Change sweep angle.
          if (sweep_angle_index == (configuration_.kSizeOfSweepAngles - 1)) {
            sweep_angle_index = 0;
          }
          else {
            sweep_angle_index++;
          }

          motion_type = mt::StepperDriver::MotionType::kStopAndReset;
          Log.noticeln(F("Sweep angle (degrees): %F"), configuration_.kSweepAnglesDegrees[sweep_angle_index]);
        }
        else {
          // Change control mode.
          control_mode = Configuration::ControlMode::kOscillate;
          Log.noticeln(F("Control mode: oscillate"));
        }

        break;
      }
    }
    case Configuration::ControlAction::kCycleSpeed: {
      // Cycle through motor speed settings.
      if (move_motor == false) {
        // Fall through to start motor.          
        [[fallthrough]];
      }
      else {
        // Change speed.
        if (speed_index == (configuration_.kSizeOfSpeeds - 1)) {
          speed_index = 0;
        }
        else {
          speed_index++;
        }
        
        stepper_driver_.SetSpeed(configuration_.kSpeedsRPM[speed_index],
                            mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
        Log.noticeln(F("Speed (RPM): %F"), configuration_.kSpeedsRPM[speed_index]);
        break;
      }
    }
    case Configuration::ControlAction::kToggleMotion: {
      // Toggle (start/stop) the motor.
      if (move_motor == false) {
        move_motor = true;
        Log.noticeln(F("Start moving."));
      }
      else {
        move_motor = false;
        Log.noticeln(F("Stop moving."));
      }
      
      break;
    }
    case Configuration::ControlAction::kIdle: {
      // No action.
      break;
    }
    default: {
      Log.errorln(F("Invalid control action."));
      break;
    }
  }

  // Reset control action.
  control_action = Configuration::ControlAction::kIdle;

  if (move_motor == true) {
    switch (control_mode) {
      case Configuration::ControlMode::kContinuous: {
        stepper_driver_.MoveByJogging(motion_direction);
        break;
      }
      case Configuration::ControlMode::kOscillate: {
        static float sweep_direction = static_cast<float>(motion_direction);
        mt::StepperDriver::MotionStatus motion_status = stepper_driver_.MoveByAngle(
                                                        sweep_direction * configuration_.kSweepAnglesDegrees[sweep_angle_index],
                                                        mt::StepperDriver::AngleUnits::kDegrees, motion_type);

        if (motion_status == mt::StepperDriver::MotionStatus::kIdle) {
          motion_type = mt::StepperDriver::MotionType::kRelative;

          if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
            motion_direction = mt::StepperDriver::MotionDirection::kNegative; 
          }
          else {
            motion_direction = mt::StepperDriver::MotionDirection::kPositive;
          }

          sweep_direction = static_cast<float>(motion_direction);
        }

        break;
      }
    }
  }
}

} // namespace mtspin
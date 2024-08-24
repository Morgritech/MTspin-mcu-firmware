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
                        configuration_.kDirectionButtonUnpressedPinState,
                        configuration_.kDirectionButtonDebouncePeriod_ms,
                        configuration_.kDirectionButtonShortPressPeriod_ms,
                        configuration_.kDirectionButtonLongPressPeriod_ms),
      speed_button_(configuration_.kSpeedButtonPin,
                    configuration_.kSpeedButtonUnpressedPinState,
                    configuration_.kSpeedButtonDebouncePeriod_ms,
                    configuration_.kSpeedButtonShortPressPeriod_ms,
                    configuration_.kAngleButtonLongPressPeriod_ms),
      angle_button_(configuration_.kAngleButtonPin,
                    configuration_.kAngleButtonUnpressedPinState,
                    configuration_.kAngleButtonDebouncePeriod_ms,
                    configuration_.kAngleButtonShortPressPeriod_ms,
                    configuration_.kAngleButtonLongPressPeriod_ms),
      stepper_driver_(configuration_.kPulPin,
                      configuration_.kDirPin,
                      configuration_.kEnaPin,
                      configuration_.kMicrostepMode,
                      configuration_.kFullStepAngleDegrees,
                      configuration_.kGearRatio) {
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
  /* TESTING BUTTON STATE CHANGE.
  mt::MomentaryButton::ButtonState direction_button_state = direction_button_.DetectStateChange();

  static int state_counter = 0;
  if (direction_button_state == mt::MomentaryButton::ButtonState::kPressed) {
    state_counter++;
    Log.noticeln(F("DIRECTION BUTTON PRESSED: %d"), state_counter);
  }

  if (direction_button_state == mt::MomentaryButton::ButtonState::kReleased) {
    Log.noticeln(F("DIRECTION BUTTON RELEASED: %d"), state_counter);
  }
  //*/
  /* TESTING BUTTON PRESS TYPE.
  mt::MomentaryButton::PressType direction_button_press_type = direction_button_.DetectPressType();
  if (direction_button_press_type == mt::MomentaryButton::PressType::kShortPress) {
    Log.noticeln(F("DIRECTION SHORT PRESS"));
  }
  
  if (direction_button_press_type == mt::MomentaryButton::PressType::kLongPress) {
    Log.noticeln(F("DIRECTION LONG PRESS"));
  }
  //*/
  /* TESTING BUTTON PRESS COUNT.
  uint8_t direction_button_press_count = direction_button_.CountPresses();
  if (direction_button_press_count > 0)
  {
    Log.noticeln(F("DIRECTION PRESSED %d TIMES"), direction_button_press_count);
  }
  //*/
  //* TESTING MOVE BY JOGGING.
  // Save power.
  //if (stepper_driver_.power_state() == mt::StepperDriver::PowerState::kEnabled) {
  //  stepper_driver_.set_power_state(mt::StepperDriver::PowerState::kDisabled);
  //}

  // Variable to determine if this is the first entry into the control system.
  static bool initial_entry = true;
  // Variable to keep track of the control system mode.
  static Configuration::ControlMode control_mode = configuration_.kDefaultControlMode;
  // Variable to hold the serial message received.
  static char serial_message;
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
      Log.noticeln(F("Control mode: full rotation"));  
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

  // Check for and process serial messages, one character at a time.
  if (Serial.available() > 0) {
    serial_message = Serial.read();
    Log.noticeln(F("Message received: %c"), serial_message);

    switch(serial_message) {
      case Configuration::kToggleDirectionMessage: {
        if (move_motor == false) {
          // Fall through to start motor.
          [[fallthrough]];
        }
        else {
          // Change direction.
          if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
            motion_direction = mt::StepperDriver::MotionDirection::kNegative;
            Log.noticeln(F("Motion direction: counter-clockwise (CCW)"));
          }
          else {
            motion_direction = mt::StepperDriver::MotionDirection::kPositive;
            Log.noticeln(F("Motion direction: clockwise (CW)"));
          }

          break;
        }
      }
      case Configuration::kCycleAngleMessage: {
        if (move_motor == false) {
          // Fall through to start motor.
          [[fallthrough]];
        }
        else {
          // Change sweep angle.
          if (sweep_angle_index == (configuration_.kSizeOfSweepAngles - 1)) {
            sweep_angle_index = 0;
          }
          else {
            sweep_angle_index++;
          }

          motion_type = mt::StepperDriver::MotionType::kStopAndReset;
          Log.noticeln(F("Sweep angle (degrees): %F"), configuration_.kSweepAnglesDegrees[sweep_angle_index]);
          break;
        }
      }
      case Configuration::kCycleSpeedMessage: {
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
      case Configuration::kToggleMotionMessage: {
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
      default : {
        Log.noticeln(F("Invalid serial message."));
        break;
      }
    }
  }

  switch (control_mode) {
    case Configuration::ControlMode::kContinuous: {
      if (move_motor == true) {
        stepper_driver_.MoveByJogging(motion_direction);
      }

      break;
    }
    case Configuration::ControlMode::kOscillate: {
      stepper_driver_.MoveByAngle(configuration_.kSweepAnglesDegrees[sweep_angle_index],
                                  mt::StepperDriver::AngleUnits::kDegrees, motion_type);
      break;
    }
  }
}

} // namespace mtspin
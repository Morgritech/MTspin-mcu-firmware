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
  stepper_driver_.SetSpeed(configuration_.kDefaultSpeed_RPM, mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
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

  // Variable to keep track of the motor speed (RPM).
  static double speed_RPM = configuration_.kDefaultSpeed_RPM;
  // Variable to hold the serial message received.
  static char serial_message;
  // Flag to keep track of when to move the motor based on a start/stop message received over serial.
  static bool move_motor = false;
  // Variable to keep track of the motion direction based on a direction message received over serial.
  static mt::StepperDriver::MotionDirection motion_direction = mt::StepperDriver::MotionDirection::kPositive;

  // Check for and process serial messages, one character at a time.
  if (Serial.available() > 0) {
    serial_message = Serial.read();
    //Serial.print(serial_message);

    switch(serial_message) {
      case Configuration::kToggleMotionMessage: {
        if (move_motor == false) {
          move_motor = true;
          Serial.println(F("Start moving."));
        }
        else {
          move_motor = false;
          Serial.println(F("Stop moving."));
        }
        
        break;
      }
      case Configuration::kToggleDirectionMessage: {
        if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
          motion_direction = mt::StepperDriver::MotionDirection::kNegative;
          Serial.println(F("Negative direction."));
        }
        else {
          motion_direction = mt::StepperDriver::MotionDirection::kPositive;
          Serial.println(F("Positive direction."));
        }

        break;
      }
      case Configuration::kCycleSpeedMessage: {
        if (speed_RPM < configuration_.kMaxSpeed_RPM) {
          speed_RPM = speed_RPM * configuration_.kSpeedMultiplier;
        }
        else {
          speed_RPM = configuration_.kMinSpeed_RPM;
        }
        
        stepper_driver_.SetSpeed(speed_RPM, mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
        Serial.print(F("Speed (RPM) = "));
        Serial.println(speed_RPM);
        break;
      }
      default : {
        Serial.println(F("Invalid serial message."));
        break;
      }
    }
  }

  // Move the motor.
  if (move_motor == true) {
    stepper_driver_.MoveByJogging(motion_direction);
  }


  //*/

  // Process button presses.

}

} // namespace mtspin
// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file control_system.cpp
/// @brief Class that links sensor inputs (buttons, serial, etc.) to actuator outputs (steppers, etc.).

#include "control_system.h"

#include <Arduino.h>
#include <ArduinoLog.h>

#include "configuration.h"

namespace mtspin {

ControlSystem::ControlSystem()
    : direction_button_(Configuration::GetInstance().kDirectionButtonPin,
                        Configuration::GetInstance().kDirectionButtonUnpressedPinState,
                        Configuration::GetInstance().kDirectionButtonDebouncePeriod,
                        Configuration::GetInstance().kDirectionButtonShortPressPeriod,
                        Configuration::GetInstance().kDirectionButtonLongPressPeriod),
      speed_button_(Configuration::GetInstance().kSpeedButtonPin,
                    Configuration::GetInstance().kSpeedButtonUnpressedPinState,
                    Configuration::GetInstance().kSpeedButtonDebouncePeriod,
                    Configuration::GetInstance().kSpeedButtonShortPressPeriod,
                    Configuration::GetInstance().kAngleButtonLongPressPeriod),
      angle_button_(Configuration::GetInstance().kAngleButtonPin,
                    Configuration::GetInstance().kAngleButtonUnpressedPinState,
                    Configuration::GetInstance().kAngleButtonDebouncePeriod,
                    Configuration::GetInstance().kAngleButtonShortPressPeriod,
                    Configuration::GetInstance().kAngleButtonLongPressPeriod),
      stepper_driver_(Configuration::GetInstance().kPulPin,
                      Configuration::GetInstance().kDirPin,
                      Configuration::GetInstance().kEnaPin,
                      Configuration::GetInstance().kStepMode,
                      Configuration::GetInstance().kFullStepAngleDegrees,
                      Configuration::GetInstance().kGearRatio) {
  stepper_driver_.set_pul_delay_us(Configuration::GetInstance().kPulDelay_us);
  stepper_driver_.set_dir_delay_us(Configuration::GetInstance().kDirDelay_us);
  stepper_driver_.set_ena_delay_us(Configuration::GetInstance().kEnaDelay_us);
  stepper_driver_.SetSpeed(Configuration::GetInstance().kDefaultSpeed_RPM,
                           mt::StepperDriver::SpeedUnits::kRevolutionsPerMinute);
  stepper_driver_.SetAcceleration(Configuration::GetInstance().kAcceleration_rads_per_s_per_s, 
                                  mt::StepperDriver::AccelerationUnits::kRadiansPerSecondPerSecond);
}

ControlSystem::~ControlSystem() {}

void ControlSystem::Begin() const {
  Configuration::GetInstance().BeginHardware();
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

  static bool pressed = false;
  static mt::StepperDriver::MotionDirection motion_direction = mt::StepperDriver::MotionDirection::kPositive;

  mt::MomentaryButton::ButtonState direction_button_state = direction_button_.DetectStateChange();
  if (direction_button_state == mt::MomentaryButton::ButtonState::kPressed) {
    pressed = true;
    Log.noticeln(F("DIRECTION BUTTON PRESSED."));
  }

  if (direction_button_state == mt::MomentaryButton::ButtonState::kReleased) {
    Log.noticeln(F("DIRECTION BUTTON RELEASED."));
    pressed = false;
  }

  mt::MomentaryButton::PressType angle_button_press_type = angle_button_.DetectPressType();
  if (angle_button_press_type == mt::MomentaryButton::PressType::kShortPress) {
    if (motion_direction == mt::StepperDriver::MotionDirection::kPositive) {
      motion_direction = mt::StepperDriver::MotionDirection::kNegative;
    }
    else {
      motion_direction = mt::StepperDriver::MotionDirection::kPositive;
    }

    Log.noticeln(F("ANGLE SHORT PRESS"));
  }

  if (pressed == true) {
    stepper_driver_.MoveByJogging(motion_direction);
  }
  //*/

  // Process button presses.

}

} // namespace mtspin
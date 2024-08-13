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
                      Configuration::GetInstance().kGearRatio) {}

ControlSystem::~ControlSystem() {}

void ControlSystem::Begin() const {
  mtspin::Configuration::GetInstance().BeginHardware();
}

void ControlSystem::CheckAndProcess() const {
  /* TESTING BUTTON STATE CHANGE.
  MomentaryButton::ButtonState direction_button_state = direction_button_.DetectStateChange();

  static int state_counter = 0;
  if (direction_button_state == MomentaryButton::ButtonState::kPressed) {
    state_counter++;
    Log.noticeln(F("DIRECTION BUTTON PRESSED: %d"), state_counter);
  }

  if (direction_button_state == MomentaryButton::ButtonState::kReleased) {
    Log.noticeln(F("DIRECTION BUTTON RELEASED: %d"), state_counter);
  }
  //*/
  /* TESTING BUTTON PRESS TYPE.
  MomentaryButton::PressType direction_button_press_type = direction_button_.DetectPressType();
  if (direction_button_press_type == MomentaryButton::PressType::kShortPress) {
    Log.noticeln(F("DIRECTION SHORT PRESS"));
  }
  
  if (direction_button_press_type == MomentaryButton::PressType::kLongPress) {
    Log.noticeln(F("DIRECTION LONG PRESS"));
  }
  //*/
  //* TESTING BUTTON PRESS COUNT.
  uint8_t direction_button_press_count = direction_button_.CountPresses();
  if (direction_button_press_count > 0)
  {
    Log.noticeln(F("DIRECTION PRESSED %d TIMES"), direction_button_press_count);
  }
  //*/

  // Process button presses.

}

} // namespace mtspin
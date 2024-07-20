// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class that links hardware (buttons) and serial inputs to their respective outputs/actions.
/// @file control_system.cpp

#include "control_system.h"

#include <Arduino.h>

#include "hardware_config.h"
namespace mtspin {

ControlSystem::ControlSystem()
    : direction_button_(kDirectionButtonPin, MomentaryButton::PinState::kLow, 70, 500, 1000),
      speed_button_(kSpeedButtonPin, MomentaryButton::PinState::kLow, 70, 500, 1000),
      angle_button_(kAngleButtonPin, MomentaryButton::PinState::kLow, 70, 500, 1000) {
}

ControlSystem::~ControlSystem() {}

void ControlSystem::CheckAndProcess() const {
  /* TESTING BUTTON STATE CHANGE.
  MomentaryButton::ButtonState direction_button_state = direction_button_.DetectStateChange();

  static int state_counter = 0;
  if (direction_button_state == MomentaryButton::ButtonState::kPressed) {
    state_counter++;
    MTSPIN_SERIAL_LOG(F("INFO: DIRECTION BUTTON PRESSED: "));
    MTSPIN_SERIAL_LOGLN(state_counter);
  }

  if (direction_button_state == MomentaryButton::ButtonState::kReleased) {
    MTSPIN_SERIAL_LOG(F("INFO: DIRECTION BUTTON RELEASED: "));
    MTSPIN_SERIAL_LOGLN(state_counter);
  }
  //*/
  /* TESTING BUTTON PRESS TYPE.
  MomentaryButton::PressType direction_button_press_type = direction_button_.DetectPressType();
  if (direction_button_press_type == MomentaryButton::PressType::kShortPress) {
    MTSPIN_SERIAL_LOGLN(F("INFO: DIRECTION SHORT PRESS"));
  }
  
  if (direction_button_press_type == MomentaryButton::PressType::kLongPress) {
    MTSPIN_SERIAL_LOGLN(F("INFO: DIRECTION LONG PRESS"));
  }
  //*/
  //* TESTING BUTTON PRESS COUNT.
  uint8_t direction_button_press_count = direction_button_.CountPresses();
  if (direction_button_press_count > 0)
  {
    MTSPIN_SERIAL_LOG(F("INFO: DIRECTION PRESSED "));
    MTSPIN_SERIAL_LOG(direction_button_press_count);
    MTSPIN_SERIAL_LOGLN(F(" TIMES"));
  }
  //*/

  // Process button presses.

}

} // namespace mtspin
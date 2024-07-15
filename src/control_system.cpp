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
    : direction_button_(kDirectionButtonPin, MomentaryButton::PinState::kLow, 200, 500, 1000),
      speed_button_(kSpeedButtonPin, MomentaryButton::PinState::kLow, 200, 500, 1000),
      angle_button_(kAngleButtonPin, MomentaryButton::PinState::kLow, 200, 500, 1000) {
}

ControlSystem::~ControlSystem() {}

void ControlSystem::CheckAndProcess() const {
  // Check if buttons are pressed.
  uint8_t direction_button_press_count = 0;
  uint8_t speed_button_press_count = 0;
  uint8_t angle_button_press_count = 0;
  MomentaryButton::PressType direction_button_press_type = direction_button_.IsPressed(direction_button_press_count);
  //MomentaryButton::PressType speed_button_press_type = direction_button_.IsPressed(speed_button_press_count);
  //MomentaryButton::PressType angle_button_press_type = direction_button_.IsPressed(angle_button_press_count);

  // Process button presses.
  if (direction_button_press_count >= 1) {
    // TBD
  }

  if (speed_button_press_count >= 1) {
    // TBD
  }

  if (angle_button_press_count >= 1) {
    // TBD
  }

  if (direction_button_press_count == 1) {
    MTSPIN_SERIAL_LOGLN(F("INFO: DIRECTION BUTTON SINGLE PRESS"));
  }

  if (direction_button_press_count == 2) {
    MTSPIN_SERIAL_LOGLN(F("INFO: DIRECTION BUTTON DOUBLE PRESS"));
  }

  if (direction_button_press_count == 3) {
    MTSPIN_SERIAL_LOGLN(F("INFO: DIRECTION BUTTON TRIPLE PRESS"));
  }

  if (direction_button_press_type == MomentaryButton::PressType::kLongPress) {
    MTSPIN_SERIAL_LOGLN(F("INFO: SPEED BUTTON LONG PRESS"));
  }

}

} // namespace mtspin
// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware buttons.
/// @file button.cpp

#include "button.h"

#include "hardware_config.h"
namespace mtspin {

Button::Button(uint8_t gpio_pin) {
    gpio_pin_ = gpio_pin;
}

Button::~Button() {}

bool Button::IsPressed() const {
    return digitalRead(gpio_pin_) == HIGH ? true : false;
}

void Button::Test() const {
    MTSPIN_SERIAL.println(F("I'm the button with pin:"));
    MTSPIN_SERIAL.println(gpio_pin_);
}

} // namespace mtspin
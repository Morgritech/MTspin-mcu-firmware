// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware buttons.
/// @file button.h

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Arduino.h"

namespace mtspin {

/// @brief The Button class.
class Button {  
    public:
    /// @brief Construct a Button object.
    /// @param gpio_pin The GPIO pin to assign to the button.
    Button(uint8_t gpio_pin);

    /// @brief Destroy the Button object.
    ~Button();

    /// @brief Check if the button is pressed.
    /// @return true (if button pressed) or false (if not pressed).
    bool IsPressed() const;

    void Test() const;

    private:
    /// @brief The GPIO pin assigned to the button.
    uint8_t gpio_pin_;
};

} // namespace mtspin

#endif // BUTTON_H_
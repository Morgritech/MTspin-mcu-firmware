// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware button presses.
/// @file button_manager.h

#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_

namespace mtspin {

/// @brief The button class.
class ButtonManager {
    public:
    /// @brief Construct a Button Manager object for a single button. 
    /// @param gpio_pin The GPIO pin assigned to the button.
    ButtonManager(uint8_t gpio_pin);

    /// @brief Destroy the Button Manager object.
    ~ButtonManager();



};



} // namespace mtspin

#endif // BUTTON_MANAGER_H_
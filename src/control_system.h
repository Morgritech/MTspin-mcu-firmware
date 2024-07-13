// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware button presses.
/// @file control_system.h

#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include "Arduino.h"

#include "momentary_button.h"

namespace mtspin {

/// @brief The button class.
class ControlSystem {
    public:
    /// @brief Construct a Control System object. 
    ControlSystem(MomentaryButton* buttons, uint8_t no_of_buttons);

    /// @brief Destroy the Button Manager object.
    ~ControlSystem();

    void CheckAndProcessButtons();

    private:

    MomentaryButton* buttons_;

};



} // namespace mtspin

#endif // CONTROL_SYSTEM_H_
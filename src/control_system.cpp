// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware button presses.
/// @file control_system.cpp

#include "control_system.h"

namespace mtspin {

ControlSystem::ControlSystem(MomentaryButton* buttons, uint8_t no_of_buttons)
{
    if (buttons + 1 == &buttons[1]) {
        // Pointer points to an array
        buttons_ = buttons;
    } 
    else {
        //Pointer does not point to an array
        static_assert(false); // Throw an error.
    }
}

ControlSystem::~ControlSystem()
{
}

void ControlSystem::CheckAndProcessButtons()
{
}

} // namespace mtspin


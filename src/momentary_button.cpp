// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware buttons.
/// @file momentary_button.cpp

#include "momentary_button.h"

#include "src_hardware_config.h"

namespace mtspin {

MomentaryButton::MomentaryButton(uint8_t gpio_pin, PinState unpressed_pin_state, uint16_t debounce_period, uint16_t multiple_press_period, uint16_t long_press_period) {
    gpio_pin_ = gpio_pin;
    unpressed_pin_state_ = unpressed_pin_state;
    debounce_period_ = debounce_period; // (ms).
    multiple_press_period_ = multiple_press_period; // (ms).
    long_press_period_ = long_press_period; // (ms).
}

MomentaryButton::~MomentaryButton() {}

MomentaryButton::PressType MomentaryButton::IsPressed(uint8_t& press_count_output) const {
    static uint8_t press_counter = 0; // To count the number of button presses.
    static bool long_press = false;
    static bool waiting_for_long_press = false;
    static bool waiting_for_press = true;
    static bool waiting_for_release = false;
    static DebounceStatus debouncing = DebounceStatus::kNotStarted;
    static uint32_t previous_button_press_time = millis(); // (ms).

    PressType press_type = PressType::kNoPress;
    press_count_output = 0;

    debouncing = CheckAndDebounceButton();
    if (waiting_for_release == true && debouncing == DebounceStatus::kNotStarted) {
        // Button has been released after a long press.
        waiting_for_press = true;
    }
    else if (waiting_for_press == true && debouncing == DebounceStatus::kOngoing) {
        // Button has been pressed.
        press_counter++;
        waiting_for_press = false;
    }
    else if (waiting_for_press == false && debouncing == DebounceStatus::kNotStarted) {
        PinState pin_state = static_cast<PinState>(digitalRead(gpio_pin_));
        if (pin_state != unpressed_pin_state_) {
            // Button is being held.
            waiting_for_long_press = true;
        }
        else {
            // Waiting for a button to be pressed.
            waiting_for_press = true;
            waiting_for_long_press = false;
        }
        previous_button_press_time = millis(); // Reset the button press timer.
    }
    else if (waiting_for_long_press == true) {
        PinState pin_state = static_cast<PinState>(digitalRead(gpio_pin_));
        if (pin_state != unpressed_pin_state_ && (millis() - previous_button_press_time) > long_press_period_) {
            // The required period of time for a long press has elapsed.
            long_press = true;
            waiting_for_release = true;
            waiting_for_long_press = false;
        }
        else if (pin_state == unpressed_pin_state_) {
            // Button has been released before long press occurred.
            waiting_for_long_press = false;
            waiting_for_press = true;
        }
    }
    else if (waiting_for_press == true) {
        if ((millis() - previous_button_press_time) > multiple_press_period_) {
            // The allowed period of time for multiple button presses has elapsed.
            press_count_output = press_counter;
            press_counter = 0;
            
            //DEBUGGING.
            //mSerial.print(F("button presses: ")); mSerial.println(buttonPresses); 
        }
    }

    switch (press_count_output){
        case 0: {
            PressType press_type = PressType::kNoPress;
            break;
        }
        case 1: {
            if (long_press == true) {
                PressType press_type = PressType::kLongPress;
                long_press = false;
            }
            else {
                PressType press_type = PressType::kSinglePress;
            }
            break;
        }
        default: { // press_count > 1
            PressType press_type = PressType::kMultiplePress;
            break;
        }
    }
    return press_type;
}

MomentaryButton::DebounceStatus MomentaryButton::CheckAndDebounceButton() const {
    static DebounceStatus status = DebounceStatus::kNotStarted;
    static PinState previous_pin_state = unpressed_pin_state_;
    static uint32_t previous_debounce_time = millis(); // (ms).

    PinState pin_state = static_cast<PinState>(digitalRead(gpio_pin_));
    previous_pin_state = pin_state;

    if (status == DebounceStatus::kNotStarted && pin_state != unpressed_pin_state_) {
        // Button has been pressed and not yet debounced.
        status = DebounceStatus::kOngoing;
    }
    else if (status == DebounceStatus::kOngoing) {
        if (pin_state != previous_pin_state) {
            // A bounce has occurred.
            previous_debounce_time = millis(); // Reset the bounce timer (ms).
        
            //DEBUGGING.
            //mSerial.println(F("debouncing"));
        }
        else if ((millis() - previous_debounce_time) > debounce_period_) {
            // The required period of time with no bounce has elapsed (no more bouncing expected).
            status = DebounceStatus::kNotStarted;
        
            //DEBUGGING.
            //mSerial.println(F("finished debouncing"));
        }
    }
    return status;
}

} // namespace mtspin
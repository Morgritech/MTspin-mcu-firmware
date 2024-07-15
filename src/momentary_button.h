// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Class to manage hardware buttons.
/// @file momentary_button.h

#ifndef MOMENTARY_BUTTON_H_
#define MOMENTARY_BUTTON_H_

#include <Arduino.h>
namespace mtspin {

/// @brief The Button class.
class MomentaryButton {
 public:

  enum class PressType {
    kNoPress = 0,
    kSinglePress,
    kMultiplePress,
    kLongPress,
  };

  enum class PinState {
    kLow = 0,
    kHigh,
  };

  enum class DebounceStatus {
    kNotStarted = 0,
    kOngoing,
  };

  /// @brief Construct a Button object.
  /// @param gpio_pin The GPIO pin to assign to the button.
  /// @param unpressed_pin_state The pin state when the button is not pressed.
  /// @param debounce_period The period of time (ms) allowed for pin debouncing.
  /// @param multiple_press_period The period of time (ms) allowed between multiple button presses in succession.
  /// @param long_press_period The period of time (ms) allowed for a long button press (press and hold).
  MomentaryButton(uint8_t gpio_pin, PinState unpressed_pin_state = PinState::kLow, uint16_t debounce_period = 200, uint16_t multiple_press_period = 500, uint16_t long_press_period = 1000);

  /// @brief Destroy the Button object.
  ~MomentaryButton();

  /// @brief Check if the button is pressed.
  /// @param press_count_output The no. of presses returned by the function.
  /// @return The type of button press.
  PressType IsPressed(uint8_t& press_count_output) const;

 private:

  /// @brief Check the pin state and debounce if the button is pressed/released.
  /// @return The status of the debounce operation.
  DebounceStatus CheckAndDebounceButton() const;

  uint8_t gpio_pin_;

  PinState unpressed_pin_state_;

  uint16_t debounce_period_;

  uint16_t multiple_press_period_;

  uint16_t long_press_period_;
};

} // namespace mtspin

#endif // MOMENTARY_BUTTON_H_
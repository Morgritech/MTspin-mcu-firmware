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
    kNotApplicable = 0,
    kShortPress,
    kLongPress,
  };

  enum class PinState {
    kLow = 0,
    kHigh,
  };

  enum class ButtonState {
    kReleased = 0,
    kPressed,
    kNoChange,
  };

  enum class DebounceStatus {
    kNotStarted = 0,
    kOngoing,
  };

  /// @brief Construct a Button object.
  /// @param gpio_pin The GPIO pin to assign to the button.
  /// @param unpressed_pin_state The pin state when the button is not pressed.
  /// @param debounce_period The period of time (ms) allowed for pin debouncing.
  /// @param multiple_press_period The period of time (ms) allowed between multiple button presses.
  /// @param long_press_period The period of time (ms) allowed for a long button press (press and hold).
  MomentaryButton(uint8_t gpio_pin, PinState unpressed_pin_state = PinState::kLow, uint16_t debounce_period = 70, uint16_t multiple_press_period = 500, uint16_t long_press_period = 1000);

  /// @brief Destroy the Button object.
  ~MomentaryButton();

  /// @brief Check if the button has changed state, and what state it has changed to.
  /// @return The button state at the time of checking (released, pressed, or no change).
  ButtonState DetectStateChange() const;

  /// @brief Check if a button has been pressed, and what type of press occurred.
  /// @return The type of button press at the time of checking.
  PressType DetectPressType() const;

  /// @brief Count the number of (short) button presses.
  /// @return The number of (short) button presses.
  uint8_t MomentaryButton::CountPresses() const;

 private:

  /// @brief Debounce (filter out input noise) when the button is pressed/released.
  /// @return The status of the debounce operation.
  DebounceStatus Debounce() const;

  uint8_t gpio_pin_;

  PinState unpressed_pin_state_;

  uint16_t debounce_period_;

  uint16_t multiple_press_period_;

  uint16_t long_press_period_;
};

} // namespace mtspin

#endif // MOMENTARY_BUTTON_H_
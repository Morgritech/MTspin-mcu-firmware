// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Common hardware configuration settings, including serial port and pin definitions.
/// @file src_hardware_config.h

// Serial port

#define MTSPIN_SERIAL Serial // "Serial" for programming port, "SerialUSB" for native port (Due and Zero only).

//GPIO Pins

//Buttons

const uint8_t kDirectionButtonPin = 0; // Motor direction button input pin.
const uint8_t kSpeedButtonPin = 1; // Motor speed button input pin.
const uint8_t kAngleButtonPin = 2; // Motor angle button input pin.

//Motor Driver

const uint8_t kPulMotorDriverPin = 3; // Motor pulse/PUL/CLK/STP output pin.
const uint8_t kDirMotorDriverPin = 4; // Motor direction/DIR/CW output pin.
const uint8_t kEnaMotorDriverPin = 5; // Motor enable/ENA/EN output pin.


//Notes

//Interrupt pins:
//Uno, Nano, Mini, other 328-based   2, 3
//Mega, Mega2560, MegaADK            2, 3, 18, 19, 20, 21
//Due                                all digital pins
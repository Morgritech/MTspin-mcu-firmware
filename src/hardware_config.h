// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @brief Common hardware configuration settings, including serial port and pin definitions.
/// @file hardware_config.h

#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include <Arduino.h>

// Serial port
#define MTSPIN_SERIAL Serial // "Serial" for programming port, "SerialUSB" for native port (Due and Zero only).

// Simple logging

#define MTSPIN_DEBUG_LOG // Comment out to disable debug log messages.

#ifdef MTSPIN_DEBUG_LOG
  #define MTSPIN_SERIAL_LOG(a) (MTSPIN_SERIAL.print(a))
  #define MTSPIN_SERIAL_LOGLN(a) (MTSPIN_SERIAL.println(a))
#else
  #define MTSPIN_SERIAL_LOG(a)
  #define MTSPIN_SERIAL_LOGLN(a)
#endif

//GPIO Pins

//Buttons

const uint8_t kDirectionButtonPin = 2; // Motor direction button input pin.
const uint8_t kSpeedButtonPin = 3; // Motor speed button input pin.
const uint8_t kAngleButtonPin = 4; // Motor angle button input pin.

//Motor Driver

const uint8_t kPulMotorDriverPin = 11; // Motor pulse/PUL/CLK/STP output pin.
const uint8_t kDirMotorDriverPin = 12; // Motor direction/DIR/CW output pin.
const uint8_t kEnaMotorDriverPin = 13; // Motor enable/ENA/EN output pin.


//Notes

//Interrupt pins:
//Uno, Nano, Mini, other 328-based   2, 3
//Mega, Mega2560, MegaADK            2, 3, 18, 19, 20, 21
//Due                                all digital pins

#endif // HARDWARE_CONFIG_H_
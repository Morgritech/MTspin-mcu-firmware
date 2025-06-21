// Copyright (C) 2024 Morgritech
//
// Licensed under GNU General Public License v3.0 (GPLv3) License.
// See the LICENSE file in the project root for full license details.

/// @file version.h
/// @brief Variables to define the firmware name and version.

#pragma once

#include <Arduino.h>

namespace mtspin {

inline constexpr char* kName = "mtspin-mcu-firmware";
inline constexpr uint16_t kMajor = 3;
inline constexpr uint16_t kMinor = 0;
inline constexpr uint16_t kPatch = 0;
inline constexpr char* kSuffix = "";

} // namespace mtspin
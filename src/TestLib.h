/// @brief Library for testing.
/// @file TestLib.h

#ifndef TestLib_h
#define TestLib_h

#include "Arduino.h"

/// @brief The TestLib namespace 
namespace mt::TestLib
{
  
//**Better to use "inline constexpr" for these but requires C++17**;

// Global variables & constants

/// pi
constexpr float pi = 3.14159265359;

// Function declarations

/// @brief Double a number.
/// @param number (int) The number to double.
/// @return 2 x number (int)
int testDoubleNumber(int number);

} // namespace mt::TestLib

#endif // TestLib_h

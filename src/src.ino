//Pre-processor Directives/Include Guards
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef mSerial
#define mSerial Serial //"Serial" for programming port, "SerialUSB" for native port (Due and Zero only).
#endif

#include "FTDebouncer.h"
#include "TestLib.h"

FTDebouncer pinDebouncer;

//Function Implementations
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int baudRate = 9600;

int minStartupTime = 1000;

//Setup Function
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------------------------------------------------------------------
void setup()
{

  mSerial.begin(baudRate); //Begin serial.

  while (!mSerial); //Wait until a serial connection is made (This is mainly for the arduino due, other boards do this automatically).

  //DEBUGGING.
  mSerial.println(F("...\n...Setup Start...\n..."));

  delay(minStartupTime); //Delay for the startup/boot time required by motor controllers.

  mSerial.println(TestLib::testDoubleNumber(2));

  //DEBUGGING.
  mSerial.println(F("...\n...Setup End...\n..."));
}

//Loop Function
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  //DEBUGGING.
  mSerial.println(F("...\n...Loop Start...\n..."));

  //TESTING.
  delay(500);

  //DEBUGGING.
  mSerial.println(F("...\n...Loop End...\n..."));
}

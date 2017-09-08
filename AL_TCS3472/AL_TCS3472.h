//
// AL_TCS3472.h
//
// Provides functions for TCS3472 color sensor.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_TCS3472__H
#define AL_TCS3472__H

#include <Arduino.h>
#include "AL_Color.h"

struct AL_RawColor
{
  uint16_t c, r, g, b;
};

class AL_TCS3472
{
public:
  AL_TCS3472(uint8_t addr, uint8_t ledPin)
      : addr(addr), ledPin(ledPin)
  {
  }

public:
  void setup();
  void powerOn(bool ledOn);
  void powerOff();
  AL_RawColor readRaw();
  AL_RgbColor readRgb();

private:
  uint8_t addr, ledPin;
};

#endif // AL_TCS3472__H

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
    AL_RgbColor read();

  private:
    uint8_t addr, ledPin;
};

#endif // AL_TCS3472__H

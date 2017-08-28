//
// AL_PCA9685.h
//
// Provides functions for PCA9685 16-channel PWM controller.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_PCA9685__H
#define AL_PCA9685__H

#include <Arduino.h>

// Pre-scale = round(clock / (4096 x update-rate)) - 1
// For 50Hz: round(25MHz / (4096 x 50Hz)) - 1 = 121 = 0x79
#define AL_PCA9685_PRE_SCALE_50HZ ((uint8_t)0x81)

class AL_PCA9685
{
  public:
    AL_PCA9685(uint8_t addr, uint8_t oePin)
        : addr(addr), oePin(oePin)
    {
    }

  public:
    void setup();
    void setSleep(bool sleep);
    void setOutputEnable(bool enabled)
    {
        if (enabled)
            digitalWrite(oePin, LOW);
        else
            digitalWrite(oePin, HIGH);
    }

    // Must enter sleep before set pre-scale.
    void setPreScale(uint8_t /* AL_PCA9685_PRE_SCALE_x */ value);

    void setOffTime(uint8_t channel, uint16_t /* [0,4096) */ value);
    void setOnTime(uint8_t channel, uint16_t /* [0,4096) */ value);

  private:
    uint8_t addr;
    uint8_t oePin;
};

#endif // AL_PCA9685__H

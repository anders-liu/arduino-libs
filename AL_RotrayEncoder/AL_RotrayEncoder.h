//
// AL_RotrayEncoder.h
//
// Provides functions for simple rotray encoder.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_ROTRAYENCODER__H
#define AL_ROTRAYENCODER__H

#include <Arduino.h>

typedef void (*AL_RotrayEncoderEventHandler)();

class AL_RotrayEncoder
{
  public:
    AL_RotrayEncoder(uint8_t aPin, uint8_t bPin)
        : aPin(aPin), bPin(bPin),
          state(0),
          clockwiseTickHandler(nullptr),
          counterclockwiseTickHandler(nullptr)
    {
    }

  public:
    void setup()
    {
        pinMode(aPin, INPUT_PULLUP);
        pinMode(bPin, INPUT_PULLUP);
    }

    void loop();

  public:
    void handleClockwiseTick(AL_RotrayEncoderEventHandler handler) { clockwiseTickHandler = handler; }
    void handleCounterclockwiseTick(AL_RotrayEncoderEventHandler handler) { counterclockwiseTickHandler = handler; }

  private:
    uint8_t aPin, bPin;
    uint32_t lastMillis;
    uint8_t state;

    AL_RotrayEncoderEventHandler clockwiseTickHandler;
    AL_RotrayEncoderEventHandler counterclockwiseTickHandler;
};

#endif // AL_ROTRAYENCODER__H

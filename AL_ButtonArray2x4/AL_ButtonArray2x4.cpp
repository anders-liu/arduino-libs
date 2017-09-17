//
// AL_ButtonArray2x4.cpp
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include "AL_ButtonArray2x4.h"

#define KEY_MASK00 ((uint8_t)(1 << AL_BA2X4_KEY00))
#define KEY_MASK01 ((uint8_t)(1 << AL_BA2X4_KEY01))
#define KEY_MASK02 ((uint8_t)(1 << AL_BA2X4_KEY02))
#define KEY_MASK03 ((uint8_t)(1 << AL_BA2X4_KEY03))
#define KEY_MASK10 ((uint8_t)(1 << AL_BA2X4_KEY10))
#define KEY_MASK11 ((uint8_t)(1 << AL_BA2X4_KEY11))
#define KEY_MASK12 ((uint8_t)(1 << AL_BA2X4_KEY12))
#define KEY_MASK13 ((uint8_t)(1 << AL_BA2X4_KEY13))

#define CHECK_AND_FIRE(key)                         \
    {                                               \
        if ((lastKeys & KEY_MASK##key) != 0 &&      \
            (tempKeys & KEY_MASK##key) == 0 &&      \
            keyDownHandler)                         \
        {                                           \
            keyDownHandler(AL_BA2X4_KEY##key);      \
        }                                           \
        else if ((lastKeys & KEY_MASK##key) == 0 && \
                 (tempKeys & KEY_MASK##key) != 0 && \
                 keyUpHandler)                      \
        {                                           \
            keyUpHandler(AL_BA2X4_KEY##key);        \
        }                                           \
    }

void AL_ButtonArray2x4::setup()
{
    pinMode(pinCol0, INPUT_PULLUP);
    pinMode(pinCol1, INPUT_PULLUP);
    pinMode(pinCol2, INPUT_PULLUP);
    pinMode(pinCol3, INPUT_PULLUP);

    pinMode(pinRow0, OUTPUT);
    pinMode(pinRow1, OUTPUT);

    digitalWrite(pinRow0, HIGH);
    digitalWrite(pinRow1, HIGH);
}

void AL_ButtonArray2x4::loop()
{
    uint32_t ms = millis();

    if (ms - lastMillis > 5)
    {
        lastMillis = ms;

        if (!tempHasBeenRead)
        {
            tempHasBeenRead = true;
            tempKeys = readKeys();
        }
        else // tempHasBeenRead
        {
            tempHasBeenRead = false;
            uint8_t tempKeys2 = readKeys();
            if (tempKeys2 == tempKeys && tempKeys != lastKeys)
            {
                // Two temp keys are equal, debounced;
                // Temp keys are different from last reading, events happen.

                CHECK_AND_FIRE(00);
                CHECK_AND_FIRE(01);
                CHECK_AND_FIRE(02);
                CHECK_AND_FIRE(03);
                CHECK_AND_FIRE(10);
                CHECK_AND_FIRE(11);
                CHECK_AND_FIRE(12);
                CHECK_AND_FIRE(13);

                lastKeys = tempKeys;
            }
        }
    }
}

uint8_t AL_ButtonArray2x4::readKeys()
{
    uint8_t keys = 0x00;

    digitalWrite(pinRow0, LOW);
    keys |= (digitalRead(pinCol0) << AL_BA2X4_KEY00);
    keys |= (digitalRead(pinCol1) << AL_BA2X4_KEY01);
    keys |= (digitalRead(pinCol2) << AL_BA2X4_KEY02);
    keys |= (digitalRead(pinCol3) << AL_BA2X4_KEY03);
    digitalWrite(pinRow0, HIGH);
    
    digitalWrite(pinRow1, LOW);
    keys |= (digitalRead(pinCol0) << AL_BA2X4_KEY10);
    keys |= (digitalRead(pinCol1) << AL_BA2X4_KEY11);
    keys |= (digitalRead(pinCol2) << AL_BA2X4_KEY12);
    keys |= (digitalRead(pinCol3) << AL_BA2X4_KEY13);
    digitalWrite(pinRow1, HIGH);
    
    return keys;
}

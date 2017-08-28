//
// AL_RotrayEncoder.cpp
//
// Provides functions for simple rotray encoder.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include "AL_RotrayEncoder.h"

#define S_READY 0
#define S_POSSIBLE_ADOWN 1
#define S_ADOWN 2
#define S_POSSIBLE_AUP 3

#define DEBOUNCE_INTERVAL 1 /* ms */

void AL_RotrayEncoder::loop()
{
    switch (state)
    {
    case S_READY:
    {
        uint8_t aVal = digitalRead(aPin);
        if (aVal == LOW)
        {
            state = S_POSSIBLE_ADOWN;
            lastMillis = millis();
        }
        break;
    }
    case S_POSSIBLE_ADOWN:
    {
        uint32_t ms = millis();
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            uint8_t aVal = digitalRead(aPin);
            if (aVal == LOW)
            {
                uint8_t bVal = digitalRead(bPin);

                if (bVal == LOW && clockwiseTickHandler)
                {
                    clockwiseTickHandler();
                }
                else if (bVal == HIGH && counterclockwiseTickHandler)
                {
                    counterclockwiseTickHandler();
                }
                state = S_ADOWN;
            }
            else
            {
                state = S_READY;
            }
        }
        break;
    }
    case S_ADOWN:
    {
        uint8_t aVal = digitalRead(aPin);
        if (aVal == HIGH)
        {
            state = S_POSSIBLE_AUP;
            lastMillis = millis();
        }
        break;
    }
    case S_POSSIBLE_AUP:
    {
        uint32_t ms = millis();
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            uint8_t aVal = digitalRead(aPin);
            if (aVal == HIGH)
            {
                state = S_READY;
            }
            else
            {
                state = S_ADOWN;
            }
        }
        break;
    }
    }
}
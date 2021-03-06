/*
 * Copyright Anders Liu.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs
 */

#include "AL_PushButton.h"

void AL_PushButton::setup()
{
    pinMode(pin, INPUT_PULLUP);
}

// state:
//     MSB ---------------> LSB
//     b7 b6 b5 b4 b3 b2 b1 b0
//
//   b7: the forth time key value.
//   b6: the third time key value.
//   b5: the second time key value.
//   b4: the first time key value.
//   b3-b0: index of which time is checking.
void AL_PushButton::loop()
{
    unsigned long ms = millis();

    if (ms - lastMillis > 5)
    {
        byte index = state & 0x0F;
        if (index == 0)
        {
            bitWrite(state, 4, digitalRead(pin));
            byte keyMap = state & 0xF0;
            if (keyMap == B01100000) handleKeyDown();
            if (keyMap == B10010000) handleKeyUp();
            state = (state & 0xF0) | 1;
        }
        else if (index == 1)
        {
            bitWrite(state, 5, digitalRead(pin));
            byte keyMap = state & 0xF0;
            if (keyMap == B11000000) handleKeyDown();
            if (keyMap == B00110000) handleKeyUp();
            state = (state & 0xF0) | 2;
        }
        else if (index == 2)
        {
            bitWrite(state, 6, digitalRead(pin));
            byte keyMap = state & 0xF0;
            if (keyMap == B10010000) handleKeyDown();
            if (keyMap == B01100000) handleKeyUp();
            state = (state & 0xF0) | 3;
        }
        else if (index == 3)
        {
            bitWrite(state, 7, digitalRead(pin));
            byte keyMap = state & 0xF0;
            if (keyMap == B00110000) handleKeyDown();
            if (keyMap == B11000000) handleKeyUp();
            state = (state & 0xF0);
        }

        lastMillis = ms;
    }
}

void AL_PushButton::handleKeyDown()
{
    isPushedValue = true;
    if (keyDownHandler) keyDownHandler();
}

void AL_PushButton::handleKeyUp()
{
    isPushedValue = false;
    if (keyUpHandler) keyUpHandler();
}

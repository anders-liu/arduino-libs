//
// AL_TouchScreen.h
//
// Implement resistive touch screen on graphic screen module.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include "AL_TouchScreen.h"

#define MIN_X_RAW 120
#define MAX_X_RAW 890

#define MIN_Y_RAW 120
#define MAX_Y_RAW 930

#define IDEL_INTERVAL 20
#define DEBOUNCE_INTERVAL 5
#define MOVE_INTERVAL 20

#define MAX_SAMPLES 4

#define S_IDLE ((uint8_t)0)
#define S_START ((uint8_t)1)
#define S_POSSIBLE_TOUCH_START ((uint8_t)2)
#define S_TOUCHED ((uint8_t)3)
#define S_POSSIBLE_TOUCH_END ((uint8_t)4)

#define HI_Z(pin)               \
    {                           \
        pinMode(pin, INPUT);    \
        digitalWrite(pin, LOW); \
    }

#define POS(pin)                 \
    {                            \
        pinMode(pin, OUTPUT);    \
        digitalWrite(pin, HIGH); \
    }

#define NEG(pin)                \
    {                           \
        pinMode(pin, OUTPUT);   \
        digitalWrite(pin, LOW); \
    }

void AL_TouchScreen::loop()
{
    uint32_t ms = millis();

    switch (state)
    {
    case S_IDLE:
        if (ms - lastMillis > IDEL_INTERVAL)
        {
            state = S_START;
            lastMillis = ms;
        }
        break;

    case S_START:
        if (checkTouch())
        {
            state = S_POSSIBLE_TOUCH_START;
        }
        break;

    case S_POSSIBLE_TOUCH_START:
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            if (checkTouch())
            {
                state = S_TOUCHED;
                read();
                if (touchStartHandler)
                {
                    touchStartHandler(curX, curY);
                }
            }
            else
            {
                state = S_START;
            }
            lastMillis = ms;
        }
        break;

    case S_TOUCHED:
        if (!checkTouch())
        {
            state = S_POSSIBLE_TOUCH_END;
        }
        else
        {
            if (ms - lastMillis > MOVE_INTERVAL)
            {
                if (touchMoveHandler)
                {
                    read();
                    touchMoveHandler(curX, curY);
                }
                lastMillis = ms;
            }
        }
        break;

    case S_POSSIBLE_TOUCH_END:
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            if (!checkTouch())
            {
                state = S_IDLE;
                if (touchEndHandler)
                {
                    touchEndHandler();
                }
            }
            else
            {
                state = S_TOUCHED;
            }
            lastMillis = ms;
        }
        break;
    }
}

void AL_TouchScreen::read()
{
    uint32_t x = sampleX();
    uint32_t y = sampleY();

    for (uint8_t i = 1; i < MAX_SAMPLES; i++)
    {
        x += sampleX();
        y += sampleY();
    }

    x /= MAX_SAMPLES;
    y /= MAX_SAMPLES;

    // Map to screen coordinates.
    x = (x - MIN_X_RAW) * shortSide / (MAX_X_RAW - MIN_X_RAW);
    y = (y - MIN_Y_RAW) * longSide / (MAX_Y_RAW - MIN_Y_RAW);

    switch (orientation)
    {
    case AL_SO_PORTRAIT1:
        curX = x;
        curY = longSide - y;
        break;
    case AL_SO_PORTRAIT2:
        curX = shortSide - x;
        curY = y;
        break;
    case AL_SO_LANDSCAPE1:
        curX = y;
        curY = x;
        break;
    case AL_SO_LANDSCAPE2:
        curX = longSide - y;
        curY = shortSide - x;
        break;
    }
}

bool AL_TouchScreen::checkTouch()
{
    HI_Z(xmPin);
    HI_Z(ypPin);
    pinMode(xpPin, INPUT_PULLUP);
    pinMode(ymPin, OUTPUT);
    digitalWrite(ymPin, LOW);
    return LOW == digitalRead(xpPin);
}

uint16_t AL_TouchScreen::sampleX()
{
    HI_Z(ypPin);
    HI_Z(ymPin);
    POS(xpPin);
    NEG(xmPin);
    return analogRead(ypPin);
}

uint16_t AL_TouchScreen::sampleY()
{
    HI_Z(xpPin);
    HI_Z(xmPin);
    POS(ypPin);
    NEG(ymPin);
    return analogRead(xpPin);
}

//
// AL_TouchScreen.cpp
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

#define CHECK_TOUCH(res)                   \
    {                                      \
        HI_Z(xmPin);                       \
        HI_Z(ypPin);                       \
        pinMode(xpPin, INPUT_PULLUP);      \
        pinMode(ymPin, OUTPUT);            \
        digitalWrite(ymPin, LOW);          \
        res = (LOW == digitalRead(xpPin)); \
    }

#define SAMPLE_X(res)            \
    {                            \
        HI_Z(ypPin);             \
        HI_Z(ymPin);             \
        POS(xpPin);              \
        NEG(xmPin);              \
        res = analogRead(ypPin); \
    }

#define SAMPLE_Y(res)            \
    {                            \
        HI_Z(xpPin);             \
        HI_Z(xmPin);             \
        POS(ypPin);              \
        NEG(ymPin);              \
        res = analogRead(xpPin); \
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
    {
        bool touched;
        CHECK_TOUCH(touched)
        if (touched)
        {
            state = S_POSSIBLE_TOUCH_START;
        }
    }
    break;

    case S_POSSIBLE_TOUCH_START:
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            bool touched;
            CHECK_TOUCH(touched)
            if (touched)
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
    {
        bool touched;
        CHECK_TOUCH(touched)
        if (!touched)
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
    }
    break;

    case S_POSSIBLE_TOUCH_END:
        if (ms - lastMillis > DEBOUNCE_INTERVAL)
        {
            bool touched;
            CHECK_TOUCH(touched)
            if (!touched)
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
    uint32_t x = 0, y = 0;
    SAMPLE_X(x);
    SAMPLE_Y(y);

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

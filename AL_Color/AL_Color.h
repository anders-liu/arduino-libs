//
// AL_Color.h
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_COLOR__H
#define AL_COLOR__H

#include <Arduino.h>
#include <stdint.h>

struct AL_RgbColor
{
    uint8_t r, g, b;
    AL_RgbColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    uint16_t toColor565()
    {
        return ((r & B11111000) << 8) | ((g & B11111100) << 3) | (b >> 3);
    }
};

struct AL_HsvColor
{
    float h, s, v;
    AL_HsvColor(float h, float s, float v) : h(h), s(s), v(v) {}
};

AL_HsvColor rgb2hsv(AL_RgbColor rgb);
AL_RgbColor hsv2rgb(AL_HsvColor hsv);

#endif // AL_COLOR__H

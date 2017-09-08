//
// AL_Color.h
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include <float.h>
#include "AL_Color.h"

AL_HsvColor rgb2hsv(AL_RgbColor rgb)
{
    float r = (float)rgb.r / 255, g = (float)rgb.g / 255, b = (float)rgb.b / 255;

    uint8_t maxColor = 0; // R=0,G=1,B=2
    float max = r, min = r;
    if (g > max)
    {
        max = g;
        maxColor = 1;
    }
    if (g < min)
    {
        min = g;
    }

    if (b > max)
    {
        max = b;
        maxColor = 2;
    }
    if (b < min)
    {
        min = b;
    }

    // Value = Max
    float v = max;

    float c = max - min;
    if (c <= FLT_EPSILON)
        return AL_HsvColor(0, 0, v);

    // Hue
    float h;
    switch (maxColor)
    {
    case 0: //R
        h = (g - b) / c;
        break;
    case 1: // G
        h = 2 + (b - r) / c;
        break;
    default: // B
        h = 4 + (r - g) / c;
        break;
    }

    h *= 60;
    if (h < 0)
        h += 360;

    // Satuation
    float s = (v <= FLT_EPSILON) ? 0 : c / v;

    return AL_HsvColor(h, s, v);
}

AL_RgbColor hsv2rgb(AL_HsvColor hsv)
{
    if (hsv.h <= FLT_EPSILON)
    {
        uint8_t v = (uint8_t)(hsv.v * 255);
        return AL_RgbColor(v, v, v);
    }

    float c = hsv.v * hsv.s;
    float h = hsv.h / 60;
    float r, g, b;
    if (h <= 1)
    {
        r = c;
        g = c * (1 - h);
        b = 0;
    }
    else if (h <= 2)
    {
        r = c * (2 - h);
        g = c;
        b = 0;
    }
    else if (h <= 3)
    {
        r = 0;
        g = c;
        b = c * (3 - h);
    }
    else if (h <= 4)
    {
        r = 0;
        g = c * (4 - h);
        b = c;
    }
    else if (h <= 5)
    {
        r = c * (5 - h);
        g = 0;
        b = c;
    }
    else if (h <= 6)
    {
        r = c;
        g = 0;
        b = c * (6 - h);
    }
    else
    {
        r = g = b = 0;
    }

    float m = hsv.v - c;
    r += m, g += m, b += m;

    return AL_RgbColor((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

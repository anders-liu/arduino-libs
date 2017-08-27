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
    AL_RgbColor(uint8_t r, uint8_t g, uint8_t b)
        : r(r), g(g), b(b) {}

    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint16_t toColor565()
    {
        return ((r & B11111000) << 8) | ((g & B11111100) << 3) | (b >> 3);
    }

    float getHue()
    {
        //   R,G,B = r,g,b / 255
        //   max = max(R,G,B)
        //   min = min(R,G,B)
        //   diff=max-min
        //   hue= |
        //        (G-B)/diff, if max = R
        //        2+(B-R)/diff, if max = G
        //        4+(R-G)/diff, if max = B
        //  hue = hue*60
        if (r == g && g == b)
            return -1;

        float rv = (float)r / 255, gv = (float)g / 255, bv = (float)b / 255;
        uint8_t maxColor = 0; // R=0,G=1,B=2
        float max = rv, min = rv;
        if (gv > max)
        {
            max = gv;
            maxColor = 1;
        }
        if (gv < min)
        {
            min = gv;
        }

        if (bv > max)
        {
            max = bv;
            maxColor = 2;
        }
        if (bv < min)
        {
            min = bv;
        }

        float hue;
        switch (maxColor)
        {
        case 0: //R
            hue = (gv - bv) / (max - min);
            break;
        case 1: // G
            hue = 2 + (bv - rv) / (max - min);
            break;
        default: // B
            hue = 4 + (rv - gv) / (max - min);
            break;
        }

        hue *= 60;
        if (hue < 0)
            hue = 360 + hue;

        return hue;
    }
};

#endif // AL_COLOR__H

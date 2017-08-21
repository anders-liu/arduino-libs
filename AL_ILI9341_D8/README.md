# AL_ILI9341_D8

An Arduino library for driving ILI9341 based TFT screen.

## Summary

This library drives ILI9341 based TFT screen, which is typically 320x240.

*NOTE*, this library has some restrictions:

* Works on ILI9341 8-bit data bus only.
* *Important*, it works on a customized wiring, you *MUST* connect all 8 data pins to a single port.
* Currently Works in landscape mode only (320 width x 240 height).

*NOTE*, sometimes you must manually modify AL_ILI9341_D8.h to make your app works. See [docs/HEADER_CUSTOMIZATION.md](docs/HEADER_CUSTOMIZATION.md)

## Dependency

This library depends on:

* [AL_GraphScreen](https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen)

## Usage

1. **Define the port and pins**

For using data bus on a single port, you need to define 3 registers related this port:
* PORTx, for sending data to bus;
* PINx, for reading data from bus. note, AVR doesn't work well when reading from PORTx, so PINx is required;
* DDRx, for seting up reading/writing.

```C++
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A8
#define TFT_CS A9
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12
```

2. **Define tft object.**

```C++
AL_ILI9341_D8 tft(&TFT_PORT, &TFT_PIN, &TFT_DDR, TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);
```

3. **Setup**

In Arduino sketch's ```setup()``` function, call ```tft.setup```

Since this library has to use ```delay()``` function family for initialize the TFT, and ```delay()``` doesn't work in global scope, you have to call ```setup()``` function.

```C++
void setup()
{
    // Setup others...

    // Setup TFT screen.
    tft.setup();

    // Setup others...
}
```

4. **Drawing**

Now draw a rectangle.

```C++
AL_RgbColor color = {0, 255, 0};
tft.fillRect(100, 100, 120, 40, color);
```

Here you go!
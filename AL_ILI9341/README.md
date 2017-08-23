# AL_ILI9341

Implement graphic screen on AL_ILI9341 (240x320) based TFT screen.

_NTOE_, this library works on customized wiring, all data lines are connected to a single port.

# Dependencies

This library depends on:

* [AL_GraphScreen](https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen)
* [AL_Color](https://github.com/anders-liu/arduino-libs/tree/master/AL_Color)
* [AL_Font](https://github.com/anders-liu/arduino-libs/tree/master/AL_Font)

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
AL_ILI9341 tft(&TFT_PORT, &TFT_PIN, &TFT_DDR, TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);
```

3. **Setup**

In Arduino sketch's ```setup()``` function, call ```tft.setup()```.

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
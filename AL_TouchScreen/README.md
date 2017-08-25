# AL_TouchScreen

Provides functions to manipulate 4-wire resistive touch screen.

# Dependencies

This library depends on:

* [AL_GraphScreen](https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen)

## Usage

1. **Define the pins**

Typically, a 4-wire resistive touch screen has 4 pins, X+ (XP), X- (XM), Y+ (YP) and Y- (YM).

In this library, X and Y values are read from Y+ and X+, so these two pins should be analog pins.

```C++
#define TC_XP A0
#define TC_XM A10
#define TC_YP A9
#define TC_YM A1
```

2. **Define ```ts``` object.**

```C++
AL_TouchScreen ts(
    TC_XP, TC_XM, TC_YP, TC_YM,
    tft.getLongSide(), tft.getShortSide());
```

```tft``` is the TFT screen along with the touch screen, see [AL_ILI9341](https://github.com/anders-liu/arduino-libs/tree/master/AL_ILI9341).

3. **Define event handlers**

AL_TouchScreen provides 3 events, you can define only a few you need.

```C++
void onTouchStart(uint16_t x, uint16_t y)
{
    // Consider MouseDown event.
}

void onTouchMove(uint16_t x, uint16_t y)
{
    // Consider MouseMove event.
}

void onTouchEnd()
{
    // Consider MouseUp event.
}
```

4. **setup**

In Arduino sketch's ```setup()``` function, setup event handlers.
Again, only setup which events you need.

```C++
void setup()
{
    // Setup others...

    // Setup touch screen.
    ts.handleTouchStart(onTouchStart);
    ts.handleTouchMove(onTouchMove);
    ts.handleTouchEnd(onTouchEnd);

    // Setup others...
}
```

5. **loop**

In Arudino sketch's ```loop()``` function, call touch screen's ```loop()``` function, make it alive!

```C++
void loop()
{
    ts.loop();
}
```

Here you go!
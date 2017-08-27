# AL_TCS3472

Provides functions to manipulate TCS3472x series color sensor.

# Dependencies

This library depends on:

* [AL_Color](https://github.com/anders-liu/arduino-libs/tree/master/AL_Color)

## Usage

1. **Define I<sup>2</sup>C address and LED pins**

```C++
#define CS_ADDR 0x29
#define CS_LED_PIN 13
```

2. **Define ```cs``` object.**

```C++
AL_TCS3472 cs(CS_ADDR, CS_LED_PIN);
```

3. **Setup and power on**

In Arduino sketch's ```setup()``` function, call ```cs.setup()``` and ```cs.powerOn()``` function.

```C++
void setup()
{
    // Setup others...

    // Setup color sensor.
    cs.setup();
    cs.powerOn(true);  // Pass 'true' to turn on LED also.

    // Setup others...
}
```

4. **Read color value anytime you want**

Once you need a color value, just ```read``` it!

```C++
AL_RgbColor color = cs.read();
```

Here you go!
# AL_RotrayEncoder

Provides functions for simple rotray encoder.

_Note_, this library doesn't handle push button on the rotray encoder, if you want to leverage the push button, just attach it to an [AL_PushButton](https://github.com/anders-liu/arduino-libs/tree/master/AL_PushButton).

## Usage

* **Define the pins connected to the rotray encoder**

```C++
#define ENC_A_PIN 49
#define ENC_B_PIN 48
```

_NOTE_, these pins will always be set as ```INPUT_PULLUP```.

* **Define encoder object.**

```C++
AL_RotrayEncoder enc(ENC_A_PIN, ENC_B_PIN);
```

* **Prepare event handlers**

AL_RotrayEncoder class provides two events, _ClockwiseTick_ and _CounterclockwiseTick_, corresponding to user tweaking the encoder clockwise or counterclockwise. The event handlers can have arbitrary name, but they must take no parameter and return void.

```C++
void onClockwiseTick()
{
    // Do what you want when the encoder is tweaked clockwise.
}

void onCounterclockwiseTick()
{
    // Do what you want when the encoder is tweaked counterclockwise.
}
```

* **setup**

In Arduino sketch's ```setup()``` function, setup the encoder.

```C++
void setup()
{
    // Setup others...

    // Setup push button.
    enc.setup();
    enc.handleClockwiseTick(onClockwiseTick);
    enc.handleCounterclockwiseTick(onCounterclockwiseTick);

    // Setup others...
}
```

* **loop**

In Arudino sketch's ```loop()``` function, call encoder's ```loop()``` function, make it alive!

```C++
void loop()
{
    // Other logics....

    // Loop the encoder.
    enc.loop();

    // Other logics...
}
```

Here you go!
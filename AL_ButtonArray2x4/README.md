# AL_ButtonArray2x4

A 2-row, 4-col button array library with anti-vibration (debounce), no delay.

## Usage

* **Define the pin connected to your button array. Optional if you want to hard coded it or it comes from variable.**

```C++
    #define COL0 3
    #define COL1 2
    #define COL2 5
    #define COL3 9
    #define ROW0 A1
    #define ROW1 4
```

_NOTE_, during scanning, column pins (COLx) will be setup as ```INPUT_PULLUP```, while row pins (ROWx) will be setup as ```OUTPUT```.

* **Define button array object.**

```C++
AL_ButtonArray2x4 ba(COL0, COL1, COL2, COL3, ROW0, ROW1);
```

* **Prepare event handler**

AL_ButtonArray2x4 class provides two events, _KeyDown_ and _KeyUp_. The event handler can have arbitrary name, but it must take one parameter as the key index, corresponding to which button is pressed or released.

Key indices can be found in AL_ButtonArray2x4.h, the AL_BA2X4_KEYxx constants.

```C++
void onKeyDown(uint8_t /* AL_BA2X4_KEYxx */ key)
{
    // Do what you want when the button is pressed.
}

void onKeyUp(uint8_t /* AL_BA2X4_KEYxx */ key)
{
    // Do what you want when the button is released.
}
```

* **setup**

In Arduino sketch's ```setup()``` function, setup your button array.

```C++
void setup()
{
    // Setup others...

    // Setup button array.
    ba.handleKeyDown(onKeyDown);
    ba.handleKeyUp(onKeyUp);
    ba.setup();

    // Setup others...
}
```

* **loop**

In Arudino sketch's ```loop()``` function, call button array's ```loop()``` function, make it alive!

```C++
void loop()
{
    // Other logics....

    // Loop the button array.
    ba.loop();

    // Other logics...
}
```

Here you go!
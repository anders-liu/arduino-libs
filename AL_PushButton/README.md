# AL_PushButton

A push button library with anti-vibration, no delay.

## Usage

* **Define the pin connected to your button. Optional if you want to hard coded it or it comes from variable.**

```C++
#define BUTTON_PIN A11
```

_NOTE_, button pin will always be set as ```INPUT_PULLUP```, which means your button should be connected between this pin and GND, and it's active low.

* **Define button object.**

```C++
AL_PushButton btn(BUTTON_PIN);
```

* **Prepare event handler**

AL_PushButton class provides two events, _KeyDown_ and _KeyUp_. The event handler can have arbitrary name, but it must take no parameter and return void.

```C++
void onKeyDown()
{
    // Do what you want when the button is pushed.
}

void onKeyUp()
{
    // Do what you want when the button is released.
}
```

* **setup**

In Arduino sketch's ```setup()``` function, setup your button.

```C++
void setup()
{
    // Setup others...

    // Setup push button.
    btn.handleKeyDown(onKeyDown);
    btn.handleKeyUp(onKeyUp);
    btn.setup();

    // Setup others...
}
```

* **loop**

In Arudino sketch's ```loop()``` function, call button's ```loop()``` function, make it alive!

```C++
void loop()
{
    // Other logics....

    // Loop the button.
    btn.loop();

    // Other logics...
}
```

Here you go!
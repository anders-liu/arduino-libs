# AL_PCA9685

Provides functions for manipulating PCA9685 16-channel PWM controller.

## Usage

1. **Define I<sup>2</sup>C address and OE pins**

```C++
#define PWM_ADDR 0x40
#define PWM_OE_PIN 12
```

2. **Define ```pwm``` object.**

```C++
AL_PCA9685 pwm(PWM_ADDR, PWM_OE_PIN);
```

3. **Setup and power on**

In Arduino sketch's ```setup()``` function, call ```cs.setup()``` and ```cs.powerOn()``` function.

Since PCA9685 communicates with MCU through I<sup>2</sup>C interface, don't forget ```Wire.begin()```.

```C++
void setup()
{
    Wire.begin();
    // Setup others...

    // Setup PWM controller.
    pwm.setup();
    pwm.setOutputEnable(true);

    // Setup others...
}
```

4. **Set OFF time for a channel**

Set OFF time can control PWM width.

```C++
pwm.setOffTime(channel, offTime /* [0, 4096] */);
```

If you want to control phasing of PWM, you can also call ```setOnTime``` function:

```C++
pwm.setOnTime(channel, onTime /* [0, 4096] */);
```

Here you go!
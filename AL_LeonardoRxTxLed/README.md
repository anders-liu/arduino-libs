# AL_LeonardoRxTxLed

This library provides functions for manipulating TX/RX LED on Arduino Leonardo (ATmega32u4).

On ATmega32u4 based Arduino board, TX_LED is connected to PD5, negative active; RX_LED is coneected to PB0, negative active; They can be manipulated as normal I/O pins.

## Usage

* **Define the rxtx object.**

```C++
AL_LeonardoRxTxLed rxtx;
```

* **Turn on/off the rx/tx LED(s) as you wish!**

```C++
	rxtx.rxOn();  // Turn on RX LED.
	rxtx.rxOff();  // Turn off RX LED.
	rxtx.txOn();  // Turn on TX LED.
	rxtx.txOff();  // Turn off TX LED.

```

Here you go!
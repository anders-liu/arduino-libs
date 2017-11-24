//
// AL_LeonardoRxTxLed.h
//
// Provides functions for manipulating TX/RX LED on Arduino Leonardo (ATmega32u4).
//
// On ATmega32u4 based Arduino board,
// TX_LED is connected to PD5, negative active;
// RX_LED is coneected to PB0, negative active;
// They can be manipulated as normal I/O pins.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_LEONARDORXTXLED_H
#define AL_LEONARDORXTXLED_H

#include <Arduino.h>

class AL_LeonardoRxTxLed {
public:
	AL_LeonardoRxTxLed() {
		// Set TX/RX as output.
		DDRD |= (1 << DDD5);
		DDRB |= (1 << DDB0);
	}

	void txOn() { PORTD &= ~(1 << PORTD5); }
	void txOff() { PORTD |= (1 << PORTD5); }
	void rxOn() { PORTB &= ~(1 << PORTB0); }
	void rxOff() { PORTB |= (1 << PORTB0); }
};

#endif  // AL_LEONARDORXTXLED_H

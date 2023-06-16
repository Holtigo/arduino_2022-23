#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <button.h>
#include <led.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <util/delay.h>

ISR(PCINT1_vect) {
    if (buttonPushed(0)) {
        _delay_us(1000);
        if (buttonPushed(0)) {
            lightUpLeds(0b00000110);
            _delay_ms(5000);
            lightDownLeds(0b00000110);
            _delay_ms(5000);
        }
    } else if (buttonPushed(1)) {
        _delay_us(1000);
        if (buttonPushed(1)) {
            for (int i = 0; i < 7; i++) {
                lightUpLed(2);
                _delay_ms(500);
                lightDownLed(2);
                _delay_ms(500);
            }
        }
    }
}

int main() {
    enableAllLeds();
    lightDownAllLeds();
    enableButton(0);
    enableButton(1);

    PCICR |= _BV(PCIE1);
    PCMSK1 |= _BV(PC1) | _BV(PC2);
    sei();

    while (1) {
        lightUpLeds(0b00001100);
        _delay_ms(2000);
        lightDownLeds(0b00001100);
        _delay_ms(2000);
    }
    return 0;
}
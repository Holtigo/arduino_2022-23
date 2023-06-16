#define __DELAY_BACKWARD_COMPATIBLE__  // laat toe om een variabele te gebruiken in _delay_ms(..)

#include <../usart/usart.h>
#include <avr/io.h>
#include <util/delay.h>

#define NUMBER_OF_LEDS 4

void enableLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS)  // We controleren of de ledNumber binnen de grenzen van het aantal leds valt
        return;
    // DDRB |= (1 << (PB2 + ledNumber));
    DDRB |= _BV(PB2 + ledNumber);  // DDR instellen, ipv (1<<PB2 + ledNumber) gebruiken we de macro _BV(bit) (BV staat voor "Bit Value")
}

void enableLeds(uint8_t leds) {
    DDRB = (leds << PB2);
}

void enableAllLeds() {
    enableLeds(0b00001111);
}

void lightUpLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS)
        return;
    PORTB &= ~_BV(PB2 + ledNumber);
}

void lightUpLeds(uint8_t leds) {
    PORTB &= (~leds << PB2);
}

void lightUpAllLeds() {
    lightUpLeds(0b00001111);
}

void lightDownLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS)
        return;
    PORTB |= _BV(PB2 + ledNumber);
}

void lightDownLeds(uint8_t leds) {
    PORTB |= (leds << PB2);
}

void lightDownAllLeds() {
    lightDownLeds(0b00001111);
}

// 0b00000000 --> 1 toggles status led
void toggelLeds(uint8_t leds) {
    PORTB ^= (leds << PB2);
}

void toggelLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS)
        return;
    PORTB ^= _BV(PB2 + ledNumber);
}

void dimLed(int lednumber, int percentage, int duration) {
    if (lednumber < 0 || lednumber >= NUMBER_OF_LEDS || percentage < 0 || percentage > 100 || duration < 0)
        return;
    int milliseconds;
    for (milliseconds = duration; milliseconds > 0; milliseconds -= 10) {
        lightUpLed(lednumber);
        _delay_ms(percentage / 10);
        lightDownLed(lednumber);
        _delay_ms((100 - percentage) / 10);
    }
}

void fadeInLed(int led, int duration) {
    if (led < 0 || led >= NUMBER_OF_LEDS || duration < 0)
        return;
    for (int percentage = 1; percentage <= 100; percentage += 1) {
        dimLed(led, percentage, duration / 100);
    }
    lightUpLed(led);
}

void fadeOutLed(int led, int duration) {
    if (led < 0 || led >= NUMBER_OF_LEDS || duration < 0)
        return;
    for (int percentage = 100; percentage >= 1; percentage -= 1) {
        dimLed(led, percentage, duration / 100);
    }
    lightDownLed(led);
}

int isLedOn(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS)
        return 0;
    if (bit_is_clear(PORTB, PB2 + ledNumber)) {  // ipv (!(PORTB & (1 << PB2 + ledNumber))) gebruiken we de macro bit_is_clear
        return 1;
    }
    return 0;
}

void fadeOutAllLeds(int direction, int duration) {
    // Only fade out leds that are on
    if (direction < 0 || direction > 1 || duration < 0)
        return;

    for (int led = 0; led < NUMBER_OF_LEDS; led++) {
        if (direction == 0) {
            if (isLedOn(led) == 1) {
                fadeOutLed(led, duration);
            }
        } else {
            // direction == 1, inverted
            if (isLedOn(NUMBER_OF_LEDS - led - 1) == 1) {
                fadeOutLed(NUMBER_OF_LEDS - led - 1, duration);
            }
        }
    }
}
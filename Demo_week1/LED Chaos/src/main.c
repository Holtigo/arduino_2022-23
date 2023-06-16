#define __DELAY_BACKWARD_COMPATIBLE__  // laat toe om een variabele te gebruiken in _delay_ms(..)

#include <avr/io.h>
#include <led.h>
#include <stdlib.h>
#include <usart.h>
#include <util/delay.h>

// LED Chaos
// On launch, make random LEDs light up and down, fade in and out, and dim for a random amount of time.
int main() {
    int randomDelay = 0;    // Random delay in ms between 100 and 1000
    int randomFunction = 0;
    int randomLed = 0;
    // Initialize random generator
    srand(time(0));
    // Initialize USART
    initUSART();
    // Initialize LEDs
    enableAllLeds();
    lightDownAllLeds();

    return 0;
}
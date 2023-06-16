#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <button.h>
#include <led.h>
#include <math.h>
#include <util/delay.h>

int leds = 0;
int buttons[] = {0, 0, 0};
int ledValues[] = {1, 2, 4};

// If button 0 is pressed, start blinking LED 0, if button 0 is pressed again, stop blinking LED 0
// If button 1 is pressed, start blinking LED 1, if button 1 is pressed again, stop blinking LED 1
// If button 2 is pressed, start blinking LED 2, if button 2 is pressed again, stop blinking LED 2

void handleButtonValue(int button) {
    for (int i = 0; i < 3; i++) {
        if (button == i) {
            if (buttons[i] == 0) {
                leds += ledValues[i];
                buttons[i] = 1;
            } else {
                lightDownLed(i);
                leds -= ledValues[i];
                buttons[i] = 0;
            }
        }
    }
}

void handleButton(int button) {
    for (int i = 0; i < 3; i++) {
        if (button == i) {
            if (buttonPushed(i) == 1) {
                handleButtonValue(i);
            }
        }
    }
}

ISR(PCINT1_vect) {
    handleButton(0);
    handleButton(1);
    handleButton(2);
    // for (int i = 0; i < 3; i++) {
    //     if (buttonPushed(i) == 1) {
    //         if (buttons[i] == 0) {
    //             leds += ledValues[i];
    //             buttons[i] = 1;
    //         } else {
    //             lightDownLed(i);
    //             leds -= ledValues[i];
    //             buttons[i] = 0;
    //         }
    //     }
    // }
}

int main() {
    enableAllButtons();
    enableAllButtonInterrupts();
    enableAllLeds();
    lightDownAllLeds();

    while (1) {
        toggelLeds(leds);
        _delay_ms(250);
        lightDownAllLeds();
        _delay_ms(250);
    }

    return 0;
}
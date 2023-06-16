#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <button.h>
#include <buzzer.h>
#include <display.h>
#include <potentiometer.h>
#include <usart.h>
#include <util/delay.h>

int playing = 0;
uint32_t potVal;

void handleButtonPress(int button) {
    if (buttonPushed(button) == 1) {
        if (playing == 0) {
            playing = 1;
        } else {
            playing = 0;
        }
    }
}

ISR(PCINT1_vect) {
    handleButtonPress(0);
    handleButtonPress(1);
    handleButtonPress(2);
}

int main() {
    enableAllButtons();
    enableAllButtonInterrupts();
    enableBuzzer();
    turnOffBuzzer();
    initDisplay();
    initUSART();
    initADC1V1();
    uint32_t prevValue = 0;
    float toneVal = 0;
    while (1) {
        potVal = readPotmeterValue();
        if (potVal != prevValue && potVal != prevValue + 1 && potVal != prevValue - 1) {
            writeNumber(potVal);
            prevValue = potVal;
        } else {
            writeNumber(prevValue);
        }
        toneVal = (5800.0 / 1023.) * prevValue + 200.0;
        if (playing == 1) {
            clearDisplay();
            for (int i = 0; i < 500; i += 25) {
                playTone(toneVal, 25);
            }
        }
    }

    return 0;
}
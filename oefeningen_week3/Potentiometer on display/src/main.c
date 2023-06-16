#include <avr/io.h>
#include <display.h>
#include <potentiometer.h>
#include <usart.h>
#include <util/delay.h>

int main() {
    initADC1V1();
    initDisplay();

    uint32_t prevValue = 0;
    while (1) {
        uint32_t potValue = readPotmeterValue();
        if (potValue != prevValue && potValue != prevValue + 1 && potValue != prevValue - 1) {
            writeNumberAndWait(potValue, 25);
            prevValue = potValue;
        } else {
            writeNumberAndWait(prevValue, 25);
        }
    }
    return 0;
}
#include <display.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#define DELAY 1000

int main() {
    initDisplay();

    for (int i = 0; i < DELAY / 5; i++) {
        writeCharToSegment(0, 'A');
        _delay_ms(5);
    }
    clearDisplay();
    _delay_ms(DELAY);
    for (int i = 0; i < DELAY / 5; i++) {
        _delay_ms(5);
        writeString("ABCD");
    }
    clearDisplay();
    _delay_ms(DELAY);
    writeStringAndWait("EFGH", DELAY);
    clearDisplay();

    return 0;
}
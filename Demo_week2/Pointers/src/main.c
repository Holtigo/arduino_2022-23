#include <avr/io.h>
#include <util/delay.h>
#define LOW 0
#define HIGH 1

void writeMask(volatile uint8_t *registerPtr, uint8_t bitmask) {
    *registerPtr = bitmask;
}

/* write value LOW (0) or HIGH (1) to specific digital pin on specific register */
void writePin(volatile uint8_t *registerPtr, uint8_t pin, uint8_t value) {
    if (value == LOW) {
        // Clear the bit corresponding to the pin
        *registerPtr &= ~(1 << pin);
    } else if (value == HIGH) {
        // Set the bit corresponding to the pin
        *registerPtr |= (1 << pin);
    }
}

int main() {
    uint8_t bitmask = (1 << (PB2 + 2));  // maak bitmask aan
    writeMask(&DDRB, bitmask);           // schrijf bitmask naar register
    writePin(&PORTB, PB4, HIGH);         // schrijf HIGH naar pin PB2
    bitmask = (0 << (PC1));              // maak bitmask aan
    writeMask(&DDRC, bitmask);
    writePin(&PORTC, PC1, HIGH);
    while (1) {
        if (bit_is_clear(PINC, PC1)) {
            writePin(&PORTB, PB4, LOW);
        } else {
            writePin(&PORTB, PB4, HIGH);
        }
        _delay_ms(25);
    }
    return 0;
}
#include <avr/io.h>
#include <avr/interrupt.h>

void initTimer2(uint8_t mode, uint8_t interrupts) {
    // Mode of operation
    // Check https://www.ermicro.com/blog/wp-content/uploads/2009/01/m168lcd_07.jpg
    if (mode > 11) {
        return;  // Mode does not exist
    }
    if (mode >= 8) {
        TCCR2B |= 8;  // Set WGM22 bit for advanced mode
        mode -= 8;
    }
    TCCR2A |= mode;  // Set WGM21 and WGM20 bits to mode

    // Interrupts
    // Check https://web.ics.purdue.edu/~jricha14/Timer_Stuff/TIMSK.htm
    if (interrupts > 3) {
        return;  // Interrupt does not exist
    }
    if (interrupts == 1 || interrupts == 3) {
        TIMSK2 |= _BV(OCIE2A);  // Enable OCRA interrupt
    }
    if (interrupts == 2 || interrupts == 3) {
        TIMSK2 |= _BV(TOIE2);  // Enable overflow interrupt
    }

    sei();  // Enable global interrupts
}

void startTimer2(uint8_t prescaler) {
    // Set prescaler
    // Check https://onlinedocs.microchip.com/pr/GUID-0EC909F9-8FB7-46B2-BF4B-05290662B5C3-en-US-12.1.1/index.html?GUID-2A013642-EB1B-4283-9756-C2449EF0A5C2
    if (prescaler > 7) {
        return;  // Prescaler does not exist
    }
    TCCR2B |= prescaler;  // Set prescaler
}

void stopTimer2() {
    // Set prescaler to 0
    TCCR2B &= ~7;  // Set prescaler to 0
}
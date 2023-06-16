#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "button.h"
#include "display.h"
#include "timer.h"
#include "usart.h"
#define VEELVOUD 250
// globale variabelen
uint8_t counter = 0;
uint8_t seconden = 50;
uint8_t minuten = 0;
uint8_t startCounting = 0;
// plaats hier je globale variabelen
// void initTimer() {
//     // kies de WAVE FORM en dus de Mode of Operation
//     // Hier kiezen we FAST PWM waardoor de TCNT0 steeds tot 255 telt
//     TCCR2A |= _BV(WGM21);  // WGM21 = 1 --> Clear Timer on Compare (CTC) mode

//     // enable INTERRUPTs
//     // Enable interrupts voor een geval: TCNT2 == OCR2A
//     TIMSK2 |= _BV(OCIE2A);  // OCRA interrupt enablen

//     sei();  // interrupts globaal enablen

//     OCR2A = 249;  // 16.000.000 / 256 / 250 = 250Hz
// }
// void startTimer() {
//     // vul aan
//     // stel *altijd* een PRESCALER in, anders telt hij niet.
//     // De snelheid van tellen wordt bepaald door de CPU-klok (16Mhz) gedeeld door deze factor.
//     TCCR2B |= _BV(CS22) | _BV(CS21);  // prescaler = 256
// }
// void stopTimer() {
//     // vul aan
//     // STAP 2: stel *altijd* een PRESCALER in, anders telt hij niet.
//     // De snelheid van tellen wordt bepaald door de CPU-klok (16Mhz) gedeeld door deze factor.
//     TCCR2B &= ~_BV(CS22) | ~_BV(CS21);  // prescaler = 0
// }
// deze functie moet elke seconde opgeroepen worden
void tick() {
    // bereken seconden en minuten
    // en stockeer ze in globale variabele seconden en minuten.
    // als seconden == 60, dan reset je seconden naar 0 en verhoog je minuten met 1.
    if (seconden == 60) {
        seconden = 0;
        minuten++;
    }
}
void writeTimeAndWait(uint8_t minuten, uint8_t seconden, int delay) {
    // Vul aan
    if (minuten < 0 || minuten > 99 || seconden < 0 || seconden > 99) return;
    for (int i = 0; i < delay / 20; i++) {
        writeNumberToSegment(0, (minuten / 10) % 10);
        _delay_ms(5);
        writeNumberToSegment(1, minuten % 10);
        _delay_ms(5);
        writeNumberToSegment(2, (seconden / 10) % 10);
        _delay_ms(5);
        writeNumberToSegment(3, seconden % 10);
        _delay_ms(5);
    }
}
// Deze ISR loopt elke 4ms
ISR(TIMER2_COMPA_vect) {
    // verhoog counter met 1
    // als counter + 1 deelbaar is door VEELVOUD tel één seconde.
    if ((counter + 1) == VEELVOUD) {
        seconden++;
        counter = 0;
        tick();
    }
    counter++;
}

ISR(PCINT1_vect) {
    if (buttonPushed(0) == 1) {
        if (startCounting == 0) {
            seconden = 50;
            minuten = 0;
            startCounting = 1;
            startTimer2(6);
        }
    }
    if (buttonPushed(1) == 1) {
        if (startCounting == 1) {
            startCounting = 0;
            stopTimer2();
            clearDisplay();
        }
    }
    if (buttonPushed(2) == 1) {
        seconden = 0;
        minuten = 0;
        clearDisplay();
    }
}

int main() {
    initUSART();
    initDisplay();
    enableAllButtons();
    enableAllButtonInterrupts();
    printf("Start de stopwatch met S1, stop met S2, en reset met S3\n");

    // initTimer();
    initTimer2(2, 1);
    OCR2A = 249;
    startTimer2(6);
    while (1) {
        if (startCounting == 1) {
            printf("%d minuten : %d seconden \n", minuten, seconden);
            writeTimeAndWait(minuten, seconden, 1000);
        } else {
            printf("not counting \n");
            clearDisplay();
            _delay_ms(1000);
        }
    }
    return 0;
}
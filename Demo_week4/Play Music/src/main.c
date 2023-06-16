#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <led.h>
#include <usart.h>
#include <util/delay.h>

// DURATION IS IN MILLISECONDEN
#define DURATION 200

// FREQUENCIES VAN DE VERSCHILLENDE NOTEN
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.000
#define B5 987.770
#define C6 1046.500

void enableBuzzer() { DDRD |= (1 << PD3); }  // buzzer hangt op PD3

void playTone(float frequency, uint32_t duration) {
    uint32_t periodInMicro = (uint32_t)(1000000 / frequency);                 // we berekenen de periode in microseconden uit de frequency
    uint32_t durationInMicro = duration * 1000;                               // we willen duration in microseconden
    for (uint32_t time = 0; time < durationInMicro; time += periodInMicro) {  // Zie tutorial over muziek voor meer uitleg!
        PORTD &= ~(1 << PD3);                                                 // aanzetten van de buzzer
        _delay_us(periodInMicro / 2);                                         // helft van de periode wachten
        PORTD |= (1 << PD3);                                                  // afzetten van de buzzer
        _delay_us(periodInMicro / 2);                                         // weer helft van de periode wachten
    }
}

int main() {
    initUSART();
    uint32_t frequencies[] = {C5, D5, E5, F5, G5, A5, B5, C6};  // do-re-mi...
    enableBuzzer();
    enableLed(0);
    lightDownLed(0);
    for (int note = 0; note < 8; note++) {
        printf("Period: %lu\n", frequencies[note]);
        playTone(frequencies[note], DURATION);
        lightUpLed(0);
        _delay_ms(DURATION);
        lightDownLed(0);
    }

    return 0;
}
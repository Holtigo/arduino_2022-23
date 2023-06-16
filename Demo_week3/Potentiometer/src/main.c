#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>

void initADC() {
    ADMUX |= (1 << REFS0);                                 // instellen van de reference voltage. We kiezen op deze manier voor de 5V als reference voltage
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // bepalen van de samplerate door een deelfactor in te stellen. Deelfactor hier: 128
    ADCSRA |= (1 << ADEN);                                 // Enable de ADC
}



int main() {
    initUSART();
    initADC();
    while (1) {
        ADCSRA |= (1 << ADSC);                  // start de conversie analoog -> digitaal
        loop_until_bit_is_clear(ADCSRA, ADSC);  // wacht tot conversie gebeurd is
        uint16_t value = ADC;                   // lees het resultaat uit
        printf("Value:%d\n", value);
    }
    return 0;
}
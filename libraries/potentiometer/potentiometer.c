#define __DELAY_BACKWARD_COMPATIBLE__
#include "potentiometer.h"
#include <avr/io.h>
#include <util/delay.h>

void initADC() {
  ADMUX |= (1 << REFS0);  // instellen van de reference voltage. We kiezen op deze manier voor de 5V als reference voltage
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // bepalen van de samplerate door een deelfactor in te stellen. Deelfactor hier: 128
  ADCSRA |= (1 << ADEN);  // Enable de ADC
}

void initADC1V1() {
    ADMUX |= (1 << REFS0) | (1 << REFS1);                  // instellen van de reference voltage. We kiezen op deze manier voor de 1.1V als reference voltage
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // bepalen van de samplerate door een deelfactor in te stellen. Deelfactor hier: 128
    ADCSRA |= (1 << ADEN);                                 // Enable de ADC
}

uint16_t readPotmeterValue() {
  ADCSRA |= (1 << ADSC);                    // start de conversie analoog -> digitaal
  loop_until_bit_is_clear(ADCSRA, ADSC);    // wacht tot conversie gebeurd is
  return ADC;                               // return het resultaat
}

#include <avr/io.h>
void initTimer2(uint8_t mode, uint8_t interrupts);
void startTimer2(uint8_t prescaler);
void stopTimer2();
#include <avr/interrupt.h>
#include <avr/io.h>
#include <button.h>
#include <led.h>
#include <util/delay.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5



void handleButtonPress(int button, int led) {
    
    if (buttonPushed(button) == 1) {
        toggelLed(led);
    }
}

ISR(PCINT1_vect) {
    handleButtonPress(0, 1);
    handleButtonPress(1, 2);
    handleButtonPress(2, 3);
}

int main() {
                                   // pull up aanzetten
    enableAllButtons();

    enableAllButtonInterrupts();  // Set Enable Interrupts --> globaal interrupt systeem aanzetten
    enableAllLeds();
    lightDownAllLeds();
    while (1) {
        // LED_PORT &= ~_BV(LED1);  // led1 aanzetten
        lightUpLed(0);
        _delay_ms(100);
        // LED_PORT |= _BV(LED1);  // led1 uitzetten
        lightDownLed(0);
        _delay_ms(100);
    }
    return 0;
}
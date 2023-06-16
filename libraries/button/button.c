#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define NUMBER_OF_BUTTONS 3

void enableButton(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
        return;
    BUTTON_DDR &= ~_BV(PC1 + button);  // zet DDR op input (0)
    BUTTON_PORT |= _BV(PC1 + button);  // pull up aanzetten (1)
}

void enableButtons(uint8_t buttons) {
    BUTTON_DDR = (~buttons << PC1);
    BUTTON_PORT = (buttons << PC1);
}

void enableAllButtons() {
    enableButtons(0b00000111);
}

int buttonPushed(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
        return;
    if (bit_is_clear(BUTTON_PIN, PC1 + button)) {
        _delay_us(1000);
        if (bit_is_clear(BUTTON_PIN, PC1 + button)) {
            return 1;
        }
    }
    return 0;
}

int buttonReleased(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
        return;
    if (bit_is_clear(BUTTON_PIN, PC1 + button)) {
        _delay_us(1000);
        if (bit_is_set(BUTTON_PIN, PC1 + button)) {
            return 1;
        }
    }
    return 0;
}

// int buttonPushed(int button){
//     if (bit_is_clear(BUTTON_PIN, PC1 + button)) //(PINC & (1 << PC1 == 0))
//         return 1;
//     else
//         return 0;
// }

// int buttonReleased(int button){
//     if(buttonPushed(button) == 0)
// 		return 1;
// 	else
// 		return 0;
// }


void enableButtonInterrup(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
        return;
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PC1 + button);
    sei();
}


void enableAllButtonInterrupts() {
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PC1) | (1 << PC2) | (1 << PC3);
    sei();
}
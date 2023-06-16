
#include <avr/io.h>
#include <led.h>
#include <stdlib.h>
#include <string.h>
#include <usart.h>
#include <util/delay.h>

const char morse[36][6] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--..",  // Z

    "-----",  // 0
    ".----",  // 1
    "..---",  // 2
    "...--",  // 3
    "....-",  // 4
    ".....",  // 5
    "-....",  // 6
    "--...",  // 7
    "---..",  // 8
    "----."   // 9
};

void startUpSequence() {
    lightUpAllLeds();
    _delay_ms(1000);
    fadeOutAllLeds(1, 1000);
    _delay_ms(2000);
}

void stopSequence() {
    lightUpAllLeds();
    _delay_ms(1000);
    fadeOutAllLeds(0, 1000);
    _delay_ms(2000);
}

void showMorse(const char c[]) {
    for (int i = 0; i < strlen(c); i++) {
        if (c[i] == '.') {
            lightUpAllLeds();
            _delay_ms(250);
            lightDownAllLeds();
            _delay_ms(250);
        } else if (c[i] == '-') {
            lightUpAllLeds();
            _delay_ms(600);
            lightDownAllLeds();
            _delay_ms(250);
        }
    }
}

int main() {
    // initialize usart
    initUSART();
    // initialize leds
    enableAllLeds();
    lightDownAllLeds();
    // Initialize random generator
    srand(time(0));

    startUpSequence();
    for (int i = 0; i < 10; i++) {
        int random = rand() % 36;
        showMorse(morse[random]);
        _delay_ms(3000);
        // 65 = A, 48 = 0
        printf("%d: %c \n", i + 1, random > 25 ? ((random - 25) + 48) : (random + 65));
        _delay_ms(2000);
    }
    _delay_ms(2000);
    stopSequence();
    return 0;
}
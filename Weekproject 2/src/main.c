#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <button.h>
#include <led.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <util/delay.h>

#define NUMBERAMOUNT 10
#define DELAY 380

int button_pushed = 0;

ISR(PCINT1_vect) {
    if (buttonPushed(0)) {
        button_pushed = 1;
    }
}

void generatePuzzle(uint8_t* numbers, int size) {
    for (int i = 0; i < size; i++) {
        numbers[i] = rand() % 3;
    }
}

void printPuzzle(uint8_t* numbers, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("]\n");
}

void playPuzzle(uint8_t* numbers, int size) {
    for (int i = 0; i < size; i++) {
        lightUpLed(numbers[i]);
        _delay_ms(DELAY);
        lightDownLed(numbers[i]);
        _delay_ms(DELAY);
    }
}

int readInput(uint8_t* numbers, int size) {
    int counter = 0;
    while (counter < size) {
        for (int i = 0; i < 3; i++) {
            if (buttonPushed(i)) {
                lightUpLed(i);
                _delay_ms(500);
                lightDownLed(i);
                if (i == numbers[counter]) {
                    printf("Je drukte op knop %d, correct!\n", i + 1);
                    counter++;
                } else {
                    printf("Je drukte op knop %d, fout!\n De juiste reeks was [ ", i + 1);
                    for (int j = 0; j < counter; j++) {
                        printf("%d ", numbers[j]);
                    }
                    printf("]\n");
                    return 0;
                }
            }
        }
    }
    return 1;
}

void correctCombination() {
    for (int i = 0; i < 3; i++) {
        lightUpLed(3);
        _delay_ms(100);
        lightDownLed(3);
        _delay_ms(100);
    }
}

int main() {
    int seed = 0;
    uint8_t numbers[NUMBERAMOUNT];
    int level = 1;
    int correct = 1;
    initUSART();
    enableAllLeds();
    lightDownAllLeds();
    enableAllButtons();
    enableAllButtonInterrupts();
    printf("Druk op knop 1 om te beginnen!\n");
    while (!button_pushed) {
        toggelLed(3);
        _delay_ms(100);
        seed += 100;
    }

    lightDownAllLeds();
    srand(seed);
    generatePuzzle(numbers, NUMBERAMOUNT);
    printPuzzle(numbers, NUMBERAMOUNT);
    _delay_ms(1000);
    playPuzzle(numbers, NUMBERAMOUNT);

    while (level != 11) {
        printf("Level %d!\n", level);
        playPuzzle(numbers, level);
        printf("Jouw beurt!\n");
        correct = readInput(numbers, level);
        _delay_ms(400);
        if (correct) {
            level++;
            if (level == 11) {
                printf("\n========================\nJe hebt het spel gewonnen!\n");
                break;
            }
            correctCombination();
            printf("Goed gedaan! Op naar level %d!\n", level);
            _delay_ms(500);
        } else {
            printf("Je hebt het spel verloren!\n Het patroon was: ");
            printPuzzle(numbers, NUMBERAMOUNT);
            break;
        }
    }

    return 0;
}
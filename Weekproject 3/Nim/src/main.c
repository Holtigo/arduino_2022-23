#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "button.h"
#include "display.h"
#include "potentiometer.h"
#include "usart.h"

#define TRUE 1
#define FALSE 0
#define MAXAANTAL 3
#define STARTAMOUNT 21
#define MAX 3
#define MIN 1
uint8_t seedSelected = FALSE;
uint8_t gameStart = FALSE;
uint8_t availableAmount = STARTAMOUNT;
char turn = 'P';
char win = 'A';
uint8_t choice = 0;
uint8_t *turns;
uint8_t *amounts;
uint8_t *rest;
uint8_t counter = 0;

ISR(PCINT1_vect) {
    if (buttonPushed(0) == TRUE) {
        if (seedSelected == FALSE && gameStart == FALSE) {
            seedSelected = TRUE;
            gameStart = TRUE;
        } else if (gameStart == TRUE && choice > MIN && turn == 'P') {
            choice--;
        }
    }

    if (buttonPushed(1) == TRUE) {
        if (gameStart == TRUE) {
            if (turn == 'P' && choice > 0) {
                turns[counter] = turn;
                amounts[counter] = choice;
                availableAmount -= choice;
                rest[counter] = availableAmount;
                printf("%d\n", choice);
                turn = 'C';
                choice = 0;
                counter++;
                if (availableAmount == 0) {
                    gameStart = FALSE;
                    win = 'C';
                }
                _delay_ms(100);
            } else if (turn == 'C') {
                if (choice == 0) {
                    choice = (availableAmount - 1) % (MAX + 1);
                    if (choice == 0) {
                        choice = rand() % MAX + 1;
                    }
                } else if (choice > 0) {
                    turns[counter] = turn;
                    amounts[counter] = choice;
                    availableAmount -= choice;
                    rest[counter] = availableAmount;
                    printf("%d\n", choice);
                    turn = 'P';
                    choice = 0;
                    counter++;
                    if (availableAmount == 0) {
                        gameStart = FALSE;
                        win = 'P';
                    }
                    _delay_ms(100);
                }
            }
        }
    }

    if (buttonPushed(2) == TRUE) {
        if (gameStart == TRUE && choice < MAX && turn == 'P') {
            choice++;
        }
    }
}

int main() {
    initUSART();
    initADC1V1();
    initDisplay();
    enableAllButtons();
    enableAllButtonInterrupts();
    uint16_t potentiometerValue = 0;
    uint16_t seed = 0;

    // allocation
    turns = calloc(STARTAMOUNT, sizeof(char));
    amounts = calloc(STARTAMOUNT, sizeof(uint8_t));
    rest = calloc(STARTAMOUNT, sizeof(uint8_t));

    while (seedSelected == FALSE) {
        potentiometerValue = readPotmeterValue();
        seed = (9999 / 1023) * potentiometerValue;
        writeNumberAndWait(seed, 25);
    }

    srand(seed);
    if (rand() % 2 == 1) {
        turn = 'P';
    } else {
        turn = 'C';
    }

    while (gameStart == TRUE) {
        for (int i = 0; i < 10000; i++) {
            if (i < 6000) {
                writeCharToSegment(1, turn);
            } else {
                writeCharToSegment(1, ' ');
            }
            writeNumberToSegment(0, choice);
            // _delay_ms(2);
            write2NumbersRLED(availableAmount);
        }
    }

    for (uint8_t i = 0; i < counter; i++) {
        printf("%c Took %d match(es) on turn %d (rest: %d)\n", turns[i], amounts[i], i + 1, rest[i]);
    }

    free(turns);
    free(amounts);
    free(rest);

    if (win == 'P') {
        printf("You won!\n");
    } else {
        printf("You lost!\n");
    }

    return 0;
}
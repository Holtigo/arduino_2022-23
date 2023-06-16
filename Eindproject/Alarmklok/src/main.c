#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "button.h"
#include "buzzer.h"
#include "display.h"
#include "led.h"
#include "potentiometer.h"
#include "timer.h"
#include "usart.h"

#define TRUE 1
#define FALSE 0
#define MAXCOUNT 250

typedef struct
{
    int hours;
    int minutes;
} KLOK;

typedef struct
{
    int hours;
    int minutes;
} ALARM;

uint8_t selectklokOrAlarm = 3;
// FALSE = klok en TRUE = alarm
uint8_t klokAlarmSet = FALSE;
// if TRUE, when button 2 is pushed, klok or alarm is selected and ready to set time
uint8_t alarmSet = FALSE;
// alarm time has been set if TRUE
uint8_t klokSet = FALSE;
// klok time has been set if TRUE
uint8_t selectSetTime = 0;
// Menu item selected
// 0 = menu, 1 = klok, 2 = alarm
uint8_t alarmArmed = FALSE;
// If klokSet and alarmSet are TRUE, alarmArmed is TRUE and timer starts
uint8_t alarmOn = FALSE;
// Extra check to see if alarm is on and regulate ISR(PCINT1_vect)
uint8_t alarmEnd = FALSE;
// If TRUE, alarm has ended
uint8_t counter = 0;
// Counter for ISR(TIMER0_COMPA_vect)
uint8_t secondCounter = 0;
// Counter for seconds
uint8_t minuteCounter = 0;
// Counter for minutes
uint8_t showCounter = FALSE;
// if TRUE, while alarm is on, klok is displayed, if FALSE, alarm is displayed
uint8_t stopSong = FALSE;
// if TRUE, song is stopped
ALARM alarm;
KLOK klok;

void setKlokHours(KLOK* klok) {
    if (buttonPushed(0) == TRUE) {
        klok->hours++;
        if (klok->hours > 23) {
            klok->hours = 0;
        }
        _delay_ms(100);
    }
    if (buttonPushed(2) == TRUE) {
        klok->hours--;
        if (klok->hours < 0) {
            klok->hours = 23;
        }
        _delay_ms(100);
    }
}

void setAlarmHours(ALARM* alarm) {
    if (buttonPushed(0) == TRUE) {
        alarm->hours++;
        if (alarm->hours > 23) {
            alarm->hours = 0;
        }
        _delay_ms(250);
    }
    if (buttonPushed(2) == TRUE) {
        alarm->hours--;
        if (alarm->hours < 0) {
            alarm->hours = 23;
        }
        _delay_ms(250);
    }
}

ISR(PCINT1_vect) {
    if (buttonPushed(0) == TRUE) {
        if (selectSetTime == 0 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE && alarmEnd == FALSE) {
            selectSetTime = 1;
            _delay_ms(100);
            // Cycle right through menu items, displays klok
        } else if (selectSetTime == 1 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE) {
            selectSetTime = 2;
            _delay_ms(100);
            // Cycle right through menu items, displays alarm
        } else if (selectSetTime == 2 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE && alarmEnd == FALSE) {
            selectSetTime = 0;
            _delay_ms(100);
            // Cycle right through menu items, displays menu
        } else if (alarmEnd == TRUE && alarmOn == FALSE && alarmArmed == FALSE && stopSong == FALSE) {
            stopSong = TRUE;
            // Used to stop song when alarm has ended
        }
    }

    if (buttonPushed(1) == TRUE) {
        if (selectSetTime == 0 && klokSet == TRUE && alarmSet == TRUE && alarmOn == FALSE && alarmEnd == FALSE) {
            alarmArmed = TRUE;
            // If klok and alarm are set, alarmArmed is TRUE and timer starts
        }
        if (selectSetTime == 1 && selectklokOrAlarm == 3) {
            selectklokOrAlarm = FALSE;
            klokAlarmSet = TRUE;
            // If klok is selected, selectklokOrAlarm is FALSE and klokAlarmSet is TRUE
        } else if (selectSetTime == 2 && selectklokOrAlarm == 3) {
            selectklokOrAlarm = TRUE;
            klokAlarmSet = TRUE;
            // If alarm is selected, selectklokOrAlarm is TRUE and klokAlarmSet is TRUE
        } else if (selectSetTime == 1 && selectklokOrAlarm == FALSE) {
            selectSetTime = 1;
            selectklokOrAlarm = 3;
            klokAlarmSet = FALSE;
            klokSet = TRUE;
            // Exit klok set menu and set klokSet to TRUE
        } else if (selectSetTime == 2 && selectklokOrAlarm == TRUE) {
            selectSetTime = 2;
            selectklokOrAlarm = 3;
            klokAlarmSet = FALSE;
            alarmSet = TRUE;
            // Exit alarm set menu and set alarmSet to TRUE
        } else if (alarmArmed == TRUE && alarmOn == TRUE && showCounter == FALSE) {
            clearDisplay();
            showCounter = TRUE;
            _delay_ms(100);
            // If alarm is being displayed, show klok
        } else if (alarmArmed == TRUE && alarmOn == TRUE && showCounter == TRUE) {
            clearDisplay();
            showCounter = FALSE;
            _delay_ms(100);
            // If klok is being displayed, show alarm
        } else if (alarmEnd == TRUE && alarmOn == FALSE && alarmArmed == FALSE && stopSong == FALSE) {
            stopSong = TRUE;
            // Used to stop song when alarm has ended
        }
    }

    if (buttonPushed(2) == 1) {
        if (selectSetTime == 0 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE && alarmEnd == FALSE) {
            selectSetTime = 2;
            _delay_ms(100);
            // Cycle left through menu items, displays alarm
        } else if (selectSetTime == 1 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE && alarmEnd == FALSE) {
            selectSetTime = 0;
            _delay_ms(100);
            // Cycle left through menu items, displays menu
        } else if (selectSetTime == 2 && klokAlarmSet == FALSE && alarmArmed == FALSE && alarmOn == FALSE) {
            selectSetTime = 1;
            _delay_ms(100);
            // Cycle left through menu items, displays klok
        } else if (alarmEnd == TRUE && alarmOn == FALSE && alarmArmed == FALSE && stopSong == FALSE) {
            stopSong = TRUE;
            // Used to stop song when alarm has ended
        }
    }
}

ISR(TIMER2_COMPA_vect) {
    if (alarmArmed == TRUE && alarmEnd == FALSE) {
        if ((counter + 1) == MAXCOUNT) {
            toggelLeds(15);
            secondCounter++;
            counter = 0;
            // Count 250 steps at 250Hz, 1 second
            // Toggles LEDs every second
        }
        if (alarm.hours == 0 && alarm.minutes == 0 && secondCounter == 60) {
            alarmEnd = TRUE;
            alarmOn = FALSE;
            alarmArmed = FALSE;
            // if alarm has 0 minutes and 0 hours, alarmEnd is TRUE and alarmOn and alarmArmed are FALSE
        }
        if (secondCounter == 60) {
            secondCounter = 0;
            klok.minutes++;
            if (klok.minutes > 59) {
                klok.minutes = 0;
                klok.hours++;
                if (klok.hours > 23) {
                    klok.hours = 0;
                }
                // If secondCounter is 60, secondCounter is set to 0 and klok.minutes is incremented
                // If klok.minutes is 60, klok.hours is incremented
                // If klok.hours is 24, klok.hours is set to 0
            }
            alarm.minutes--;
            if (alarm.minutes < 0 && alarm.hours != 0) {
                alarm.minutes = 59;
                alarm.hours--;
            }
            // If secondCounter is 60, secondCounter is set to 0 and alarm.minutes is decremented
            // If alarm.minutes is 0 and alarm.hours is not 0, alarm.minutes is set to 59
            // and alarm.hours is decremented
        }
        counter++;
    } else if (alarmEnd == TRUE && stopSong == FALSE) {
        if ((counter + 1) == MAXCOUNT) {
            secondCounter++;
            counter = 0;
            // Count 250 steps at 250Hz, 1 second
        }
        counter++;
        if (secondCounter == 60) {
            stopSong = TRUE;
            // Used to stop song automatically after 1 minute
        }
    }
}

void printInstructions() {
    // print introduction / instructions
    printf("=========================================================\n");
    printf("Welcome to the alarm clock!\n\n");
    printf("Button 1 and 3 to cycle right and left respectively.\n");
    printf("Button 2 to select item.\n");
    printf("Button 1 and 3 to cycle up and down through hours.\n");
    printf("Potentiometer to cycle through minutes.\n");
    printf("Button 2 to confirm time.\n");
    printf("If Clock and Alarm are set, return to 'MENU' and\n");
    printf("press button 2 to arm the alarm.\n");
    printf("Use button 2 to cycle through the clock and alarm.\n");
    printf("When the alarm goes off, press any button to stop it.\n");
    printf("=========================================================\n");
}

float notes[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88};
// Notes for the alarm

void alarmFinish() {
    for (int i = 0; i < sizeof(notes) / sizeof(float); i++) {
        playTone(notes[i], 350);
        toggelLeds(15);
    }
    turnOffBuzzer();
    // Plays the alarm
    // Toggles LEDs every note
}

int main() {
    // Initialize all modules
    initUSART();
    initTimer2(2, 1);
    OCR2A = 249;
    enableAllButtons();
    enableAllButtonInterrupts();
    enableAllLeds();
    lightDownAllLeds();
    enableBuzzer();
    turnOffBuzzer();
    initDisplay();
    clearDisplay();
    initADC1V1();

    // Initialize variables
    alarm.hours = 0;
    alarm.minutes = 0;
    klok.hours = 0;
    klok.minutes = 0;
    uint16_t* potmeterValue = calloc(1, sizeof(uint16_t));
    uint8_t* minutes = calloc(1, sizeof(uint8_t));
    printInstructions();
    while (alarmOn == FALSE) {
        if (selectSetTime == 0 && alarmArmed == FALSE && alarmOn == FALSE) {
            writeStringAndWait("MENU", 25);
        }
        if (selectSetTime == 1 && selectklokOrAlarm == 3) {
            writeStringAndWait("KLOK", 25);
        }
        if (selectSetTime == 2 && selectklokOrAlarm == 3) {
            writeStringAndWait("ALAR", 25);
        }
        clearDisplay();
        if (selectSetTime == 1 && selectklokOrAlarm == FALSE) {
            write2NumbersLLED(klok.hours);
            write2NumbersRLED(klok.minutes);
            *potmeterValue = readPotmeterValue();
            *minutes = *potmeterValue / 17;
            if (*minutes > 59) {
                *minutes = 59;
            }
            klok.minutes = *minutes;
            setKlokHours(&klok);
        }
        if (selectSetTime == 2 && selectklokOrAlarm == TRUE) {
            write2NumbersLLED(alarm.hours);
            write2NumbersRLED(alarm.minutes + 1);
            *potmeterValue = readPotmeterValue();
            *minutes = *potmeterValue / 17;
            if (*minutes > 59) {
                *minutes = 59;
            }
            alarm.minutes = *minutes;
            setAlarmHours(&alarm);
        }
        if (selectSetTime == 0 && alarmArmed == TRUE && alarmOn == FALSE) {
            writeStringAndWait("INIT", 25);
            startTimer2(6);
            alarmOn = TRUE;
            free(potmeterValue);
            free(minutes);
        }
    }
    printf("Alarm on!\n");
    while (alarmEnd == FALSE) {
        if (alarmArmed == TRUE && alarmOn == TRUE && showCounter == FALSE) {
            write2NumbersLLED(alarm.hours);
            write2NumbersRLED(alarm.minutes);
        }
        if (alarmArmed == TRUE && alarmOn == TRUE && showCounter == TRUE) {
            write2NumbersLLED(klok.hours);
            write2NumbersRLED(klok.minutes);
        }
    }
    while (stopSong == FALSE) {
        alarmFinish();
    }
    printf("Alarm end\n");
    return 0;
}
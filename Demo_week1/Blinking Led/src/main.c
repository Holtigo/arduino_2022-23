#define __DELAY_BACKWARD_COMPATIBLE__  // laat toe om een variabele te gebruiken in _delay_ms(..)
// De #include regels gebruik je om "C-libraries" te importeren
// Binnen dit vak gaan we zoveel mogelijk onze libraries zelf schrijven, maar een paar bestaande libraries kunnen we echt niet missen
#include <avr/io.h>  //in deze library worden namen gegeven aan de verschillende registers van de ATMega328P microcontroller zoals DDRB
#include <led.h>
#include <util/delay.h>  //deze library bevat onder andere de functie _delay_ms(..) om de uitvoering van je programma even te pauzeren

#define NUMBER_OF_LEDS 4  // We gebruiken een define om het aantal leds dat we gebruiken te definiëren

int main()  // de start van onze applicatie
{
    // DDRB = 0b00100000;  // We schrijven een binair getal naar Data Direction Register B. Hierdoor wordt schrijven ge-activeerd op pin 5 van port B.
    // DDRB |= 0b00100000; //Beter doen we een bitwise OR, zodat we eventuele 1-tjes die op andere posities van DDRB stonden niet wissen!
    // DDRB |= (1 << 5); //Door gebruik te maken van de << bitshift operator kunnen we het binair getal eenvoudiger aanmaken
    // DDRB |= (1 << (PB2 + 3));  // We gebruiken de poortbenamingen uit io.h om onze code leesbaarder te maken
    enableAllLeds();
    lightDownAllLeds();
    lightUpLed(0);
    _delay_ms(1000);
    lightUpLeds(0b00001100);
    _delay_ms(1000);
    dimLed(1, 10, 2000);
    _delay_ms(1000);
    fadeOutAllLeds(1,2000);

    // while (1) {
    //     for (int i = 0; i <= 100; i += 5) {
    //         dimLed(0, i, 75);
    //         dimLed(1, i, 75);
    //         dimLed(2, i, 75);
    //         dimLed(3, i, 75);
    //     }

    //     for (int i = 100; i >= 0; i -= 5) {
    //         dimLed(0, i, 75);
    //         dimLed(1, i, 75);
    //         dimLed(2, i, 75);
    //         dimLed(3, i, 75);
    //     }
    //     lightDownAllLeds();
    // }

    return 0;
}
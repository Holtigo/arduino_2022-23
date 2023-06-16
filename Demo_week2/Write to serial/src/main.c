#include <util/delay.h>
#include <avr/io.h>
#include <usart.h> //Deze preprocessor directive zorgt ervoor dat de functiedefinities van de usart library geladen worden. Check de tutorial van week 1: "3.5 Een eigen library gebruiken".

int main(){
    DDRB |= (1 << PB2);
    initUSART();//Hier roepen we een functie van usart.h aan. Deze functie initialiseert de communicatie met de seriële poort.
    int counter = 0;
    while (counter < 50) {
        PORTB = (1 << PB2);
        _delay_ms(100);
        PORTB = (0 << PB2);
        _delay_ms(100);
        printString("Debugging!!"); //Hier roepen we een functie van usart.h aan. Als alles goed is verschijnt deze tekst onderaan in de Serial Monitor
        printf("Countertje: %d\n", counter); //Hier roepen we de standaard C functie printf aan, de heel gelijkaardig is aan de System.out.printf uit java
        counter++;
    }
    return 0;
}
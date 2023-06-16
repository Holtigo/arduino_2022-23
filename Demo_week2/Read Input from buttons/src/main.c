#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <button.h>

int main() {
    initUSART();
    // DDRC &= ~(1 << PC1);  // Knop 1 hangt aan C1, we zetten in het Data Direction Register de overeenkomstige bit op 0
    //        0 0 0 1 1 0 1 0
    //     ~  0 0 0 0 0 0 1 0
    //        ---------------
    //        0 0 0 1 1 0 1 0
    // &      1 1 1 1 1 1 0 1
    //        ---------------
    //        0 0 0 1 1 0 0 0
    enableButton(0);
    printf("DDRC: ");
    printBinaryByte(DDRC);
    // PORTC |= (1 << PC1);  // Pull up aanzetten van C1: PINC komt op high te staan
    printf("\nPORTC: ");
    printBinaryByte(PORTC);
    while (1) {
        printf("\nPINC: ");
        printBinaryByte(PINC);
        if (buttonReleased(0) == 1) {  // op deze manier kunnen we testen of de PC1-e bit van PINC op 0 staat (knop ingeduwd)).
            printf(" - Button 0 released!\n");
            return;
        } else {
            printf(" - Button 0 NOT released!!\n");
        }
        _delay_ms(1000);
    }
    return 0;
}

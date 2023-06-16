#include <display.h>
#include <stdio.h>
#include <stdlib.h>

#define DELAY 1000
#define DURATION 10000

int main() {
    initDisplay();
    uint32_t number = 1234567890;
    writeScrollingNumber(number, DELAY, DURATION);
    clearDisplay();

    return 0;
}

// #include <avr/io.h>
// #include <display.h>
// #include <usart.h>
// #include <util/delay.h>

// int main() {
//   initUSART();
//   initDisplay();

//   int getallen[] = {1234, 2345, 3456, 4567, 5678, 6789, 7890, 8901, 9012};

//   while (1) {
//     for (int i = 0; i < 10; i++) {
//       writeNumberAndWait(getallen[i], 500);
//     }
//     for (int i = 0; i < 500 / 20; i++) {
//       writeNumberToSegment(0, 0);
//       _delay_ms(5);
//       writeNumberToSegment(1, 1);
//       _delay_ms(5);
//       writeNumberToSegment(2, 2);
//       _delay_ms(5);
//       writeNumberToSegment(3, 3);
//       _delay_ms(5);
//     }
//   }
// }
void enableLed(int ledNumber);

void enableLeds(uint8_t leds);

void enableAllLeds();

void lightUpLed(int ledNumber);

void lightUpLeds(uint8_t leds);

void lightUpAllLeds();

void lightDownLed(int ledNumber);

void lightDownLeds(uint8_t leds);

void lightDownAllLeds();

// 0b00000000 --> 1 toggles status led
void toggelLeds(uint8_t leds);

void toggelLed(int ledNumber);

void dimLed(int lednumber, int percentage, int duration);

void fadeInLed(int led, int duration);

void fadeOutLed(int led, int duration);

void fadeOutAllLeds(int direction, int duration);

int isLedOn(int ledNumber);
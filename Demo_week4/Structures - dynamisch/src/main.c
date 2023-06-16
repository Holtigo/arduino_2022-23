#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <usart.h>
#include <util/delay.h>

#define AANTAL_KAARTEN 52

typedef struct {
    int waarde;
    char* kleur;  // opgelet: pointer, want we willen juist genoeg reserveren
} KAART;

void vulBoek(KAART* boek) {
    char kleuren[4][10] = {"harten", "ruiten", "schoppen", "klaveren"};
    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            // TODO: gebruik malloc om juist genoeg ruimte te reserveren voor de kleur
            boek[index].kleur = malloc(strlen(kleuren[i]) + 1);
            // TODO: kopieer de kleur naar de gealloceerde ruimte
            strcpy(boek[index].kleur, kleuren[i]);
            // TODO: stel de waarde van de kaart in
            boek[index].waarde = j + 1;
            index++;
        }
    }
}

// OPGELET: er komt een adres van een KAART binnen:
void toonKaart(KAART* deKaart) {
    switch (deKaart->waarde) {
        // TODO: pas onderstaande code aan:
        case 1:
            printf("%s aas", deKaart->kleur);
            break;
        case 11:
            printf("%s boer", deKaart->kleur);
            break;
        case 12:
            printf("%s dame", deKaart->kleur);
            break;
        case 13:
            printf("%s heer", deKaart->kleur);
            break;
        default:  // alle andere kaarten:
            printf("%s %d", deKaart->kleur, deKaart->waarde);
    }
}

// OPGELET: de returnwaarde is een pointer!
KAART* trekKaart(KAART boek[]) {
    int willek = rand() % 52;
    // TODO: retourneer het adres van de willekeurige kaart
    return &boek[willek];
}

int main() {
    initUSART();
    // TODO: Gebruik calloc om de boek kaarten aan te maken
    KAART* boek = calloc(AANTAL_KAARTEN, sizeof(KAART));
    vulBoek(boek);

    // TODO: pas onderstaande code aan zodat ze compileert
    for (int i = 0; i < 10; i++) {
        KAART kaart = *trekKaart(boek);
        toonKaart(&kaart);
        printf("\n");
    }

    // TODO: geef alle dynamisch gereserveerde ruimte weer vrij:
    for (int i = 0; i < AANTAL_KAARTEN; i++) {
        free(boek[i].kleur);
    }
    free(boek);

    return 0;
}

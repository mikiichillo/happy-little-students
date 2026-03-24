#ifndef PROGETTO_FINALE_PERSONAGGI_H
#define PROGETTO_FINALE_PERSONAGGI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struttureGioco.h"
#include "ANSI-color-codes.h"


/// Pacchetto di funzioni Personaggio
personaggio *caricaPersonaggi(int *dimensione);

void stampaListaPersonaggi(personaggio *listaPersonaggi, int dimensione);
void stampaPersonaggio(personaggio personaggio);


#endif //PROGETTO_FINALE_PERSONAGGI_H

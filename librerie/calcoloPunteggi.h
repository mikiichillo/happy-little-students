#ifndef PROGETTO_FINALE_CALCOLOPUNTEGGI_H
#define PROGETTO_FINALE_CALCOLOPUNTEGGI_H

#include <stdio.h>
#include "personaggi.h"
#include "carteCfu.h"
#include "giocatori.h"

void trovaValoreMassimo(cartaCfu *arrayCarteGiocate, int numGiocatori, int *valoreMassimo);
bool vincitoriMultipliFunction(cartaCfu *arrayCarteGiocate, int numGiocatori, int valoreMassimo);

void trovaValoreMinimo(cartaCfu *arrayCarteGiocate, int numGiocatori, int *valoreMinimo);
bool perdentiMultipliFunction(cartaCfu *arrayCarteGiocate, int numGiocatori, int valoreMinimo);

bool turnoExtraPerdenti(int valoreMinimo, giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **pilaScarti, int *indicePerdente, FILE *fileLog, int turno);

#endif //PROGETTO_FINALE_CALCOLOPUNTEGGI_H

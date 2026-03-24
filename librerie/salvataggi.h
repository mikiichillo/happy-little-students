#ifndef PROGETTO_FINALE_SALVATAGGI_H
#define PROGETTO_FINALE_SALVATAGGI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struttureGioco.h"
#include "funzioniUtili.h"
#include "giocatori.h"
#include "carteCfu.h"
#include "carteOstacolo.h"
#include "personaggi.h"
#include "macro.h"

bool caricaSalvataggio(char nomeFile[CHAR_MAX], giocatore **elencoGiocatori, cartaCfu **mazzoCfu, cartaCfu **pilaDegliScarti, cartaOstacolo **mazzoOstacolo, int *turno, int *numGiocatori, FILE *fileLog);
void salvaSalvataggio(char nomeFile[CHAR_MAX], giocatore *elencoGiocatori, cartaCfu *mazzoCfu, cartaCfu *pilaDegliScarti, cartaOstacolo *mazzoOstacolo, int turno, int numGiocatori);

#endif //PROGETTO_FINALE_SALVATAGGI_H

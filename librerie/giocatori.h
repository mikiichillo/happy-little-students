#ifndef PROGETTO_FINALE_GIOCATORI_H
#define PROGETTO_FINALE_GIOCATORI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "personaggi.h"
#include "carteOstacolo.h"


/// Pacchetto di funzioni Giocatore
void creazioneElencoGiocatori(giocatore **listaGiocatori, int numGiocatori, int numPersonaggi, personaggio *listaPersonaggi);
void nuovoGiocatore(giocatore **elenco, giocatore *nuovoGiocatore);

void stampaElencoGiocatori(giocatore *elenco);
void stampaGiocatore(giocatore *giocatore);

void eliminaElencoGiocatori(giocatore *elenco);

#endif //PROGETTO_FINALE_GIOCATORI_H

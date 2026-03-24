#ifndef PROGETTO_FINALE_GIOCO_H
#define PROGETTO_FINALE_GIOCO_H

#include <stdio.h>
#include "personaggi.h"



int menuInizioTurno();

void azioneTurno(int *scelta, giocatore **elencoGiocatori, cartaCfu **mano, cartaCfu **pilaScarti, cartaCfu *arrayCarteGiocate, int indiceGiocatore, int numGiocatori, int turno, FILE *fileLog);

bool controlloCarteCfuPunto(cartaCfu **mano);

void stampaCarteGiocate(cartaCfu *arrayCarteGiocate, giocatore *elencoGiocatori, int numGiocatori);

void applicaBonusMalus(cartaCfu *arrayCarteGiocate, giocatore *elencoGiocatori, int numGiocatori, cartaOstacolo cartaOstacoloEstratta);

void effettiCarteCfuPunto(giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **mazzoCfu, cartaCfu **pilaScarti, int punteggioMaggiore, int punteggioMinore, bool *raddoppiaTutto);

bool giocaIstantanee(giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **pilaScarti, int valoreMassimo, int valoreMinimo, bool raddoppiaTutto, FILE *fileLog, int turno);

bool giocaSalva(giocatore **elencoGiocatori, int numGiocatori, cartaCfu **pilaScarti, int *indicePerdente, cartaOstacolo *cartaOstacoloTurno, cartaOstacolo **mazzoOstacolo, FILE *fileLog, int turno);

void rimescolaPilaScarti(cartaCfu **mazzo, cartaCfu **pilaScarti);

void eliminazioneGiocatori(giocatore **elencoGiocatori, int *numGiocatori, cartaCfu **pilaDegliScarti, cartaOstacolo **mazzoOstacolo, int indiceGiocatore, FILE *fileLog, int turno);

bool finePartita(int numGiocatori, giocatore *elencoGiocatori, FILE *fileLog, int turno);

#endif //PROGETTO_FINALE_GIOCO_H

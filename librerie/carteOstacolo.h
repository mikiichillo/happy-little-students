#ifndef PROGETTO_FINALE_CARTEOSTACOLO_H
#define PROGETTO_FINALE_CARTEOSTACOLO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struttureGioco.h"
#include "funzioniUtili.h"
#include "macro.h"
#include "ANSI-color-codes.h"

/// Pacchetto di funzioni carte Ostacolo
void caricamentoMazzoOstacolo(cartaOstacolo **mazzo);
void nuovaCartaOstacolo(cartaOstacolo **mazzo, cartaOstacolo *nuovaCarta);

void stampaMazzoOstacoloIntero(cartaOstacolo *mazzo);
void stampaCartaOstacolo(cartaOstacolo *carta);

void mescolaMazzoOstacolo(cartaOstacolo **mazzo);

void pescaCartaOstacolo(cartaOstacolo **mazzo, cartaOstacolo **mano);

int contaCarteMazzoOstacolo(cartaOstacolo **mazzo);

void eliminaMazzoOstacolo(cartaOstacolo *mazzo);

#endif //PROGETTO_FINALE_CARTEOSTACOLO_H

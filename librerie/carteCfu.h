#ifndef PROGETTO_FINALE_CARTECFU_H
#define PROGETTO_FINALE_CARTECFU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struttureGioco.h"
#include "funzioniUtili.h"
#include "macro.h"
#include "ANSI-color-codes.h"

/// Pacchetto di funzioni carte CFU
void caricamentoMazzoCfu(cartaCfu **mazzo);
void nuovaCartaCfu(cartaCfu **mazzo, cartaCfu *nuovaCarta);

void stampaManoCfuIntera(cartaCfu *mazzo);
void stampaCartaCfu(cartaCfu *carta);

void mescolaMazzoCfu(cartaCfu **mazzo);

void pescaCartaCfu(cartaCfu **mazzo, cartaCfu **mano);

int contaCarteMazzoCfu(cartaCfu **mazzo);

void eliminaMazzoCfu(cartaCfu *mazzo);

cartaCfu *giocaCarta(cartaCfu **mano, int indice);
#endif //PROGETTO_FINALE_CARTECFU_H

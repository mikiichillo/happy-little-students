#ifndef PROGETTO_FINALE_STRUTTUREGIOCO_H
#define PROGETTO_FINALE_STRUTTUREGIOCO_H

#include "macro.h"

typedef enum {NESSUNO, SCARTAP, RUBA, SCAMBIADS, SCARTAE, SCARTAC, SCAMBIAP, DOPPIOE, SBIRCIA, SCAMBIAC, ANNULLA, AUMENTA, DIMINUISCI, INVERTI, SALVA, DIROTTA} Effetto;

typedef enum {STUDIO, SOPRAVVIVENZA, SOCIALE, ESAME} Ostacolo;

typedef struct personaggio{
    char nome[DIM_NOME];
    int bonus[NUM_BONUS];
} personaggio;

typedef struct cartaCfu{
    char nome[DIM_NOME];
    int numeroCfu;
    Effetto effetto;
    struct cartaCfu* next;
} cartaCfu;

typedef struct cartaOstacolo{
    char nome[DIM_NOME];
    char descrizione[DIM_DESCRIZIONE];
    Ostacolo ostacolo;
    struct cartaOstacolo* next;
} cartaOstacolo;

typedef struct giocatore{
    char nome[DIM_NOME];
    personaggio personaggio;
    int cfuAccumulati;

    cartaCfu* mazzoCfu;
    cartaOstacolo* mazzoOstacolo;

    struct giocatore* next;
}giocatore;


#endif //PROGETTO_FINALE_STRUTTUREGIOCO_H

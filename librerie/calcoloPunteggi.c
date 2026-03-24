#include "calcoloPunteggi.h"

/** Trova il valore massimo tra le carte CFU giocate nel turno
 * @param arrayCarteGiocate array contenente i valori in CFU delle carte giocate (modificatori già applicati)
 * @param numGiocatori numero di giocatori in gioco
 * @param valoreMassimo valore in CFU più alto tra le carte giocate
 * */
void trovaValoreMassimo(cartaCfu *arrayCarteGiocate, int numGiocatori, int *valoreMassimo){
    *valoreMassimo = arrayCarteGiocate[0].numeroCfu;
    for (int indiceGiocatori = 0; indiceGiocatori < numGiocatori; ++indiceGiocatori) {
        if(arrayCarteGiocate[indiceGiocatori].numeroCfu > *valoreMassimo){
            *valoreMassimo = arrayCarteGiocate[indiceGiocatori].numeroCfu;
        }
    }
}

/** Calcola se il vincitore del turno è uno solo o ce ne sono stati di più
 * @param arrayCarteGiocate array contenente i valori in CFU delle carte giocate (modificatori già applicati)
 *
 * @param numGiocatori numero di giocatori in gioco
 * @param valoreMassimo valore in CFU più alto tra le carte giocate
 * @param indiceGiocatoreVincente indice del giocatore che ha giocato la carta più alta
 *
 * @return TRUE se ci sono più vincitori
 * @return FALSE se il vincitore è uno solo
 * */
bool vincitoriMultipliFunction(cartaCfu *arrayCarteGiocate, int numGiocatori, int valoreMassimo){
    bool vincitoriMultipli = false;
    int numVincitori = 0;
    for (int i = 0; i < numGiocatori; ++i) {
        if(arrayCarteGiocate[i].numeroCfu == valoreMassimo){
            numVincitori++;
        }
    }
    if(numVincitori>1){
        vincitoriMultipli = true;
    }
    return vincitoriMultipli;
}

/** Trova il valore minimo tra le carte CFU giocate nel turno
 * @param arrayCarteGiocate array contenente i valori in CFU delle carte giocate (modificatori già applicati)
 * @param numGiocatori numero di giocatori in gioco
 * @param valoreMinimo valore in CFU più basso tra le carte giocate
 * */
void trovaValoreMinimo(cartaCfu *arrayCarteGiocate, int numGiocatori, int *valoreMinimo){
    *valoreMinimo = arrayCarteGiocate[0].numeroCfu;
    for (int indiceGiocatori = 0; indiceGiocatori < numGiocatori; ++indiceGiocatori) {
        if(arrayCarteGiocate[indiceGiocatori].numeroCfu < *valoreMinimo){
            *valoreMinimo = arrayCarteGiocate[indiceGiocatori].numeroCfu;
        }
    }
}

/** Calcola se il perdente del turno è uno solo o ce ne sono stati di più
 *
 * @param arrayCarteGiocate array contenente i valori in CFU delle carte giocate (modificatori già applicati)
 * @param numGiocatori numero di giocatori in gioco
 * @param valoreMinimo valore in CFU più basso tra le carte giocate
 * @param indiceGiocatorePerdente indice del giocatore che ha giocato la carta più bassa
 *
 * @return TRUE se ci sono più perdenti
 * @return FALSE se il perdente è uno solo
 * */
bool perdentiMultipliFunction(cartaCfu *arrayCarteGiocate, int numGiocatori, int valoreMinimo){
    bool perdentiMultipli = false;
    int numPerdenti = 0;
    for (int i = 0; i < numGiocatori; ++i) {
        if(arrayCarteGiocate[i].numeroCfu == valoreMinimo){
            numPerdenti++;
        }
    }
    if(numPerdenti>1){
        perdentiMultipli = true;
    }
    return perdentiMultipli;
}

/** Funzione per gestire uno o più eventuali turni extra in caso ci siano più giocatori a parimerito per la sconfitta
 *
 * @param valoreMinimo valore in CFU più basso tra le carte giocate
 * @param elencoGiocatori lista contenente tutti i giocatori in gara
 * @param numGiocatori numero di giocatori in gioco
 * @param arrayCarteGiocate array contenente i valori in CFU delle carte giocate (modificatori già applicati)
 * @param pilaScarti lista che contiene le carte già giocate e ormai inserite nella pila degli scarti
 * @param indiceGiocatorePerdente indice del giocatore che ha giocato la carta più bassa
 *
 * @returns TRUE se c'è un solo perdente e quindi si può assegnare la carta ostacolo
 * @returns FALSE se ci sono più perdenti e quindi la carta ostacolo non viene data a nessuno
 * */
bool turnoExtraPerdenti(int valoreMinimo, giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **pilaScarti, int *indiceGiocatorePerdente, FILE *fileLog, int turno){

    bool altroTurno = false, carteFinite, risultato = true, cartaPunto;
    bool *perdenti = calloc(numGiocatori, sizeof (bool));

    int indiceTurnoExtra = 0;
    int indiceCartaDaGiocare;
    int *numeroCarteInMano = (int*) calloc(numGiocatori, sizeof(int));

    giocatore *lista = *elencoGiocatori;
    cartaCfu *cartaGiocata, *auxCarta;

    fileLog = fopen("log.txt", "a");

    /// Caricamento array perdenti, in cui a ogni indice viene fatto sapere quali giocatori sono da considerare tra i perdenti (true)
    for (int i = 0; i < numGiocatori; ++i) {
        perdenti[i] = false;
        if(arrayCarteGiocate[i].numeroCfu == valoreMinimo){
            perdenti[i] = true;
        }
    }

    do {

        /// Vengono contate le carte in mano a ciascun giocatore
        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; ++indiceGiocatore) {
            numeroCarteInMano[indiceGiocatore] = contaCarteMazzoCfu(&lista->mazzoCfu);
            lista = lista->next;
        }
        lista = *elencoGiocatori;


        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori && risultato; ++indiceGiocatore) {

            ///Inizializzo i booleani:
            carteFinite = false;
            altroTurno = true;

            auxCarta = lista->mazzoCfu;

            /// Scorro i giocatori. Se al giocatore corrente corrisponde TRUE nell'array dei perdenti, significa che deve giocare un turno extra.
            if (perdenti[indiceGiocatore]) {

                /** Controllo il numero di carte che il giocatore possiede.
                 * Se possiede almeno una carta può giocare, altrimenti il turno termina qui e il giocatore perde il turno.
                 * (si controlla poi successivamente se ci sono altri giocatori con 0 carte in mano) */
                if (numeroCarteInMano[indiceGiocatore] == 0) {
                    carteFinite = true;
                }

                stampaGiocatore(lista);
                stampaManoCfuIntera(lista->mazzoCfu);

                if(carteFinite){

                    printf("\n!!! Hai finito le carte CFU !!! \n");
                    altroTurno = false;         /// Un giocatore ha finito le carte: non si può quindi giocare un'altra mano

                    for (int i = 0; i < numGiocatori; ++i) {
                        if(i != indiceGiocatore && numeroCarteInMano[i] == 0){
                            risultato = false;          /// C'è però la possibilità che non sia l'unico giocatore perdente
                        }
                    }
                    if(risultato){
                        *indiceGiocatorePerdente = indiceGiocatore;      /// Teniamo traccia del primo giocatore trovato senza carte
                    }
                }
                else {
                    ///Svolgimento regolare della mano, senza tenere conto di bonus e/o malus

                    do {
                        cartaPunto = false;
                        printf(HCYN"\n*===== Scegli una carta da giocare =====*\n\n"RESET);
                        scanf("%d", &indiceCartaDaGiocare);

                        for (int i = 1; i < indiceCartaDaGiocare; ++i) {
                            auxCarta = auxCarta->next;
                        }
                        if(auxCarta->effetto > ANNULLA){
                            cartaPunto = true;
                        }
                        auxCarta = lista->mazzoCfu;

                        if(cartaPunto){
                            printf("!!! Non puoi giocare una carta CFU istantanea. Gioca una carta CFU punto. !!!\n\n");
                        }

                        if (indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > numeroCarteInMano[indiceGiocatore])
                            printf("!!! Inserire un indice per giocare una carta valida !!!\n\n");
                    } while (indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > numeroCarteInMano[indiceGiocatore] || cartaPunto);


                    cartaGiocata = giocaCarta(&lista->mazzoCfu, indiceCartaDaGiocare);
                    nuovaCartaCfu(pilaScarti, cartaGiocata);

                    fprintf(fileLog, "TURNO %d [Turno spareggio %d] -> %s gioca la carta CFU punto \"%s\".\n", turno, indiceTurnoExtra, (*elencoGiocatori)->nome, cartaGiocata->nome);

                    lista->mazzoCfu = (*elencoGiocatori)->mazzoCfu;

                    arrayCarteGiocate[indiceGiocatore] = *cartaGiocata;

                    printf("\nHai giocato la carta " HCYN "\"%s\"" RESET ".\n\n", cartaGiocata->nome);
                }
            }

            ///Si passa al giocatore successivo
            lista = lista->next;

            printf("\n*=============== Hai finito il tuo turno ===============*\n\n\n");
            printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);

            getchar();
            getchar();

        }

        trovaValoreMinimo(arrayCarteGiocate, numGiocatori, &valoreMinimo);

        if(!risultato) {
            /** Se tutti i giocatori hanno giocato una carta, si controlla se ci sono più giocatori con una carta "perdente"
             * Se è vero, si gioca un altro turno, ma solo tra coloro che possedevano la carta "perdente"
             * */
            if (altroTurno) {
                altroTurno = perdentiMultipliFunction(arrayCarteGiocate, numGiocatori, valoreMinimo);
            }

            /** Aggiornamento array perdenti
             * Viene segnato come perdente solo chi rispetta le seguenti condizioni:
             * 1. prima era già un perdente;
             * 2. il valore della carta giocata in questo turno coincide con il valore più basso giocato nel turno
             * */

            for (int i = 0; i < numGiocatori; ++i) {
                if (arrayCarteGiocate[i].numeroCfu != valoreMinimo && perdenti[i] == true) {
                    perdenti[i] = false;
                }
            }
            lista = *elencoGiocatori;       /// Torna all'inizio dell'elenco dei giocatori
        }

    }while(altroTurno);

    fclose(fileLog);

    free(perdenti);
    free(numeroCarteInMano);

    return risultato;
}
#include "gioco.h"
#include "salvataggi.h"

/** Scelta dell'azione da svolgere
 * @TASTO_1 permette al giocatore di giocare una carta dalla propria mano
 * @TASTO_2 permette al giocatore di visualizzare lo stato degli altri giocatori
 * @TASTO_0 permette di uscire dalla partita
 * */
int menuInizioTurno(){
    int scelta;

    printf("\n*----- Cosa vuoi fare? -----*\n"
           "[%d]: Gioca una carta CFU dalla tua mano.\n"
           "[%d]: Controlla lo stato degli altri giocatori.\n"
           "[%d]: Esci dalla partita.\n\n", TASTO_1, TASTO_2, TASTO_0);
    do{
        scanf("%d", &scelta);
        if(scelta!=TASTO_0 && scelta!=TASTO_1 && scelta!=TASTO_2)
            printf("\nInserisci un valore valido\n\n");
    } while (scelta!=TASTO_0 && scelta!=TASTO_1 && scelta!=TASTO_2);

    return scelta;
}

/** Funzione che gestisce le azioni da svolgere a inizio turno
 * @param scelta opzione scelta tra quelle proposte
 * @param mano mano del giocatore di turno
 * @param pilaScarti pila degli scarti, dove andrà a finire la carta giocata dal giocatore
 * @param arrayCarteGiocate la carta giocata viene inserita anche all'interno di un array, in modo da poterla successivamente confrontare con le altre carte giocate
 * @param indiceGiocatore indice del giocatore che sta svolgendo le azioni
 *
 * @TASTO_1 permette al giocatore di giocare una carta dalla propria mano
 * @TASTO_2 permette al giocatore di visualizzare lo stato degli altri giocatori
 * @TASTO_0 permette di uscire dalla partita
 * */
void azioneTurno(int *scelta, giocatore **elencoGiocatori, cartaCfu **mano, cartaCfu **pilaScarti, cartaCfu *arrayCarteGiocate, int indiceGiocatore, int numGiocatori, int turno, FILE *fileLog){
    int indiceCartaDaGiocare;

    bool cartaPunto = false;

    cartaCfu *cartaGiocata;
    cartaCfu *auxCarta = *mano;

    giocatore *aux = *elencoGiocatori;

    fileLog = fopen("log.txt", "a");


    switch (*scelta) {
        case TASTO_1:       /// Gioca una carta
            do {        /// Si chiede di giocare una carta fino a che non viene selezionato un indice valido che corrisponde a una carta Cfu punto
                cartaPunto = false;
                printf("Scegli una carta da giocare: \n\n");
                scanf("%d", &indiceCartaDaGiocare);

                for (int i = 1; i < indiceCartaDaGiocare; ++i) {
                    auxCarta = auxCarta->next;
                }
                if(auxCarta->effetto > ANNULLA){
                    cartaPunto = true;
                }
                auxCarta = *mano;
                if(cartaPunto){
                    printf("!!! Non puoi giocare una carta CFU istantanea. Gioca una carta CFU punto. !!!\n\n");
                }
                if(indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > MAX_CARTE_MANO)
                    printf("!!! Inserire un indice per giocare una carta valida !!!\n\n");
            }while(indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > MAX_CARTE_MANO || cartaPunto);

            cartaGiocata = giocaCarta(mano, indiceCartaDaGiocare);
            nuovaCartaCfu(pilaScarti, cartaGiocata);

            arrayCarteGiocate[indiceGiocatore] = *cartaGiocata; /// Viene utilizzato un array dinamico per contenere le carte giocate in ogni turno

            printf("\nHai giocato la carta " HCYN "\"%s\"" RESET ".\n\n", cartaGiocata->nome);

            fprintf(fileLog, "TURNO %d - %s ha giocato la carta CFU \"%s\"\n", turno, (*elencoGiocatori)->nome, cartaGiocata->nome);

            break;
        case TASTO_2:
            printf("\n\n ------ Informazioni altri giocatori ------\n\n");
            for (int i = 0; i < numGiocatori; ++i) {
                if(i != indiceGiocatore){
                    stampaGiocatore(aux);
                }
                aux = aux->next;
            }
            printf(BLINK "*=========== Premi Invio per tornare alla selezione carte. ===========*\n" RESET);
            getchar();
            getchar();
            break;
        case TASTO_0:
            printf("--> In uscita dal gioco...\n\n");
            printf(BLINK"*============= Premi Invio =============*\n\n"RESET);
            getchar();
            getchar();
    }
    fclose(fileLog);
}

/** La funzione controlla se il giocatore è in possesso di almeno una carta CFU punto
 * @param mano mano del giocatore
 * @return TRUE se il giocatore ha una carta che può giocare
 * @return FALSE se il giocatore non può giocare nessuna carta punto
 */
bool controlloCarteCfuPunto(cartaCfu **mano){
    int risultato = false;

    cartaCfu *aux = *mano;

    while (aux != NULL){
        if(aux->effetto <= ANNULLA){
            risultato = true;
        }
        aux = aux->next;
    }

    return risultato;
}

/** Stampa le carte Cfu punto giocate nel turno da tutti i giocatori (dopo che tutti hanno già giocato una carta)
 * @param arrayCarteGiocate carte giocate nel turno
 * @param elencoGiocatori lista di giocatori
 * @param numGiocatori numero di giocatori in gara
 */
void stampaCarteGiocate(cartaCfu *arrayCarteGiocate, giocatore *elencoGiocatori, int numGiocatori){
    for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; ++indiceGiocatore) {
        stampaGiocatore(elencoGiocatori);
        printf("\n*------- Carta Cfu Giocata -------*\n");
        stampaCartaCfu(&arrayCarteGiocate[indiceGiocatore]);
        printf("\n\n");
        elencoGiocatori = elencoGiocatori->next;
    }
    printf("\n");
}

/** Funzione che si occupa di applicare i bonus e i malus personaggio alle carte Cfu giocate a inizio turno, in base alla tipologia di carta Ostacolo estratta.
 * @param arrayCarteGiocate array che contiene le carte giocate, di cui stiamo andando a modificare i valori
 * @param elencoGiocatori elenco dei giocatori che ci serve per andare a prelevare le informazioni del personaggio di ciascun giocatore
 * @param numGiocatori numero di giocatori in gioco
 * @param cartaOstacoloEstratta carta ostacolo attiva nel turno in corso, ci dice quali bonus e/o malus vanno attivati
 * */
void applicaBonusMalus(cartaCfu *arrayCarteGiocate, giocatore *elencoGiocatori, int numGiocatori, cartaOstacolo cartaOstacoloEstratta){
    for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; ++indiceGiocatore) {
        arrayCarteGiocate[indiceGiocatore].numeroCfu += elencoGiocatori->personaggio.bonus[cartaOstacoloEstratta.ostacolo];
        elencoGiocatori = elencoGiocatori->next;
    }
}

/** Funzione che va ad attivare gli effetti delle carte CFU punto giocate dai giocatori. Gli effetti si attivano in ordine decrescente di punteggio delle carte.
 * @param elencoGiocatori lista dei giocatori in gioco
 * @param numGiocatori numero giocatori in gioco
 * @param arrayCarteGiocate array delle carte giocate nel turno
 * @param mazzoCfu mazzo delle carte CFU
 * @param pilaScarti pila degli scarti
 * @param punteggioMaggiore punteggio maggiore giocato nel turno (post-modificatore)
 * @param punteggioMinore punteggio minore giocato nel turno (post-modificatore)
 * @param raddoppiaTutto booleano che potrebbe influenzare il funzionamento delle carte Istantanee che verranno giocate successivamente
 */
void effettiCarteCfuPunto(giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **mazzoCfu, cartaCfu **pilaScarti, int punteggioMaggiore, int punteggioMinore, bool *raddoppiaTutto){

    giocatore *first = *elencoGiocatori;
    giocatore *aux;

    cartaCfu *auxCarta;
    cartaCfu *cartaGiocata;
    cartaCfu auxCartaScambio;
    cartaCfu arraySbircia[2];

    int indiceCartaDaGiocare, indiceFurto, n;
    int indiceCartaDaScambiare[2];

    bool check, annullaTutto = false;


    printf(BHCYN "\n*======= Attivazione effetti carte CFU punto =======*\n\n" RESET);

    for (int indiceValore = MAX_VALORE_CARTA; indiceValore>= MIN_VALORE_CARTA && !annullaTutto; indiceValore--) {   /// Gli effetti si attivano in ordine decrescente di punteggio della cart
        // Viene fatta scorre per ogni punteggio la lista di giocatori per sapere se qualcuno ha giocato una carta e attivarne l'effetto
        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori && !annullaTutto; ++indiceGiocatore) {
            if(arrayCarteGiocate[indiceGiocatore].numeroCfu == indiceValore){
                if(arrayCarteGiocate[indiceGiocatore].effetto != NESSUNO){
                    stampaGiocatore((*elencoGiocatori));
                    printf("\n*--- Carta Cfu Giocata ---*\n");
                    stampaCartaCfu(&arrayCarteGiocate[indiceGiocatore]);
                    printf("Effetto carta: \n");
                }
                switch (arrayCarteGiocate[indiceGiocatore].effetto) {
                    case NESSUNO:
                    {
                        printf("[NESSUNO]: Nessun effetto attivato.\n\n");
                    }
                        break;
                    case SCARTAP:
                    {
                        printf("[SCARTAP]: Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno.\n\n");

                        stampaManoCfuIntera((*elencoGiocatori)->mazzoCfu);

                        check = false;
                        auxCarta = (*elencoGiocatori)->mazzoCfu;

                        /// Se il giocatore ha in mano almeno una carta CFU punto senza effetto si attiva l'effetto
                        for (int i = 0; i < contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu); ++i) {
                            if(auxCarta->effetto == NESSUNO){
                                check = true;
                            }
                            auxCarta = auxCarta->next;
                        }

                        auxCarta = (*elencoGiocatori)->mazzoCfu;

                        if(check) {
                            do {
                                check = false;

                                printf("Scegli una carta da giocare: \n");
                                scanf("%d", &indiceCartaDaGiocare);
                                if (indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu)) {
                                    printf("!!! Inserire un indice per giocare una carta valida !!!\n\n");
                                    check = true;
                                } else {
                                    for (int i = 1; i < indiceCartaDaGiocare; ++i) {
                                        auxCarta = auxCarta->next;
                                    }
                                    /// Se la carta che si vuole scartare ha un effetto, viene richiesto di inserire un altro indice.
                                    if (auxCarta->effetto != NESSUNO) {
                                        printf("!!! Non puoi giocare una carta CFU con effetto o una carta CFU istantanea. Gioca una carta CFU punto. !!!\n\n");
                                        check = true;
                                    }
                                    auxCarta = (*elencoGiocatori)->mazzoCfu;
                                }
                            } while (check);

                            cartaGiocata = giocaCarta(&(*elencoGiocatori)->mazzoCfu, indiceCartaDaGiocare);
                            nuovaCartaCfu(pilaScarti, cartaGiocata);

                            /// Eventuale raddoppiamento del punteggio dato dall'effetto [DOPPIOE]
                            if (*raddoppiaTutto) {
                                printf(HYEL"*** Raddoppia effetto ***\n" RESET);
                                cartaGiocata->numeroCfu *= 2;
                            }

                            arrayCarteGiocate[indiceGiocatore].numeroCfu += cartaGiocata->numeroCfu;

                            printf("Hai scartato la carta " HCYN "\"%s\"." RESET " Hai aggiunto" HYEL " %d" RESET "punti al tuo punteggio. Hai ora" HYEL " %d " RESET "punti.\n\n", cartaGiocata->nome, cartaGiocata->numeroCfu, arrayCarteGiocate[indiceGiocatore].numeroCfu);
                        }
                        else{
                            printf("Non puoi scartare nessuna carta CFU punto senza effetto.\n");
                        }
                    }
                        break;
                    case RUBA:
                    {
                        printf("[RUBA]: Guarda la mano di un collega e ruba una carta a scelta.\n\n\n");
                        aux = first;
                        printf(HGRN "*========= Informazioni colleghi =========*\n\n" RESET);
                        for (int i = 0, k = 1; i < numGiocatori; ++i) {
                            if (i != indiceGiocatore) {         /// La lista scorre ma non stampa il giocatore che sta attivando l'effetto
                                printf("\n\n+--------");
                                printf("[%d]", k);
                                printf("--------+\n");
                                stampaGiocatore(aux);
                                k++;
                            }
                            aux = aux->next;
                        }
                        aux = first;
                        do {
                            printf("A quale collega vuoi rubare la carta?\n");
                            scanf("%d", &indiceFurto);
                            if (indiceFurto < 1 || indiceFurto > numGiocatori - 1) {
                                printf("Inserisci un indice valido.\n");
                            }
                        } while (indiceFurto < 1 || indiceFurto > numGiocatori - 1);        /// Un giocatore verrà escluso dal conteggio, ecco il perché di "numGiocatori-1"

                        for (int i = 0; i < indiceFurto; ++i) {
                            if (indiceFurto == indiceGiocatore) { /// In caso l'indice inserito corrisponda a quello del giocatore che ha attivato l'effetto della carta, viene fatto "saltare" il conteggio.
                                i--;
                            }
                            aux = aux->next;
                        }
                        printf("\n\n");

                        stampaManoCfuIntera(aux->mazzoCfu);     /// Il giocatore scelto mostra la sua mano

                        do {
                            printf("Che carta vuoi rubare?\n");
                            scanf("%d", &indiceFurto);
                            if (indiceFurto < MIN_CARTE_MANO || indiceFurto > contaCarteMazzoCfu(&aux->mazzoCfu)) {
                                printf("Inserisci un indice valido.\n");
                            }
                        } while (indiceFurto < MIN_CARTE_MANO || indiceFurto > contaCarteMazzoCfu(&aux->mazzoCfu));

                        cartaGiocata = giocaCarta(&aux->mazzoCfu, indiceFurto);
                        printf("\nHai rubato la carta " HCYN "\"%s\"" RESET ".\n\n", cartaGiocata->nome);
                        nuovaCartaCfu(&(*elencoGiocatori)->mazzoCfu, cartaGiocata);
                    }
                        break;
                    case SCAMBIADS:
                    {
                        printf("[SCAMBIADS]: Scambia questa carta con quella di un altro giocatore, purché senza effetto.\n");

                        check = false;

                        for (int i = 0; i < numGiocatori; ++i) {
                            if (arrayCarteGiocate[i].effetto == NESSUNO && i != indiceGiocatore) {
                                check = true;                          /// Se tra le carte giocate ne esiste almeno una il cui effetto sia [NESSUNO] (a eccezione di quella del giocatore) allora si può attivare l'effetto
                            }
                        }

                        if (check) {

                            printf("*========= Carte Giocate =========*\n\n");

                            aux = first;

                            for (int i = 0, k = 1; i < numGiocatori; ++i) {
                                if (i != indiceGiocatore) {     /// La lista scorre ma non stampa il giocatore che sta attivando l'effetto
                                    printf("\n\n+--------");
                                    printf("[%d]", k);
                                    printf("--------+\n");
                                    stampaGiocatore(aux);
                                    stampaCartaCfu(&arrayCarteGiocate[i]);
                                    k++;
                                }
                                aux = aux->next;
                            }

                            aux = first;

                            printf("\n-> La tua carta: \n");
                            stampaCartaCfu(&arrayCarteGiocate[indiceGiocatore]);

                            do {
                                check = false;
                                printf("Con quale carta vuoi scambiare la tua carta?\n");
                                scanf("%d", &indiceFurto);
                                if (indiceFurto < MIN_CARTE_MANO || indiceFurto > numGiocatori - 1) {
                                    printf("Inserisci un indice valido.\n");
                                    check = true;
                                }else{
                                    if (arrayCarteGiocate[indiceFurto].effetto != NESSUNO) {
                                        printf("Puoi scambiare questa carta solo con una carta senza effetto.\n");
                                        check = true;
                                    }
                                }

                            } while (check);

                            /** In caso l'indice inserito sia uguale o minore a quello del giocatore, viene fatto tornare indietro di uno per pareggiarlo con gli indici dell'array*/
                            if (indiceFurto <= indiceGiocatore) {
                                indiceFurto--;
                            }

                            /** Imposto il suo effetto a NESSUNO per evitare che il giocatore che riceva la carta possa riattivare l'effetto */
                            arrayCarteGiocate[indiceFurto].effetto = NESSUNO;

                            /** Scambio della carta */
                            auxCartaScambio = arrayCarteGiocate[indiceFurto];
                            arrayCarteGiocate[indiceFurto] = arrayCarteGiocate[indiceGiocatore];
                            arrayCarteGiocate[indiceGiocatore] = auxCartaScambio;

                        }
                    }
                        break;
                    case SCARTAE:
                    {
                        printf("[SCARTAE]: Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno.\n");

                        stampaManoCfuIntera((*elencoGiocatori)->mazzoCfu);

                        check = false;
                        auxCarta = (*elencoGiocatori)->mazzoCfu;

                        /// Se il giocatore possiede almeno una carta CFU punto con effetto l'effetto della carta si può attivare
                        for (int i = 0; i < contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu); ++i) {
                            if(auxCarta->effetto > NESSUNO && auxCarta->effetto <=ANNULLA){
                                check = true;
                            }
                            auxCarta = auxCarta->next;
                        }

                        auxCarta = (*elencoGiocatori)->mazzoCfu;

                        if(check) {
                            do {
                                check = false;
                                printf("\nChe carta vuoi scartare?\n");
                                scanf("%d", &indiceCartaDaGiocare);
                                if (indiceCartaDaGiocare < MIN_CARTE_MANO || indiceCartaDaGiocare > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu)) {
                                    printf("Inserisci un indice valido.");
                                    check = true;
                                } else {
                                    for (int i = 1; i < indiceCartaDaGiocare; ++i) {
                                        auxCarta = auxCarta->next;
                                    }
                                    if (auxCarta->effetto == NESSUNO || auxCarta->effetto > ANNULLA) {
                                        printf("Scegli una carta punto CFU con effetto.\n");
                                        check = true;
                                    }
                                    auxCarta = (*elencoGiocatori)->mazzoCfu;
                                }
                            } while (check);    /// Il ciclo si ripete finché non viene inserito un indice valido e finché non viene selezionata una carta CFU con effetto

                            cartaGiocata = giocaCarta(&(*elencoGiocatori)->mazzoCfu, indiceCartaDaGiocare);
                            nuovaCartaCfu(pilaScarti, cartaGiocata);

                            /// Eventuale raddoppiamento del punteggio dato dall'effetto [DOPPIOE]
                            if (*raddoppiaTutto) {
                                printf(HYEL"*** Raddoppia effetto ***\n" RESET);
                                cartaGiocata->numeroCfu *= 2;
                            }

                            /// Il punteggio viene sommato a quello della carta giocata a inizio turno
                            arrayCarteGiocate[indiceGiocatore].numeroCfu += cartaGiocata->numeroCfu;

                            printf("Hai scartato la carta " HCYN "\"%s\"." RESET " Hai aggiunto" HYEL " %d" RESET "punti al tuo punteggio. Hai ora" HYEL " %d " RESET "punti.\n\n", cartaGiocata->nome, cartaGiocata->numeroCfu, arrayCarteGiocate[indiceGiocatore].numeroCfu);
                        }
                        else{
                            printf("Non puoi scartare nessuna carta CFU punto con effetto.\n");
                        }
                    }
                        break;
                    case SCARTAC:
                    {
                        printf("[SCARTAC]: Scarta da uno a tre carte dalla tua mano.\n\n");

                        printf("Quante carte vuoi scartare?\n");
                        scanf("%d", &n);

                        auxCarta = (*elencoGiocatori)->mazzoCfu;
                        for (int i = 0; i < n; ++i) {
                            printf("\n");
                            stampaManoCfuIntera((*elencoGiocatori)->mazzoCfu);
                            do {
                                printf("\nQuale carta vuoi scartare?\n");
                                scanf("%d", &indiceCartaDaGiocare);
                                if(indiceCartaDaGiocare < 1 || indiceCartaDaGiocare > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu)){
                                    printf("Inserisci un indice valido.");
                                }
                            } while (indiceCartaDaGiocare < 1 || indiceCartaDaGiocare > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu));

                            cartaGiocata = giocaCarta(&(*elencoGiocatori)->mazzoCfu, indiceCartaDaGiocare);
                            printf("\nHai scartato la carta " HCYN "\"%s\"" RESET ".\n\n", cartaGiocata->nome);
                            nuovaCartaCfu(pilaScarti, cartaGiocata);
                        }
                    }
                        break;
                    case SCAMBIAP:
                    {
                        printf("[SCAMBIAP]: Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base.\n\n");

                        /// Inversione del punteggio massimo e minimo

                        for (int i = 0; i < numGiocatori; ++i) {
                            if (arrayCarteGiocate[i].numeroCfu == punteggioMaggiore) {
                                arrayCarteGiocate[i].numeroCfu = punteggioMinore;
                            }else{
                                if (arrayCarteGiocate[i].numeroCfu == punteggioMinore) {
                                    arrayCarteGiocate[i].numeroCfu = punteggioMaggiore;
                                }
                            }
                        }
                    }
                        break;
                    case DOPPIOE:
                    {
                        printf("[DOPPIOE]: Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti).\n\n");
                        *raddoppiaTutto = true;
                    }
                        break;
                    case SBIRCIA:
                    {
                        printf("[SBIRCIA]: Guarda due carte in cima al mazzo, prendine una e scarta l’altra.\n");

                        rimescolaPilaScarti(mazzoCfu, pilaScarti);

                        printf("+----- Carte Sbirciate -----+\n\n");

                        for (int i = 0; i < 2; ++i) {
                            pescaCartaCfu(mazzoCfu, &auxCarta);
                            arraySbircia[i] = *auxCarta;
                            printf("[%d] -> ", i+1);
                            stampaCartaCfu(&arraySbircia[i]);
                            auxCarta = auxCarta->next;
                        }

                        do {
                            printf("Quale carta vuoi pescare?\n");
                            scanf("%d", &indiceCartaDaGiocare);
                            if(indiceCartaDaGiocare<1 || indiceCartaDaGiocare>2)
                                printf("Inserisci un indice valido.\n");
                        }while(indiceCartaDaGiocare<1 || indiceCartaDaGiocare>2);

                        switch (indiceCartaDaGiocare) {
                            case 1:
                                nuovaCartaCfu(&(*elencoGiocatori)->mazzoCfu, &arraySbircia[0]);
                                nuovaCartaCfu(pilaScarti, &arraySbircia[1]);
                                break;
                            case 2:
                                nuovaCartaCfu(&(*elencoGiocatori)->mazzoCfu, &arraySbircia[1]);
                                nuovaCartaCfu(pilaScarti, &arraySbircia[0]);
                                break;
                            default:
                                break;
                        }

                    }
                        break;
                    case SCAMBIAC:
                    {
                        printf("[SCAMBIAC]: Scambia la carta punto giocata nel turno da un giocatore G1 con quella di un giocatore G2, con G1 e G2\n"
                               "scelti dal giocatore che ha giocato la carta SCAMBIAC.\n\n");

                        printf(HGRN "*========= Informazioni colleghi =========*\n\n" RESET);

                        aux = first;

                        for (int i = 0; i < numGiocatori; ++i) {
                            printf("\n\n+--------");
                            printf("[%d]", i+1);
                            printf("--------+\n");
                            stampaGiocatore(aux);
                            stampaCartaCfu(&arrayCarteGiocate[i]);
                            aux = aux->next;
                        }

                        aux = first;

                        do{
                            for (int i = 0; i < 2; ++i) {
                                do{
                                    printf("Quali carte vuoi scambiare?\n"
                                           "Carta %d: ", i+1);
                                    scanf("%d", &indiceCartaDaScambiare[i]);
                                }while(indiceCartaDaScambiare[i]<1 || indiceCartaDaScambiare[i]>numGiocatori);

                                indiceCartaDaScambiare[i]--;    /// Viene ridotto l'indice inserito dal giocatore per renderlo compatibile con l'indice dell'array di Carte Giocate

                                /** Nel caso una delle due carte che il giocatore vuole scambiare siano la carta giocata dal giocatore stesso, imposto il suo effetto a NESSUNO
                                 * per evitare che il giocatore che riceva la carta possa riattivare l'effetto */
                                if(indiceCartaDaScambiare[i] == indiceGiocatore){
                                    arrayCarteGiocate[indiceGiocatore].effetto = NESSUNO;
                                }
                            }
                            if(indiceCartaDaScambiare[0]==indiceCartaDaScambiare[1]){
                                printf("Inserisci due indici diversi.\n");
                            }
                        }while(indiceCartaDaScambiare[0]==indiceCartaDaScambiare[1]);

                        /** Scambio delle carte */
                        auxCartaScambio = arrayCarteGiocate[indiceCartaDaScambiare[0]];
                        arrayCarteGiocate[indiceCartaDaScambiare[0]] = arrayCarteGiocate[indiceCartaDaScambiare[1]];
                        arrayCarteGiocate[indiceCartaDaScambiare[1]] = auxCartaScambio;

                    }
                        break;
                    case ANNULLA:
                    {
                        printf("[ANNULLA]: Annulla gli effetti di tutte le carte punto durante il turno.\n\n");
                        annullaTutto = true;
                    }
                    default:
                        break;
                }
            }
            *elencoGiocatori = (*elencoGiocatori)->next;
        }
        *elencoGiocatori = first;
    }
    printf("\n\n");
}

/** Funzione che permette ai giocatori di giocare una carta istantanea e che attiva i suoi effetti.
 *
 * @param elencoGiocatori lista dei giocatori in gioco
 * @param numGiocatori numero giocatori in gioco
 * @param arrayCarteGiocate array delle carte giocate nel turno
 * @param pilaScarti pila degli scarti
 * @param mazzoOstacoli mazzo delle carte ostacolo
 * @param raddoppiaTutto booleano che potrebbe influenzare il funzionamento delle carte Istantanee che verranno giocate successivamente
 *
 * @return TRUE se tra i giocatori c'è almeno un giocatore che (prima di giocare la carta istantanea) ha una carta istantanea. In caso la funzione restituisca true, viene rieseguito il controllo all'iterazione successiva
 * @return FALSE se nessun giocatore è in possesso di una carta istantanea
 */
bool giocaIstantanee(giocatore **elencoGiocatori, int numGiocatori, cartaCfu *arrayCarteGiocate, cartaCfu **pilaScarti, int valoreMassimo, int valoreMinimo, bool raddoppiaTutto, FILE *fileLog, int turno){

    giocatore *first = *elencoGiocatori;
    giocatore *aux = *elencoGiocatori;

    cartaCfu *cartaGiocata = NULL;
    cartaCfu *auxCarta;

    int indiceGiocatore, indiceCarta, selezione;
    bool istantanee = false;

    fileLog = fopen("log.txt", "a");


    /** Viene controllata la mano di tutti i giocatori per sapere se almeno uno di loro può giocare una carta istantanea */
    while((*elencoGiocatori)!=NULL){
        auxCarta = (*elencoGiocatori)->mazzoCfu;
        while ((*elencoGiocatori)->mazzoCfu!=NULL){
            if((*elencoGiocatori)->mazzoCfu->effetto>ANNULLA && (*elencoGiocatori)->mazzoCfu->effetto<=INVERTI){
                istantanee = true;
            }
            (*elencoGiocatori)->mazzoCfu = (*elencoGiocatori)->mazzoCfu->next;
        }
        (*elencoGiocatori)->mazzoCfu = auxCarta;
        (*elencoGiocatori) = (*elencoGiocatori)->next;
    }

    *elencoGiocatori = first;

    if(istantanee){
        printf(HCYN "*======= Attivazione effetti carte istantanee =======*\n\n" RESET);
        printf("[%d]: Gioca carta istantanea.\n"
               "[%d]: Non giocare nessuna carta istantanea e passa al calcolo del punteggio.\n\n", TASTO_1, TASTO_0);

        scanf("%d", &selezione);

        if(selezione == TASTO_1) {

            /// Vengono mostrati tutti i giocatori e viene chiesto chi vuole giocare la carta istantanea
            for (int i = 0; i < numGiocatori; ++i) {
                printf("+--------");
                printf("[%d]", i+1);
                printf("--------+\n");
                stampaGiocatore((*elencoGiocatori));
                printf("\n*--- Carta Cfu Giocata ---*\n");
                stampaCartaCfu(&arrayCarteGiocate[i]);
                printf("\n\n");
                (*elencoGiocatori) = (*elencoGiocatori)->next;
            }
            (*elencoGiocatori) = aux;

            do {
                *elencoGiocatori = aux;
                istantanee = false;
                printf("Quale giocatore vuole giocare una carta istantanea?\n");
                scanf("%d", &indiceGiocatore);
                if (indiceGiocatore < 1 || indiceGiocatore > numGiocatori) {
                    printf("Inserisci un indice valido.\n");
                } else {
                    for (int i = 1; i < indiceGiocatore; ++i) {
                        (*elencoGiocatori) = (*elencoGiocatori)->next;
                    }
                    auxCarta = (*elencoGiocatori)->mazzoCfu;

                    /// Viene controllato se il giocatore scelto possiede una carta cfu istantanea
                    while ((*elencoGiocatori)->mazzoCfu != NULL) {
                        if ((*elencoGiocatori)->mazzoCfu->effetto > ANNULLA && (*elencoGiocatori)->mazzoCfu->effetto <= INVERTI) {
                            istantanee = true;
                        }
                        (*elencoGiocatori)->mazzoCfu = (*elencoGiocatori)->mazzoCfu->next;
                    }
                    (*elencoGiocatori)->mazzoCfu = auxCarta;
                    if (!istantanee) {
                        printf("Il giocatore scelto non ha carte istantanee in mano.\n");
                    }
                    *elencoGiocatori = aux;
                }
            } while (indiceGiocatore < 1 || indiceGiocatore > numGiocatori || !istantanee);

            for (int i = 1; i < indiceGiocatore; ++i) {
                *elencoGiocatori = (*elencoGiocatori)->next;
            }

            clearConsole();

            stampaGiocatore((*elencoGiocatori));
            printf("\n");
            stampaManoCfuIntera((*elencoGiocatori)->mazzoCfu);

            /// Selezione carta istantanea da giocare
            do {
                istantanee = false;
                auxCarta = (*elencoGiocatori)->mazzoCfu;

                printf("\nChe carta istantanea vuoi attivare?\n");
                scanf("%d", &indiceCarta);
                if (indiceCarta < 1 || indiceCarta > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu)) {
                    printf("Inserisci un indice valido.\n");
                }
                else{
                    for (int i = 1; i < indiceCarta; ++i) {
                        (*elencoGiocatori)->mazzoCfu = (*elencoGiocatori)->mazzoCfu->next;
                    }
                    /// Viene controllato il tipo della carta selezionata per determinare se può essere giocata o no
                    if ((*elencoGiocatori)->mazzoCfu->effetto > ANNULLA && (*elencoGiocatori)->mazzoCfu->effetto <= INVERTI) {
                        istantanee = true;
                    }
                    (*elencoGiocatori)->mazzoCfu = auxCarta;
                    if (!istantanee) {
                        printf("Devi giocare una carta istantanea.\n");
                    }
                }
            } while (indiceCarta < 1 || indiceCarta > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu) || !istantanee);

            cartaGiocata = giocaCarta(&(*elencoGiocatori)->mazzoCfu, indiceCarta);
            nuovaCartaCfu(pilaScarti, cartaGiocata);

            fprintf(fileLog, "TURNO %d - %s ha giocato la carta istantanea \"%s\".\n", turno, (*elencoGiocatori)->nome, cartaGiocata->nome);

            *elencoGiocatori = first;

            switch (cartaGiocata->effetto) {
                case AUMENTA:
                    printf("[ANNULLA]: Aumenta di %d CFU il punteggio del turno di un giocatore a scelta.\n\n", INCREMENTO);
                    for (int i = 0; i < numGiocatori; ++i) {
                        printf("\n+--------");
                        printf("[%d]", i + 1);
                        printf("--------+\n");
                        stampaGiocatore((*elencoGiocatori));
                        printf("\n*--- Carta Cfu Giocata ---*\n");
                        stampaCartaCfu(&arrayCarteGiocate[i]);
                        printf("\n\n");
                        (*elencoGiocatori) = (*elencoGiocatori)->next;
                    }
                    (*elencoGiocatori) = first;
                    do {
                        printf("A quale giocatore vuoi aumentare il punteggio?\n\n");
                        scanf("%d", &indiceGiocatore);
                        if (indiceGiocatore < 1 || indiceGiocatore > numGiocatori) {
                            printf("Inserisci un indice valido.\n");
                        }
                    } while (indiceGiocatore < 1 || indiceGiocatore > numGiocatori);

                    indiceGiocatore--; // Il gioco richiede di inserire gli indici a partire da 1, quindi per pareggiare con gli indici dell'array diminuisco di 1

                    arrayCarteGiocate[indiceGiocatore].numeroCfu += INCREMENTO;
                    if (raddoppiaTutto) { // Se l'effetto DOPPIOE è stato attivato, eseguo nuovamente l'operazione
                        arrayCarteGiocate[indiceGiocatore].numeroCfu += INCREMENTO;
                    }

                    break;
                case DIMINUISCI:
                    printf("[DIMINUISCI]: Diminuisci di %d CFU il punteggio del turno di un giocatore a scelta.\n\n", DECREMENTO);
                    for (int i = 0; i < numGiocatori; ++i) {
                        printf("\n+--------");
                        printf("[%d]", i + 1);
                        printf("--------+\n");
                        stampaGiocatore((*elencoGiocatori));
                        printf("\n*--- Carta Cfu Giocata ---*\n");
                        stampaCartaCfu(&arrayCarteGiocate[i]);
                        printf("\n\n");
                        (*elencoGiocatori) = (*elencoGiocatori)->next;
                    }
                    (*elencoGiocatori) = first;
                    do {
                        printf("A quale giocatore vuoi diminuire il punteggio?\n\n");
                        scanf("%d", &indiceGiocatore);
                        if (indiceGiocatore < 1 || indiceGiocatore > numGiocatori) {
                            printf("Inserisci un indice valido.\n");
                        }
                    } while (indiceGiocatore < 1 || indiceGiocatore > numGiocatori);

                    indiceGiocatore--;      // Il gioco richiede di inserire gli indici a partire da 1, quindi per pareggiare con gli indici dell'array diminuisco di 1

                    arrayCarteGiocate[indiceGiocatore].numeroCfu -= DECREMENTO;
                    if (raddoppiaTutto) { // Se l'effetto DOPPIOE è stato attivato, eseguo nuovamente l'operazione
                        arrayCarteGiocate[indiceGiocatore].numeroCfu -= DECREMENTO;
                    }

                    break;
                case INVERTI:
                    printf("[INVERTI]: Inverti punteggio minimo e massimo del turno.\n");
                    for (int i = 0; i < numGiocatori; ++i) {
                        if (arrayCarteGiocate[i].numeroCfu == valoreMassimo) {
                            arrayCarteGiocate[i].numeroCfu = valoreMinimo;
                        } else if (arrayCarteGiocate[i].numeroCfu == valoreMinimo) {
                            arrayCarteGiocate[i].numeroCfu = valoreMassimo;
                        }
                    }
                    break;
                default:
                    break;
            }

        }
        else{
            istantanee = false;
        }
    }
    else{
        printf("\n*----- Nessun giocatore può giocare carte istantanee -----*\n\n");
    }

    printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);
    getchar();
    getchar();
    clearConsole();

    *elencoGiocatori = first;
    fclose(fileLog);
    return istantanee;
}

/** La funzione permette al giocatore designato come perdente del turno di giocare una carta SALVA o DIROTTA, nel caso egli la possegga
 *
 * @param elencoGiocatori lista dei giocatori in gioco
 * @param numGiocatori numero giocatori in gioco
 * @param arrayCarteGiocate array delle carte giocate nel turno
 * @param pilaScarti pila degli scarti
 * @param valoreMassimo punteggio massimo giocato nel turno
 * @param valoreMinimo punteggio minimo giocato nel turno
 * @param indicePerdente indice del giocatore designato come perdente del turno
 *
 * @return TRUE se il giocatore ha giocato una carta che gli ha permesso di salvarsi, FALSE se il giocatore deve pescare la carta ostacolo
 */
bool giocaSalva(giocatore **elencoGiocatori, int numGiocatori, cartaCfu **pilaScarti, int *indicePerdente, cartaOstacolo *cartaOstacoloTurno, cartaOstacolo **mazzoOstacolo, FILE *fileLog, int turno){

    int selezione, indiceCarta;
    giocatore *first = *elencoGiocatori;

    cartaCfu *cartaGiocata = NULL;
    cartaCfu *auxCarta;

    bool salvaTurno = false;

    fileLog = fopen("log.txt", "a");

    for (int i = 0; i < *indicePerdente; ++i) {
        *elencoGiocatori = (*elencoGiocatori)->next;
    }

    auxCarta = (*elencoGiocatori)->mazzoCfu;

    /** Viene controllato se il giocatore possiede almeno una carta giocabile */
    while ((*elencoGiocatori)->mazzoCfu!=NULL){
        if((*elencoGiocatori)->mazzoCfu->effetto==SALVA || (*elencoGiocatori)->mazzoCfu->effetto==DIROTTA){
            salvaTurno = true;
        }
        (*elencoGiocatori)->mazzoCfu = (*elencoGiocatori)->mazzoCfu->next;
    }

    (*elencoGiocatori)->mazzoCfu = auxCarta;

    printf(HCYN"\n\n*======= Attivazione effetti Salva o Dirotta =======*\n"RESET);

    if(salvaTurno){

        salvaTurno = false;

        stampaGiocatore((*elencoGiocatori));

        stampaManoCfuIntera((*elencoGiocatori)->mazzoCfu);

        do{
            /** Viene chiesto al giocatore se vuole giocare o meno una carta */
            printf("\n[%d]: Gioca una carta Salva o Dirotta.\n"
                   "[%d]: Non giocare nessuna carta e prendi la carta ostacolo.\n", TASTO_1, TASTO_0);

            scanf("%d", &selezione);

            if(selezione!=TASTO_0 && selezione!=TASTO_1){
                printf("Inserisci un indice valido.\n");
            }
        }while(selezione!=TASTO_0 && selezione!=TASTO_1);


        if(selezione == TASTO_1) {

            do {
                salvaTurno = false;
                auxCarta = (*elencoGiocatori)->mazzoCfu;

                printf("Che carta Salva o Dirotta vuoi attivare?\n");
                scanf("%d", &indiceCarta);
                if (indiceCarta < 1 || indiceCarta > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu)) {
                    printf("Inserisci un indice valido.\n");
                }
                else{
                    for (int i = 1; i < indiceCarta; ++i) {
                        (*elencoGiocatori)->mazzoCfu = (*elencoGiocatori)->mazzoCfu->next;
                    }
                    if ((*elencoGiocatori)->mazzoCfu->effetto == SALVA || (*elencoGiocatori)->mazzoCfu->effetto == DIROTTA) {
                        salvaTurno = true;
                    }
                    (*elencoGiocatori)->mazzoCfu = auxCarta;
                    if (!salvaTurno) {
                        printf("Devi giocare una carta Salva o Dirotta.\n");
                    }
                }

            } while (indiceCarta < 1 || indiceCarta > contaCarteMazzoCfu(&(*elencoGiocatori)->mazzoCfu) || !salvaTurno);

            salvaTurno = false;

            (*elencoGiocatori)->mazzoCfu = auxCarta;

            cartaGiocata = giocaCarta(&(*elencoGiocatori)->mazzoCfu, indiceCarta);
            nuovaCartaCfu(pilaScarti, cartaGiocata);

            fprintf(fileLog, "TURNO %d - %s ha giocato la carta istantanea \"%s\".\n", turno, (*elencoGiocatori)->nome, cartaGiocata->nome);

            switch (cartaGiocata->effetto) {
                case SALVA:
                    printf("[SALVA]: Metti la carta Ostacolo che stai per prendere in fondo al mazzo.\n");
                    pescaCartaOstacolo(&cartaOstacoloTurno, mazzoOstacolo);
                    salvaTurno = true;
                    break;
                case DIROTTA:
                    printf("[DIROTTA]: Dai la carta che stai per prendere ad un altro giocatore a tua scelta.\n\n");

                    *elencoGiocatori = first;
                    for (int i = 0, k = 1; i < numGiocatori; ++i) {
                        if(i != *indicePerdente){
                            printf("\n+--------");
                            printf("[%d]", k);
                            printf("--------+\n");
                            stampaGiocatore(*elencoGiocatori);
                            k++;
                        }
                        *elencoGiocatori = (*elencoGiocatori)->next;
                    }
                    *elencoGiocatori = first;

                    do{
                        printf("Su quale giocatore vuoi dirottare la carta ostacolo? \n");
                        scanf("%d", &selezione);

                    }while(selezione<1 || selezione > numGiocatori-1);

                    if(selezione <= *indicePerdente){ // Potendo inserire un indice che parte da 1, bisogna risistemare l'indicizzazione per colmare il vuoto generato dalla presenza del giocatore che gioca la carta
                        selezione--;
                    }

                    *indicePerdente = selezione;

                    for (int i = 0; i < selezione; ++i) {
                        *elencoGiocatori = (*elencoGiocatori)->next;
                    }
                    printf("Hai dirottato la carta ostacolo su " HRED "%s" RESET"!!!\n\n", (*elencoGiocatori)->nome);
                    *elencoGiocatori = first;
                    break;
                default:
                    break;
            }

        }
        else{
            printf("\n*=== Hai deciso di non giocare nessuna carta " HCYN "SALVA" RESET " o " HCYN "DIROTTA" RESET " ===*\n");
        }

    }
    else{
        printf("\n+---> Il giocatore perdente non puo' giocare nessuna carta " HCYN "SALVA" RESET " o " HCYN "DIROTTA" RESET ".\n\n\n");
    }

    fclose(fileLog);

    return salvaTurno;

}


/** Funzione che, in caso il mazzo sia finito, rimescola la pila degli scarti nel mazzo
 * @param mazzo mazzo CFU di pesca
 * @param pilaScarti pila delle carte CFU giocate
 */
void rimescolaPilaScarti(cartaCfu **mazzo, cartaCfu **pilaScarti){
    if(mazzo == NULL){
        while (pilaScarti != NULL){
            pescaCartaCfu(pilaScarti, mazzo);
        }
        mescolaMazzoCfu(mazzo);
    }
}

/** Funzione che controlla se un giocatore ha ottenuto un certo numero di carte ostacolo e deve essere eliminato
 * Condizioni eliminazione: 1 carta ostacolo di ciascuno dei tre tipi (ESAME escluso); 3 carte Ostacolo dello stesso tipo; le carte ESAME valgono come qualunque delle altre tipologie di carte contemporaneamente
 * @param elencoGiocatori lista di giocatori in gara
 * @param numGiocatori numero di giocatori, che potrebbe essere modificato in caso di eliminazione di un giocatore
 */
void eliminazioneGiocatori(giocatore **elencoGiocatori, int *numGiocatori, cartaCfu **pilaDegliScarti, cartaOstacolo **mazzoOstacolo, int indiceGiocatore, FILE *fileLog, int turno){

    int contaOstacolo[ESAME+1];

    giocatore *first = *elencoGiocatori;
    giocatore *attuale = *elencoGiocatori;
    giocatore *precedente = NULL;

    giocatore *giocatoreDaEliminare;

    bool daEliminare;

    fileLog = fopen("log.txt", "a");

    /// Inizializzo il contatore di carte ostacolo
    for (int i = 0; i <= ESAME; ++i) {
        contaOstacolo[i] = 0;
    }
    daEliminare = false;

    /// Scorro fino al giocatore desiderato

    for (int i = 0; i < indiceGiocatore && attuale->next!=NULL; ++i) {
        precedente = attuale; /// Conservo la posizione del giocatore precedente in caso di eliminazione di un giocatore intermedio.
        attuale = attuale->next;
    }

    /// Conto le carte ostacolo di ogni tipo e il totale di carte ostacolo
    cartaOstacolo *firstOstacolo = attuale->mazzoOstacolo;
    while (attuale->mazzoOstacolo != NULL) {
        contaOstacolo[attuale->mazzoOstacolo->ostacolo]++;
        attuale->mazzoOstacolo = attuale->mazzoOstacolo->next;
    }
    attuale->mazzoOstacolo = firstOstacolo;


    /// Controlli per determinare se il giocatore ha perso

    if (contaOstacolo[STUDIO] >= 1 && contaOstacolo[SOPRAVVIVENZA] >= 1 && contaOstacolo[SOCIALE] >= 1) {       /// Il giocatore ha almeno una carta di ciascun tipo
        daEliminare = true;
    } else {
        for (int i = 0; i < ESAME; ++i) {       /// Il giocatore ha tre carte di un tipo
            if (contaOstacolo[i] == 3)
                daEliminare = true;
        }
    }

    /// Controllo delle carte ESAME (jolly)

    if (!daEliminare) {
        switch (contaOstacolo[ESAME]) {

            /// Una carta ESAME:
            case 1:
                for (int i = 0; i < ESAME; ++i) {
                    if (contaOstacolo[i] == 2) {        /// Se il giocatore è in possesso di due carte Ostacolo dello stesso tipo, ha perso.
                        daEliminare = true;
                    } else {
                        switch (i) {        /// Se il giocatore è in possesso di una carta Ostacolo di due tipi diversi, ha perso.
                            case STUDIO:
                                if (contaOstacolo[SOCIALE] == 1 && contaOstacolo[SOPRAVVIVENZA] == 1)
                                    daEliminare = true;
                                break;
                            case SOPRAVVIVENZA:
                                if (contaOstacolo[SOCIALE] == 1 && contaOstacolo[STUDIO] == 1)
                                    daEliminare = true;
                                break;
                            case SOCIALE:
                                if (contaOstacolo[STUDIO] == 1 && contaOstacolo[SOPRAVVIVENZA] == 1) {
                                    daEliminare = true;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;

            /// Due carte ESAME: se il giocatore è in possesso di una qualunque altra carta ostacolo, ha perso.
            case 2:
                for (int i = 0; i < ESAME; ++i) {
                    if (contaOstacolo[i] == 1) {
                        daEliminare = true;
                    }
                }
                break;

            /// Tre carte ESAME: il giocatore ha perso.
            case 3:
                daEliminare = true;
                break;
            default:
                break;
        }
    }

    /// Eliminazione del giocatore
    if(daEliminare){
        {
            /// Messaggio di avviso
            printf(RED "\n\n*========================================================*\n\n"
                   "Il giocatore %s e' stato eliminato dalla partita.\n\n"
                   "*========================================================*\n\n" RESET, attuale->nome);

            /// Inserimento delle carte Ostacolo del giocatore perdente nel mazzo delle carte Ostacolo
            while (attuale->mazzoOstacolo != NULL) {
                pescaCartaOstacolo(&attuale->mazzoOstacolo, mazzoOstacolo);
            }

            /// Scartamento delle carte Cfu del giocatore perdente
            while (attuale->mazzoCfu != NULL) {
                pescaCartaCfu(&attuale->mazzoCfu, pilaDegliScarti);
            }
        }

        fprintf(fileLog, "TURNO %d - %s è stato eliminato dal gioco.\n", turno, attuale->nome);

        /// Liberazione giocatore dalla lista e ricollegamento degli altri giocatori
        if(attuale == first){       // Eliminazione primo giocatore
            giocatoreDaEliminare = (*elencoGiocatori);
            (*elencoGiocatori) = (*elencoGiocatori)->next;
            free(giocatoreDaEliminare);
        }
        else {
            precedente->next = attuale->next;
            free(attuale);
        }

        /// Riduzione numero giocatori per il corretto proseguimento del gioco
        (*numGiocatori)--;
    }
    fclose(fileLog);
}


/** Controllo punteggi e numero giocatori per determinare se la partita è finita (e l'eventuale vincitore) o se si può ancora giocare
 *
 * @param numGiocatori per controllare se ci sono ancora giocatori in gara o se sono stati tutti eliminati tranne uno (il vincitore)
 * @param elencoGiocatori per controllare i punteggi di ognuno, e decidere se almeno un giocatore ha abbastanza punti per poter essere dichiarato vincitore
 *
 * @return TRUE se la partita può dirsi conclusa
 * @return FALSE se il gioco può ancora continuare
 */
bool finePartita(int numGiocatori, giocatore *elencoGiocatori, FILE *fileLog, int turno){

    bool fine = false;
    bool piuVincitori = false;
    int punteggioMaggiore, indiceVincitore;
    giocatore *lista = elencoGiocatori;

    fileLog = fopen("log.txt", "a");

    /// Se il giocatore è rimasto solo, vince la partita
    if(numGiocatori == 1){
        fine = true;
        printf("\n---> Tutti gli avversari sono stati eliminati <---\n\n");

    }
    else {

        punteggioMaggiore = 0;

        /// Si controlla chi è il giocatore con il punteggio maggiore
        for (int i = 0; i < numGiocatori; ++i) {
            if (lista->cfuAccumulati >= punteggioMaggiore) {
                punteggioMaggiore = lista->cfuAccumulati;
                indiceVincitore = i;
            }
            lista = lista->next;
        }
        lista = elencoGiocatori;

        /// In caso il giocatore con il punteggio maggiore abbia superato la soglia di punti necessaria a vincere, si controlla se altri giocatori rispettano la stessa condizione
        if(punteggioMaggiore >= PUNTEGGIO_VITTORIA){
            for (int i = 0; i < numGiocatori; ++i) {
                if(lista->cfuAccumulati >= PUNTEGGIO_VITTORIA && i != indiceVincitore){
                    piuVincitori = true;
                }
                lista = lista->next;
            }
            fine = true;
        }

        lista = elencoGiocatori;

    }

    if(fine){
        if(piuVincitori){

            printf(HYEL"\n\n *========= Abbiamo piu' vincitori!!! =========*\n\n"RESET);

            for (int i = 0; i < numGiocatori; ++i) {
                if(lista->cfuAccumulati >= PUNTEGGIO_VITTORIA){
                    fprintf(fileLog, "\nTURNO %d - %s ha vinto la partita!!!\n", turno, lista->nome);
                    printf(YEL"\n*==============================================*\n"RESET);
                    stampaGiocatore(lista);
                    printf(YEL"\n*==============================================*\n"RESET);

                }
                lista = lista->next;
            }
        } else {
            printf(HYEL"*========= Abbiamo un vincitore!!! =========*\n\n"RESET);
            fprintf(fileLog, "\nTURNO %d - %s ha vinto la partita!!!\n", turno, lista->nome);
            printf(YEL"\n*==============================================*\n"RESET);
            stampaGiocatore(lista);
            printf(YEL"\n*==============================================*\n"RESET);
        }
    }

    fclose(fileLog);

    return fine;
}
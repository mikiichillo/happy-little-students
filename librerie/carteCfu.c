#include "carteCfu.h"

/// Pacchetto di funzioni carte CFU

/** Caricamento intero mazzo di carte Cfu da file di testo
 * @param mazzo puntatore a lista carte Cfu
 * */
void caricamentoMazzoCfu(cartaCfu** mazzo){
    int numeroCarte;
    int check;

    cartaCfu aux;
    FILE *file = NULL;
    file = fopen(INDIRIZZO_CARTE_CFU, "r");
    if(file==NULL)
        exit(2);

//    printf("---Inizio Lettura carte Cfu---\n\n");

    do{
        check = fscanf(file, "%d %d %d %[^\n]s\n", &numeroCarte, &aux.effetto , &aux.numeroCfu, aux.nome);

        if(check == 4){
            for (int i = 0; i < numeroCarte; ++i) {
                nuovaCartaCfu(mazzo, &aux);
            }
        }
    }while (check == 4);
//    printf("---Fine lettura carte Cfu---\n\n");

    fclose(file);
}

/** Aggiunta di una singola carta in coda al mazzo Cfu
 * @param mazzo puntatore a lista mazzo
 * @param nuovaCarta dati della nuova carta da inserire in coda al mazzo
 * */
void nuovaCartaCfu(cartaCfu** mazzo, cartaCfu* nuovaCarta){
    cartaCfu *aux = *mazzo;

    cartaCfu *nuovo = (cartaCfu *) malloc(sizeof(cartaCfu));

    strcpy(nuovo->nome, nuovaCarta->nome);
    nuovo->numeroCfu = nuovaCarta->numeroCfu;
    nuovo->effetto = nuovaCarta->effetto;

    nuovo->next = NULL;

    if(*mazzo == NULL) {
        *mazzo = nuovo;
    }else{
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = nuovo;
    }
}

/** Funzione per stampare un'intera lista di carte CFU (generalmente la mano dei giocatori)
 * @param mazzo lista di carte Cfu da stampare
 */
void stampaManoCfuIntera(cartaCfu * mazzo){
    int indiceCarta = 1;
    if(mazzo == NULL){
        printf("*--------- Non hai carte CFU in mano. ---------*\n\n");
    }
    else{
        printf("*--------- Carte CFU in mano ---------*\n");
        while(mazzo != NULL){
            printf("+--------");
            printf("[%d]", indiceCarta);
            printf("--------+\n");
            stampaCartaCfu(mazzo);
            mazzo = mazzo->next;
            indiceCarta++;
        }
    }
}

/** Stampa singola carta CFU
 * @param carta carta Cfu da stampare
 */
void stampaCartaCfu(cartaCfu* carta){
    printf("Nome: %s - Cfu: %d - Effetto: ", carta->nome, carta->numeroCfu);
    switch (carta->effetto) {
        case NESSUNO:
            printf("[NESSUNO].\n");
            break;
        case SCARTAP:
            printf("[SCARTAP].\n");
            break;
        case RUBA:
            printf("[RUBA].\n");
            break;
        case SCAMBIADS:
            printf("[SCAMBIADS].\n");
            break;
        case SCARTAE:
            printf("[SCARTAE].\n");
            break;
        case SCARTAC:
            printf("[SCARTAC].\n");
            break;
        case SCAMBIAP:
            printf("[SCAMBIAP].\n");
            break;
        case DOPPIOE:
            printf("[DOPPIOE].\n");
            break;
        case SBIRCIA:
            printf("[SBIRCIA].\n");
            break;
        case SCAMBIAC:
            printf("[SCAMBIAC].\n");
            break;
        case ANNULLA:
            printf("[ANNULLA].\n");
            break;
        case AUMENTA:
            printf("[AUMENTA].\n");
            break;
        case DIMINUISCI:
            printf("[DIMINUISCI].\n");
            break;
        case INVERTI:
            printf("[INVERTI].\n");
            break;
        case SALVA:
            printf("[SALVA].\n");
            break;
        case DIROTTA:
            printf("[DIROTTA].\n");
            break;
    }
    printf("\n");
}

/** Mescolamento mazzo carte Cfu tramite algoritmo di Fisher-Yates
 * Funziona senza sapere a priori la dimensione del mazzo, in quanto la funzione la calcola al suo interno
 * @param mazzo lista di carte Cfu da mescolare
 */
void mescolaMazzoCfu(cartaCfu **mazzo){
    int contaCarte;
    int j;
    cartaCfu temporaneo;
    cartaCfu *aux;
    cartaCfu *nuovoMazzo = NULL;

    contaCarte = contaCarteMazzoCfu(mazzo);

    nuovoMazzo = (cartaCfu*) calloc(contaCarte, sizeof(cartaCfu));

    aux = *mazzo;
    for (int i = 0; i < contaCarte; ++i) {
        nuovoMazzo[i] = *aux;
        aux = aux->next;
    }

    for (int i = contaCarte-1; i > 0; i--) {
        j = randRangeIntero(0, i+1);
        temporaneo = nuovoMazzo[j];
        nuovoMazzo[j] = nuovoMazzo[i];
        nuovoMazzo[i] = temporaneo;
    }

    aux = *mazzo;
    for (int i = 0; i < contaCarte; ++i) {
        strcpy(aux->nome, nuovoMazzo[i].nome);
        aux->numeroCfu = nuovoMazzo[i].numeroCfu;
        aux->effetto = nuovoMazzo[i].effetto;
        aux = aux->next;
    }

    free(nuovoMazzo);
}

/** Pescata dal mazzo Cfu \n Viene tolta la carta dalla TESTA del mazzo e inserite in CODA alla mano
 * @param mazzo lista di carte Cfu dal quale pescare la carta
 * @param mano lista di carte Cfu nel quale inserire la carta pescata
 */
void pescaCartaCfu(cartaCfu** mazzo, cartaCfu** mano){
    if(*mazzo == NULL)
        exit(5);

    cartaCfu *cartaPescata = *mazzo;
    *mazzo = (*mazzo)->next;
    cartaPescata->next = NULL;

    nuovaCartaCfu(mano, cartaPescata);
}

/** Conta le carte all'interno di una lista di carte Cfu
 * @param mazzo lista di carte Cfu da contare
 * @return numero di carte all'interno della lista
 */
int contaCarteMazzoCfu(cartaCfu **mazzo){
    cartaCfu *aux = *mazzo;
    int carte = 0;
    while (aux != NULL){
        aux = aux->next;
        carte++;
    }
    return carte;
}

/** Eliminazione completa del mazzo CFU
 * @param mazzo lista di carte Cfu da eliminare
 */
void eliminaMazzoCfu(cartaCfu *mazzo){
    cartaCfu* attuale = mazzo;
    cartaCfu* prossimo;

    while(attuale!=NULL && attuale->next!=NULL){
        prossimo = attuale->next;
        free(attuale);
        attuale = prossimo;
    }
}

/** Funzione per giocare una carta dalla mano, eliminando la carta corrispondente e risistemando i collegamenti tra le varie carte
 * @param mano mano da cui viene scelta la carta da giocare
 * @param indice indice della carta da giocare
 * @return carta che verrà giocata nel turno
 * */
cartaCfu *giocaCarta(cartaCfu **mano, int indice){
    cartaCfu *cartaDaGiocare = NULL;
    cartaCfu *aux = *mano;
    cartaCfu *precedente;

    ///Copiati i valori della carta da giocare in una variabile
    cartaDaGiocare = (cartaCfu*)malloc(sizeof(cartaCfu));

    if(indice==1){  /// Se la carta da giocare è la prima della mano
        strcpy(cartaDaGiocare->nome, (*mano)->nome);
        cartaDaGiocare->numeroCfu = (*mano)->numeroCfu;
        cartaDaGiocare->effetto = (*mano)->effetto;
        (*mano) = (*mano)->next;
        free(aux);
    }
    else {  /// Se la carta da giocare si trova in qualunque altra posizione
        for (int i = 1; i < indice; ++i) {
            precedente = aux;
            aux = aux->next;
        }
        strcpy(cartaDaGiocare->nome, aux->nome);
        cartaDaGiocare->numeroCfu = aux->numeroCfu;
        cartaDaGiocare->effetto = aux->effetto;
        precedente->next = aux->next;
        free(aux);
    }

    return cartaDaGiocare;
}
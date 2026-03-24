#include "carteOstacolo.h"

/// Pacchetto di funzioni carte Ostacolo

/** Caricamento completo del mazzo degli ostacoli da un file di testo
 * @param mazzo puntatore a lista carte Ostacolo
 */
void caricamentoMazzoOstacolo(cartaOstacolo** mazzo){
    int numeroCarte;
    cartaOstacolo aux;

    Ostacolo tipoOstacolo = STUDIO;

    FILE *file = NULL;
    file = fopen(INDIRIZZO_CARTE_OSTACOLI, "r");
    if(file==NULL)
        exit(3);

//    printf("---Inizio Lettura carte ostacolo---\n\n");

    do{
//        printf("Lettura carte tipologia %d.\n\n", tipoOstacolo);
        fscanf(file, "%d\n", &numeroCarte);

        for (int i = 0; i < numeroCarte; ++i) {

//            printf("--Lettura carta--\n");
            fscanf(file, "%[^\n]s", aux.nome);
            fscanf(file, "\n");
            fscanf(file, "%[^\n]s", aux.descrizione);
            fscanf(file, "\n");
            aux.ostacolo = tipoOstacolo;
            nuovaCartaOstacolo(mazzo, &aux);
        }
        tipoOstacolo++;
    }while(tipoOstacolo<=ESAME);
//    printf("---Fine lettura carte ostacolo---\n\n");

    fclose(file);
}

/** Aggiunta di una singola carta in coda al mazzo degli ostacoli
 * @param mazzo puntatore a lista carte Ostacolo
 * @param nuovaCarta dati della nuova carta da inserire in coda al mazzo
 */
void nuovaCartaOstacolo(cartaOstacolo** mazzo, cartaOstacolo* nuovaCarta){
    cartaOstacolo *aux = *mazzo;

    cartaOstacolo *nuovo = (cartaOstacolo *) malloc(sizeof(cartaOstacolo));

    strcpy(nuovo->nome, nuovaCarta->nome);
    strcpy(nuovo->descrizione, nuovaCarta->descrizione);
    nuovo->ostacolo = nuovaCarta->ostacolo;
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

/** Funzione per stampare un'intera lista di carte Ostacolo
 * @param mazzo lista di carte Ostacolo da stampare
 */
void stampaMazzoOstacoloIntero(cartaOstacolo* mazzo){

    int indiceCarta = 1;

    if(mazzo==NULL){
        printf("*--------- Nessuna carta Ostacolo in possesso. ---------*\n\n");
    }else{
        printf("\n*--------- Carte Ostacolo in possesso ---------*\n");
        while(mazzo != NULL){
            printf("+--------");
            printf( "[%d]" , indiceCarta);
            printf("--------+\n");
            stampaCartaOstacolo(mazzo);
            mazzo = mazzo->next;
            indiceCarta++;
        }
    }
}

/** Stampa carta Ostacolo singola
 * @param carta carta Ostacolo da stampare
 */
void stampaCartaOstacolo(cartaOstacolo* carta){
    printf("Nome Carta: %s.\n"
           "Descrizione carta: %s.\n"
           "Tipologia carta: ", carta->nome, carta->descrizione);
    switch (carta->ostacolo) {
        case STUDIO:
            printf(HGRN "[STUDIO]" RESET);
            break;
        case SOCIALE:
            printf(HBLU "[SOCIALE]" RESET);
            break;
        case SOPRAVVIVENZA:
            printf(HYEL "[SOPRAVVIVENZA]" RESET);
            break;
        case ESAME:
            printf(HRED "[ESAME]" RESET);
            break;
    }
    printf(".\n\n");
}

/** Mescolamento mazzo carte Ostacolo tramite algoritmo di Fisher-Yates
 * Funziona senza sapere a priori la dimensione del mazzo, in quanto la funzione la calcola al suo interno
 * @param mazzo lista di carte Ostacolo da mescolare
 */
void mescolaMazzoOstacolo(cartaOstacolo **mazzo){
    int contaCarte;
    int j;
    cartaOstacolo temporaneo;
    cartaOstacolo *aux;
    cartaOstacolo *nuovoMazzo = NULL;

    contaCarte = contaCarteMazzoOstacolo(mazzo);

    nuovoMazzo = (cartaOstacolo*) calloc(contaCarte, sizeof(cartaOstacolo));

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
        strcpy(aux->descrizione, nuovoMazzo[i].descrizione);
        aux->ostacolo = nuovoMazzo[i].ostacolo;
        aux = aux->next;
    }

    free(nuovoMazzo);
}

/** Pescata dal mazzo Ostacolo \n Viene tolta la carta dalla TESTA del mazzo e inserite in CODA alla mano
 * @param mazzo lista di carte Ostacolo dal quale pescare la carta
 * @param mano lista di carte Ostacolo nel quale inserire la carta pescata
 */
void pescaCartaOstacolo(cartaOstacolo ** mazzo, cartaOstacolo ** mano){
    if(*mazzo == NULL)
        exit(6);

    cartaOstacolo *cartaPescata = *mazzo;
    *mazzo = (*mazzo)->next;
    cartaPescata->next = NULL;

    nuovaCartaOstacolo(mano, cartaPescata);
}

/** Conta le carte all'interno di una lista di carte Ostacolo
 * @param mazzo lista di carte Ostacolo da contare
 * @return numero di carte all'interno della lista
 */
int contaCarteMazzoOstacolo(cartaOstacolo **mazzo){
    cartaOstacolo *aux = *mazzo;
    int carte = 0;
    while (aux != NULL){
        aux = aux->next;
        carte++;
    }
    return carte;
}

/** Eliminazione completa del mazzo Ostacoli
 * @param mazzo lista di carte Ostacolo da eliminare
 */
void eliminaMazzoOstacolo(cartaOstacolo* mazzo){
    cartaOstacolo* attuale = mazzo;
    cartaOstacolo* prossimo;

    while(attuale!=NULL && attuale->next!=NULL){
        prossimo = attuale->next;
        free(attuale);
        attuale = prossimo;
    }
}
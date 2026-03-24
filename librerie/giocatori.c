#include "giocatori.h"

/// Pacchetto di funzioni Giocatore

/** Riempimento dell'elenco di giocatori
 * @param listaGiocatori lista di giocatori da caricare
 * @param numGiocatori numero di giocatori in gara a inizio gioco
 * @param numPersonaggi numero di personaggi presenti nel gioco
 * @param listaPersonaggi lista di personaggi da assegnare
 */
void creazioneElencoGiocatori(giocatore **listaGiocatori, int numGiocatori, int numPersonaggi, personaggio *listaPersonaggi){
    giocatore *nuovo = (giocatore*) malloc(sizeof(giocatore));
    bool taken = false, check = false;
    int indicePersonaggioScelto, k;
    int *personaggiPresi = (int*) calloc(numGiocatori, sizeof(int));


    for (int i = 0; i < numGiocatori; ++i) {
        personaggiPresi[i] = VALORE_NULLO;
    }

    for (int i = 0; i < numGiocatori; ++i) {
        printf("\nInserire il nome del giocatore %d:\n", i+1);
        getchar();
        scanf("%[^\n]s", nuovo->nome);
        nuovo->cfuAccumulati = 0;


        do {
            taken = false;
            indicePersonaggioScelto = randRangeIntero(0, numPersonaggi-1);

            for (int j = 0; j < numGiocatori; ++j) {
                if(indicePersonaggioScelto == personaggiPresi[j]) {
                    taken = true;
                }
            }

            if(!taken){
                nuovo->personaggio = listaPersonaggi[indicePersonaggioScelto];
                k=0;
                check = false;
                do{
                    if(personaggiPresi[k] == VALORE_NULLO) {
                        personaggiPresi[k] = indicePersonaggioScelto;
                        check = true;
                    }
                    k++;
                }while(!check && k<numGiocatori);
            }

        }while(taken);

        nuovoGiocatore(listaGiocatori, nuovo);

    }

}

/** Inserimento dati di un nuovo giocatore all'interno dell'elenco di giocatori (il mazzo di carte Cfu non viene ancora caricato)
 * @param elenco lista di giocatori in cui inserire un nuovo giocatore (in coda)
 * @param nuovoGiocatore dati del nuovo giocatore da inserire nella lista
 */
void nuovoGiocatore(giocatore **elenco, giocatore *nuovoGiocatore){
    giocatore *aux = *elenco;

    giocatore *nuovo = (giocatore *) malloc(sizeof(giocatore));

    strcpy(nuovo->nome, nuovoGiocatore->nome);
    nuovo->cfuAccumulati = nuovoGiocatore->cfuAccumulati;
    nuovo->personaggio = nuovoGiocatore->personaggio;
    nuovo->mazzoCfu = NULL;
    nuovo->mazzoOstacolo = NULL;


    nuovo->next = NULL;

    if(*elenco == NULL) {
        *elenco = nuovo;
    }else{
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = nuovo;
    }
}

/** Stampa intero elenco di giocatori
 * @param elenco lista di giocatori da stampare
 */
void stampaElencoGiocatori(giocatore *elenco){
    while(elenco!=NULL){
        stampaGiocatore(elenco);
        elenco = elenco->next;
    }
}

/** Stampa informazioni di un singolo giocatore
 * @param giocatore singolo giocatore da stampare
 */
void stampaGiocatore(giocatore *giocatore){
    printf( "*----- Informazioni Giocatore -----*\n" );
    printf("Nome: %s\n", giocatore->nome);
    printf("CFU: %d\n", giocatore->cfuAccumulati);
    stampaPersonaggio(giocatore->personaggio);
    stampaMazzoOstacoloIntero(giocatore->mazzoOstacolo);
}

/** Elimina l'intero elenco di giocatori
 * @param elenco lista di giocatori da eliminare
 */
void eliminaElencoGiocatori(giocatore *elenco){
    giocatore *attuale = elenco;
    giocatore *prossimo;

    while (attuale!=NULL && attuale->next!=NULL){
        prossimo = attuale->next;
        free(attuale);
        attuale = prossimo;
    }
}
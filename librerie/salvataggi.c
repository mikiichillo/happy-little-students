#include "salvataggi.h"

/** Funzione per caricare i dati della partita a partire da un file di salvataggio.
 *
 * @param nomeFile nome del file di salvataggio, escludendo l'estensione che verrà aggiunta in automatico dal programma
 * @param elencoGiocatori lista dei giocatori in partita
 * @param mazzoCfu mazzo di carte Cfu da cui verranno pescate le carte durante la partita
 * @param pilaDegliScarti pila degli scarti delle carte Cfu, in cui verranno messe le carte Cfu giocate
 * @param mazzoOstacolo mazzo di carte Ostacolo da cui verranno pescate le carte Ostacolo a inizio di ogni turno
 * @param turno numero del turno in cui è stata interrotta la partita
 * @param numGiocatori numero di giocatori in gara
 * @return TRUE se viene caricato un file di salvataggio, FALSE se viene creato un nuovo file di salvataggio
 */
bool caricaSalvataggio(char nomeFile[CHAR_MAX], giocatore **elencoGiocatori, cartaCfu **mazzoCfu, cartaCfu **pilaDegliScarti, cartaOstacolo **mazzoOstacolo, int *turno, int *numGiocatori, FILE *fileLog) {
    FILE *fp = NULL;

    int count = 0;
    int scelta;
    bool nuovoFile = false;

    giocatore *first = *elencoGiocatori;

    giocatore giocatoriAux;
    cartaCfu cartaCfuAux;
    cartaOstacolo cartaOstacoloAux;

    char auxNomeFile[CHAR_MAX];
    char stringClear[CHAR_MAX] = "";

    printf("\n*==================== Caricamento Salvataggi ====================*\n\n\n");

    fileLog = fopen("log.txt", "a");

    printf("*--- Vuoi caricare un salvataggio? ---*\n"
           "[%d] Si.\n"
           "[%d] No.\n\n", TASTO_1, TASTO_0);
    do{
        scanf("%d", &scelta);
        if(scelta != TASTO_0 && scelta != TASTO_1){
            printf("Inserisci un valore valido.\n");
        }
    }while(scelta != TASTO_0 && scelta != TASTO_1);

    if(scelta == TASTO_1) {
        do {
            strcpy(nomeFile, stringClear);
            printf("\n\n*--- Che file di salvataggio vuoi caricare? (non inserire l'estensione .sav) ---*\n\n");
            scanf("%s", auxNomeFile);
            strcat(nomeFile, INDIRIZZO_CARTELLA_SALVATAGGI);
            strcat(nomeFile, auxNomeFile);
            strcat(nomeFile, ESTENSIONE_FILE_SALVATAGGIO);

            fp = fopen(nomeFile, "rb");

            if (fp == NULL) {     /// Se il file .sav desiderato non esiste, viene creato con il nome scelto.

                printf("\n\nIl file di salvataggio scelto non esiste. Vuoi creare uno nuovo file con questo nome o caricare un'altro file?\n"
                       "[%d] Crea nuovo file con questo nome.\n"
                       "[%d] Scegli un altro file di salvataggio.\n\n", TASTO_1, TASTO_0);
                do{
                    scanf("%d", &scelta);
                    if(scelta != TASTO_0 && scelta != TASTO_1){
                        printf("Inserisci un valore valido.\n");
                    }
                }while(scelta != TASTO_0 && scelta != TASTO_1);

                fclose(fp);

                if(scelta == TASTO_1){
                    fp = fopen(nomeFile, "wb");
                    fclose(fp);

                    nuovoFile = true;

                    printf("\n\n*--- Creazione nuovo file di salvataggio avvenuta con successo ---*\n\n");
                    printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);

                    fprintf(fileLog, "---> Nuovo file di salvataggio creato \"%s\".\n", nomeFile);

                    getchar();
                    getchar();
                    clearConsole();
                }
            }
        }while(scelta == TASTO_0 && fp == NULL);

    }
    else if (scelta == TASTO_0){
        printf("\n*--- Come vuoi chiamare il tuo nuovo salvataggio? (non inserire l'estensione .sav) ---*\n");
        scanf("%s", auxNomeFile);

        strcat(nomeFile, INDIRIZZO_CARTELLA_SALVATAGGI);
        strcat(nomeFile, auxNomeFile);
        strcat(nomeFile, ESTENSIONE_FILE_SALVATAGGIO);

        fp = fopen(nomeFile, "wb");       /// Creazione di un nuovo salvataggio
        fclose(fp);

        nuovoFile = true;

        printf("\n*--- Creazione nuovo file di salvataggio avvenuta con successo ---*\n\n");
        printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);


        fprintf(fileLog, "---> Nuovo file di salvataggio creato \"%s\".\n", nomeFile);

        getchar();
        getchar();
        clearConsole();
    }

    if(!nuovoFile) {     /// Caricamento delle informazioni da file se non viene creato un nuovo file di salvataggio

        /// Lettura numero giocatori
        fread(numGiocatori, sizeof(int), 1, fp);
        printf("\n*--- Numero Giocatori --> [%d] ---*\n\n", *numGiocatori);

        for (int i = 0; i < *numGiocatori; ++i) {

            ///Lettura informazioni
            fread(&giocatoriAux, sizeof(giocatore), 1, fp);
            nuovoGiocatore(elencoGiocatori, &giocatoriAux);

            if (i == 0) {     /// Dopo la creazioe del primo giocatore, viene salvata la sua posizione di memoria (la posizione della testa della lista)
                first = *elencoGiocatori;
            }

            for (int j = 0; j < i; ++j) {   /// Viene fatta scorrere a ogni iterazione la lista fino al nuovo giocatore creato, per potergli assegnare le sue carte
                if ((*elencoGiocatori)->next != NULL)
                    (*elencoGiocatori) = (*elencoGiocatori)->next;
            }

            ///Lettura carte cfu giocatore
            (*elencoGiocatori)->mazzoCfu = NULL;
            for (int j = 0; j < MAX_CARTE_MANO; ++j) {
                fread(&cartaCfuAux, sizeof(cartaCfu), 1, fp);
                nuovaCartaCfu(&(*elencoGiocatori)->mazzoCfu, &cartaCfuAux);
            }

            ///Lettura carte ostacolo giocatore
            fread(&count, sizeof(int), 1, fp);

            (*elencoGiocatori)->mazzoOstacolo = NULL;
            for (int j = 0; j < count; ++j) {
                fread(&cartaOstacoloAux, sizeof(cartaOstacolo), 1, fp);
                nuovaCartaOstacolo(&(*elencoGiocatori)->mazzoOstacolo, &cartaOstacoloAux);
            }

            (*elencoGiocatori) = first;

        }

        /// Lettura mazzo carte CFU
        fread(&count, sizeof(int), 1, fp);
        for (int i = 0; i < count; ++i) {
            fread(&cartaCfuAux, sizeof(cartaCfu), 1, fp);
            nuovaCartaCfu(mazzoCfu, &cartaCfuAux);
        }

        /// Lettura pila degli scarti
        fread(&count, sizeof(int), 1, fp);
        for (int i = 0; i < count; ++i) {
            fread(&cartaCfuAux, sizeof(cartaCfu), 1, fp);
            nuovaCartaCfu(pilaDegliScarti, &cartaCfuAux);
        }

        /// Lettura mazzo carte Ostacolo
        fread(&count, sizeof(int), 1, fp);
        for (int i = 0; i < count; ++i) {
            fread(&cartaOstacoloAux, sizeof(cartaOstacolo), 1, fp);
            nuovaCartaOstacolo(mazzoOstacolo, &cartaOstacoloAux);
        }

        *turno = 1;  /// Inizializzo i automatico il valore del turno a 1 in caso non sia contenuto questo valore alla fine del file
        fread(turno, sizeof(int), 1, fp);

        fclose(fp);
        printf("\n*--- Salvataggio caricato con successo ---* \n\n");
        printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);

        fprintf(fileLog, "---> File di salvataggio caricato: %s\n", nomeFile);


        getchar();
        getchar();
        clearConsole();
    }

    fclose(fileLog);

    return !nuovoFile;      /// Se è stato creato un nuovo file allora non sono state caricate informazioni
}

/** Funzione per salvare i dati della partita su un file di salvataggio
 *
* @param nomeFile nome del file di salvataggio, escludendo l'estensione che verrà aggiunta in automatico dal programma
 * @param elencoGiocatori lista dei giocatori in partita
 * @param mazzoCfu mazzo di carte Cfu da cui verranno pescate le carte durante la partita
 * @param pilaDegliScarti pila degli scarti delle carte Cfu, in cui verranno messe le carte Cfu giocate
 * @param mazzoOstacolo mazzo di carte Ostacolo da cui verranno pescate le carte Ostacolo a inizio di ogni turno
 * @param turno numero del turno attuale della partita
 * @param numGiocatori numero di giocatori in gara
 */
void salvaSalvataggio(char nomeFile[CHAR_MAX], giocatore *elencoGiocatori, cartaCfu *mazzoCfu, cartaCfu *pilaDegliScarti, cartaOstacolo *mazzoOstacolo, int turno, int numGiocatori){
    FILE *fp = NULL;

    int count = 0;

    giocatore *giocatoriAux = elencoGiocatori;
    cartaCfu *cartaCfuAux;
    cartaOstacolo *cartaOstacoloAux;

    fp = fopen(nomeFile, "wb");

    /// Scrittura numero di giocatori
    fwrite(&numGiocatori, sizeof(int), 1, fp);


    for (int i = 0; i < numGiocatori; ++i) {  /// Salvataggio delle informazioni di ciascun giocatore, in ordine

        /// Scrittura informazioni giocatore
        fwrite(giocatoriAux, sizeof(giocatore), 1, fp);


        /// Scrittura carte Cfu in mano al giocatore
        cartaCfuAux = giocatoriAux->mazzoCfu;

        for (int j = 0; j < MAX_CARTE_MANO; ++j) {
            fwrite(cartaCfuAux, sizeof(cartaCfu), 1, fp);
            cartaCfuAux = cartaCfuAux->next;
        }


        ///Lettura carte Ostacolo giocatore
        count = contaCarteMazzoOstacolo(&giocatoriAux->mazzoOstacolo);
        fwrite(&count, sizeof(int), 1, fp);   //Numero di carte ostacolo del giocatore

        cartaOstacoloAux = giocatoriAux->mazzoOstacolo;
        for (int j = 0; j < count; ++j) {
            fwrite(cartaOstacoloAux, sizeof(cartaOstacolo), 1, fp);
            cartaOstacoloAux = cartaOstacoloAux->next;
        }
        giocatoriAux = giocatoriAux->next;
    }

    /// Scrittura mazzo carte CFU
    count = contaCarteMazzoCfu(&mazzoCfu);          /// Numero di carte nel mazzo Cfu
    fwrite(&count, sizeof(int), 1, fp);

    cartaCfuAux = mazzoCfu;
    for (int i = 0; i < count; ++i) {
        fwrite(cartaCfuAux, sizeof(cartaCfu), 1, fp);
        cartaCfuAux = cartaCfuAux->next;
    }


    /// Scrittura pila degli scarti
    count = contaCarteMazzoCfu(&pilaDegliScarti);       /// Numero di carte nella Pila degli Scarti
    fwrite(&count, sizeof(int), 1, fp);

    cartaCfuAux = pilaDegliScarti;
    for (int i = 0; i < count; ++i) {
        fwrite(cartaCfuAux, sizeof(cartaCfu), 1, fp);
        cartaCfuAux = cartaCfuAux->next;
    }


    /// Scrittura mazzo carte Ostacolo
    count = contaCarteMazzoOstacolo(&mazzoOstacolo);        /// Numero di carte nel mazzo Ostacolo
    fwrite(&count, sizeof(int), 1, fp);

    cartaOstacoloAux = mazzoOstacolo;
    for (int i = 0; i < count; ++i) {
        fwrite(cartaOstacoloAux, sizeof(cartaOstacolo), 1, fp);
        cartaOstacoloAux = cartaOstacoloAux->next;
    }

    fwrite(&turno, sizeof(int), 1, fp);

    fclose(fp);
}

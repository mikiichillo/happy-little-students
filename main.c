//Nome: Michele Chillotti (m.chillotti22@studenti.unica.it)
//Matricola: 60/61/66441
//Tipologia progetto: Medio

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "librerie/struttureGioco.h"
#include "librerie/giocatori.h"
#include "librerie/carteCfu.h"
#include "librerie/carteOstacolo.h"
#include "librerie/personaggi.h"
#include "librerie/gioco.h"
#include "librerie/calcoloPunteggi.h"
#include "librerie/salvataggi.h"
#include "librerie/ANSI-color-codes.h"


int main() {

    giocatore *elencoGiocatori = NULL;
    int numGiocatori, scelta;

    personaggio *listaPersonaggi = NULL;
    int numPersonaggi = 0, turno = 1;
    cartaCfu *mazzoCfu = NULL;
    cartaCfu *pilaDegliScarti = NULL;
    cartaOstacolo *mazzoOstacolo = NULL;

    char nomeFile[CHAR_MAX] = "";

    ///Puntatore a prima posizione dell'elenco di giocatori
    giocatore *first = NULL;

    srand(time(NULL));

    /** ----- INIZIO PREPARAZIONE ----- **/

    printf(HCYN  "+----------------------------------------------------------------------------+\n"
           "| _   _                                     _       _   _     _     _        |\n"
           "|| | | |   __ _   _ __    _ __    _   _    | |     (_) | |_  | |_  | |   ___ |\n"
           "|| |_| |  / _` | | '_ \\  | '_ \\  | | | |   | |     | | | __| | __| | |  / _ \\|\n"
           "||  _  | | (_| | | |_) | | |_) | | |_| |   | |___  | | | |_  | |_  | | |  __/|\n"
           "||_| |_|  \\__,_| | .__/  | .__/   \\__, |   |_____| |_|  \\__|  \\__| |_|  \\___||\n"
           "| ____    _      |_|     |_|_     |___/        _                             |\n"
           "|/ ___|  | |_   _   _    __| |   ___   _ __   | |_   ___                     |\n"
           "|\\___ \\  | __| | | | |  / _` |  / _ \\ | '_ \\  | __| / __|                    |\n"
           "| ___) | | |_  | |_| | | (_| | |  __/ | | | | | |_  \\__ \\                    |\n"
           "||____/   \\__|  \\__,_|  \\__,_|  \\___| |_| |_|  \\__| |___/                    |\n"
           "+----------------------------------------------------------------------------+" RESET);


    printf(BLINK "\n\n\n\n\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);

    getchar();

    clearConsole();

    bool salvataggioCaricato = false;

    FILE *fileLog = fopen("log.txt", "w");
    fclose(fileLog);


    salvataggioCaricato = caricaSalvataggio(nomeFile, &elencoGiocatori, &mazzoCfu, &pilaDegliScarti, &mazzoOstacolo, &turno, &numGiocatori, fileLog);


    if(!salvataggioCaricato){
        ///Inizializzazione mazzi di carte e lista di personaggi da file di testo

        caricamentoMazzoCfu(&mazzoCfu);
        caricamentoMazzoOstacolo(&mazzoOstacolo);
        listaPersonaggi = caricaPersonaggi(&numPersonaggi);


        mescolaMazzoCfu(&mazzoCfu);
        mescolaMazzoOstacolo(&mazzoOstacolo);

        ///Inizializzazione lista giocatori
        do {
            printf("Quanti giocatori devono partecipare?\n");
            scanf("%d", &numGiocatori);
            if (numGiocatori < NUM_MIN_GIOCATORI || numGiocatori > NUM_MAX_GIOCATORI) {
                printf("Inserire un numero di giocatori valido.\n");
            }
        } while (numGiocatori < NUM_MIN_GIOCATORI || numGiocatori > NUM_MAX_GIOCATORI);

        creazioneElencoGiocatori(&elencoGiocatori, numGiocatori, numPersonaggi, listaPersonaggi);

        first = elencoGiocatori;

        ///Pescata prime cinque carte iniziali per ciascun giocatore
        for (int j = 0; j < numGiocatori; ++j) {
            for (int i = 0; i < MAX_CARTE_MANO; ++i) {
                pescaCartaCfu(&mazzoCfu, &elencoGiocatori->mazzoCfu);
            }
            if(elencoGiocatori->next!=NULL)
                elencoGiocatori = elencoGiocatori->next;
        }
        elencoGiocatori = first;
    }
    else{
        first = elencoGiocatori;
    }


    /** ----- FINE PREPARAZIONE ----- **/


    /** ----- INIZIO GIOCO ----- **/

    /// Variabili gioco
    bool partitaFinita, vincitoriMultipli, perdentiMultipli, perdenteTurnoExtra, altreIstantanee, raddoppiaTutto, salvo;
    partitaFinita = vincitoriMultipli = perdentiMultipli = perdenteTurnoExtra = altreIstantanee = raddoppiaTutto = salvo = false;

    int indiceGiocatoreVincente, indiceGiocatorePerdente, carteInMano, valoreMassimo, valoreMinimo;
    int *carteCfuPiuAlte = (int *) calloc(numGiocatori, sizeof(int));

    cartaCfu *arrayCarteGiocate = (cartaCfu *) calloc(numGiocatori, sizeof(cartaCfu));

    cartaOstacolo *cartaOstacoloTurno = NULL;


    do{
        clearConsole();

        /** Viene eventualmente rimescolato il mazzo degli scarti nel mazzo Cfu **/
        rimescolaPilaScarti(&mazzoCfu, &pilaDegliScarti);

        for (int i = 0; i < numGiocatori; ++i) {
            /** Pesca carte fino a tornare al massimo numero di carte che si può avere in mano **/
            carteInMano = contaCarteMazzoCfu(&elencoGiocatori->mazzoCfu);
            for (; carteInMano < MAX_CARTE_MANO; ++carteInMano) {
                rimescolaPilaScarti(&mazzoCfu, &pilaDegliScarti);
                pescaCartaCfu(&mazzoCfu, &elencoGiocatori->mazzoCfu);
            }
            elencoGiocatori = elencoGiocatori->next;
        }
        elencoGiocatori = first;

        if(salvataggioCaricato){
            salvataggioCaricato = false;
        }else{
            salvaSalvataggio(nomeFile, elencoGiocatori, mazzoCfu, pilaDegliScarti, mazzoOstacolo, turno, numGiocatori);
        }

        pescaCartaOstacolo(&mazzoOstacolo, &cartaOstacoloTurno);

        arrayCarteGiocate = (cartaCfu *) realloc(arrayCarteGiocate, numGiocatori*sizeof(cartaCfu));

        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; indiceGiocatore++) {

            /** Controllo che il giocatore abbia carte Cfu punto giocabili, altrimenti gli faccio scartare tutta la mano, pescare altre 5 carte e rieseguo il controllo **/
            while (!controlloCarteCfuPunto(&elencoGiocatori->mazzoCfu)) {
                for (int i = 0; i < MAX_CARTE_MANO; ++i) {
                    giocaCarta(&elencoGiocatori->mazzoCfu, 1);
                }
                for (int i = 0; i < MAX_CARTE_MANO; ++i) {
                    rimescolaPilaScarti(&mazzoCfu, &pilaDegliScarti);
                    pescaCartaCfu(&mazzoCfu, &elencoGiocatori->mazzoCfu);
                }
            }

            do{
                /** Stampa informazioni turno attuale **/
                printf(BHWHT "*========================= [TURNO %d] =========================*\n\n"
                              BWHT "*--------- Carte Ostacolo pescata ---------*\n" RESET, turno);
                stampaCartaOstacolo(cartaOstacoloTurno);
                printf("\n");

                /** Informazioni giocatore **/
                stampaGiocatore(elencoGiocatori);
                printf("\n");
                stampaManoCfuIntera(elencoGiocatori->mazzoCfu);

                /** Scelta azione e suo svolgimento **/
                scelta = menuInizioTurno();
                azioneTurno(&scelta, &elencoGiocatori, &elencoGiocatori->mazzoCfu, &pilaDegliScarti, arrayCarteGiocate, indiceGiocatore, numGiocatori, turno, fileLog);

                /** Opzione di terminazione volontaria del gioco **/
                if(scelta == TASTO_0){
                    eliminaElencoGiocatori(elencoGiocatori);
                    eliminaMazzoCfu(mazzoCfu);
                    eliminaMazzoOstacolo(mazzoOstacolo);
                    free(listaPersonaggi);
                    free(arrayCarteGiocate);
                    free(carteCfuPiuAlte);
                    exit(1);
                }

            }while(scelta != TASTO_1);

            printf("\n*=============== Hai finito il tuo turno ===============*\n\n\n");
            printf(BLINK "\n*=============== Premere un tasto qualunque per continuare (non il tasto di spegnimento :D ) ===============*\n" RESET);
            
            getchar();
            getchar();

            elencoGiocatori = elencoGiocatori->next;    /// Scorrimento della lista dei giocatori

            clearConsole();
        }

        elencoGiocatori = first;    /// Ritorno all'inizio della lista elencoGiocatori

        /** Applicazione bonus e malus legati alla carta Ostacolo estratta **/
        applicaBonusMalus(arrayCarteGiocate, elencoGiocatori, numGiocatori, *cartaOstacoloTurno);


        /** Stampa dei giocatori e delle rispettive carte giocate **/
        stampaCarteGiocate(arrayCarteGiocate, elencoGiocatori, numGiocatori);
        elencoGiocatori = first;


        /// Controllo quale carta ha il punteggio maggiore
        trovaValoreMassimo(arrayCarteGiocate, numGiocatori, &valoreMassimo);

        /// Controllo quale carta ha il punteggio minore
        trovaValoreMinimo(arrayCarteGiocate, numGiocatori, &valoreMinimo);

        printf(BLINK "\n\n*=========== Premi invio per proseguire all'attivazione degli effetti delle carte CFU punto ===========*\n\n" RESET);
        getchar();
        getchar();
        clearConsole();

        /// Attivo gli effetti delle carte CFU punto
        effettiCarteCfuPunto(&elencoGiocatori, numGiocatori, arrayCarteGiocate, &mazzoCfu, &pilaDegliScarti, valoreMassimo, valoreMinimo, &raddoppiaTutto);

        printf(BLINK "\n\n*=========== Premi invio per andare a giocare le carte Istantanee ===========*\n\n" RESET);
        getchar();
        getchar();
        clearConsole();

        /// Attivo gli effetti delle carte CFU istantanee
        do{
            altreIstantanee = giocaIstantanee(&elencoGiocatori, numGiocatori, arrayCarteGiocate, &pilaDegliScarti, valoreMassimo, valoreMinimo, raddoppiaTutto, fileLog, turno);
        }while(altreIstantanee);

        printf("\n\n*=========== Premi invio per proseguire al calcolo finale del punteggio ===========*\n\n");
        getchar();
        getchar();
        clearConsole();

        /// Ri-controllo quale carta ha il punteggio maggiore dopo aver applicato gli effetti delle carte
        trovaValoreMassimo(arrayCarteGiocate, numGiocatori, &valoreMassimo);

        /// Ri-controllo quale carta ha il punteggio minore dopo aver applicato gli effetti delle carte
        trovaValoreMinimo(arrayCarteGiocate, numGiocatori, &valoreMinimo);

        /// Controllo quali giocatori hanno il punteggio massimo e minimo
        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; ++indiceGiocatore) {
            if(arrayCarteGiocate[indiceGiocatore].numeroCfu == valoreMassimo){
                indiceGiocatoreVincente = indiceGiocatore;
            }
        }
        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; ++indiceGiocatore) {
            if(arrayCarteGiocate[indiceGiocatore].numeroCfu == valoreMinimo){
                indiceGiocatorePerdente = indiceGiocatore;
            }
        }

        printf("\n\n*----- Valori aggiornati -----*\n\n");
        stampaCarteGiocate(arrayCarteGiocate, elencoGiocatori, numGiocatori);
        elencoGiocatori = first;


        /// Controllo se ci sono più vincitori
        vincitoriMultipli = vincitoriMultipliFunction(arrayCarteGiocate, numGiocatori, valoreMassimo);

        /// Controllo se ci sono più perdenti
        perdentiMultipli = perdentiMultipliFunction(arrayCarteGiocate, numGiocatori, valoreMinimo);



        /// Mostro a schermo chi ha vinto il turno e assegno i cfu ai vincitori

        if(valoreMassimo == valoreMinimo){
            printf(HYEL"\n\n*=== Tutti i giocatori sono a parimerito. Non ci sono ne vincitori ne vinti. ===*\n\n" RESET);

            fileLog = fopen("log.txt", "a");
            fprintf(fileLog, "TURNO %d - Pareggio.\n", turno);
            fclose(fileLog);

            pescaCartaOstacolo(&cartaOstacoloTurno, &mazzoOstacolo);
        }
        else {

            if (vincitoriMultipli) {
                printf(GRN "\n\n*===== Nel turno %d ci sono piu' vincitori =====*\n" RESET, turno);
                for (int i = 0; i < numGiocatori; ++i) {
                    if (arrayCarteGiocate[i].numeroCfu == valoreMassimo) {
                        printf("->" GRN " %s\n" RESET, elencoGiocatori->nome);

                        fileLog = fopen("log.txt", "a");
                        fprintf(fileLog, "TURNO %d - Il giocatore %s ha vinto il turno guadagnando %d CFU.\n", turno, elencoGiocatori->nome, valoreMassimo);
                        fclose(fileLog);
                        elencoGiocatori->cfuAccumulati += valoreMassimo;
                    }
                    elencoGiocatori = elencoGiocatori->next;
                }
                printf("\n");
            } else {
                for (int i = 0; i < indiceGiocatoreVincente; ++i) {
                    elencoGiocatori = elencoGiocatori->next;
                }
                printf(GRN "\n\n*===== Il vincitore del turno %d e': %s =====*\n\n" RESET, turno, elencoGiocatori->nome);

                fileLog = fopen("log.txt", "a");
                fprintf(fileLog, "TURNO %d - Il giocatore %s ha vinto il turno guadagnando %d CFU.\n", turno, elencoGiocatori->nome, valoreMassimo);
                fclose(fileLog);

                elencoGiocatori->cfuAccumulati += valoreMassimo;
            }
            elencoGiocatori = first;

            /// Mostro a schermo chi ha perso il turno

            if (perdentiMultipli) {
                printf(HRED "\n\n*===== Nel turno %d ci sono piu' perdenti =====*\n" RESET, turno);
                for (int i = 0; i < numGiocatori; ++i) {
                    if (arrayCarteGiocate[i].numeroCfu == valoreMinimo) {
                        printf("->" HRED" %s\n" RESET, elencoGiocatori->nome);
                    }
                    elencoGiocatori = elencoGiocatori->next;
                }
                printf("\n");
                elencoGiocatori = first;

                printf("\n\n*=========== INIZIO TURNO SPAREGGIO ===========*\n\n");
                perdenteTurnoExtra = turnoExtraPerdenti(valoreMinimo, &elencoGiocatori, numGiocatori, arrayCarteGiocate, &pilaDegliScarti, &indiceGiocatorePerdente, fileLog, turno);

            }
            if (!perdentiMultipli || perdenteTurnoExtra) {

                for (int i = 0; i < indiceGiocatorePerdente; ++i) {
                    elencoGiocatori = elencoGiocatori->next;
                }
                printf(HRED "\n\n*===== %s e' il giocatore con meno punti nel turno %d =====*\n\n" RESET, elencoGiocatori->nome, turno);

                elencoGiocatori = first;

                int indiceAux;
                do{
                    indiceAux = indiceGiocatorePerdente; // Teniamo una copia dell'indice del giocatore perdente
                    salvo = giocaSalva(&elencoGiocatori, numGiocatori, &pilaDegliScarti, &indiceGiocatorePerdente, cartaOstacoloTurno, &mazzoOstacolo, fileLog, turno);
                }while(indiceAux != indiceGiocatorePerdente); // Se viene giocata una carta "Snitcha il collega", si deve permettere al nuovo giocatore perdente di poter giocare a sua volta una carta "Esame di recupero" o "Snitcha il collega"

                elencoGiocatori = first;

                if(!salvo){
                    for (int i = 0; i < indiceGiocatorePerdente; ++i) {
                        elencoGiocatori = elencoGiocatori->next;
                    }
                    printf(HRED "\n\n*===== Il perdente del turno %d e': %s =====*\n\n" RESET, turno, elencoGiocatori->nome);
                    fileLog = fopen("log.txt", "a");
                    fprintf(fileLog, "TURNO %d - Il giocatore %s ha perso il turno e ha pescato la carta ostacolo \"%s\".\n", turno, elencoGiocatori->nome, cartaOstacoloTurno->nome);
                    fclose(fileLog);
                    pescaCartaOstacolo(&cartaOstacoloTurno, &elencoGiocatori->mazzoOstacolo);
                }
                else{ // La carta viene messa in fondo al mazzo degli Ostacoli
                    pescaCartaOstacolo(&cartaOstacoloTurno, &mazzoOstacolo);
                    printf(HBLU "*===== Nel turno %d non ci sono perdenti =====*\n\n" RESET, turno);
                    fileLog = fopen("log.txt", "a");
                    fprintf(fileLog, "TURNO %d - Nessun giocatore ha perso il turno.\n", turno);
                    fclose(fileLog);
                }
            }
            else{ // La carta viene messa in fondo al mazzo degli Ostacoli
                pescaCartaOstacolo(&cartaOstacoloTurno, &mazzoOstacolo);
                printf(HBLU "*===== Nel turno %d non ci sono perdenti =====*\n\n" RESET, turno);
                fileLog = fopen("log.txt", "a");
                fprintf(fileLog, "TURNO %d - Nessun giocatore ha perso il turno.\n", turno);
                fclose(fileLog);
            }
            elencoGiocatori = first;

            for (int i = 0; i < numGiocatori; ++i) {
                (elencoGiocatori->cfuAccumulati) += contaCarteMazzoOstacolo(&elencoGiocatori->mazzoOstacolo);
                fileLog = fopen("log.txt", "a");
                fprintf(fileLog, "TURNO %d - Il giocatore %s ha ottenuto %d CFU per le carte Ostacolo in suo possesso.\n", turno, elencoGiocatori->nome, contaCarteMazzoOstacolo(&elencoGiocatori->mazzoOstacolo));
                fclose(fileLog);
                elencoGiocatori = elencoGiocatori->next;
            }

        }

        elencoGiocatori = first;

        /** --- FASE FINE TURNO --- **/
        /* aggiornamento informazioni partita */

        for (int indiceGiocatore = 0; indiceGiocatore < numGiocatori; indiceGiocatore++) {
            eliminazioneGiocatori(&elencoGiocatori, &numGiocatori, &pilaDegliScarti, &mazzoOstacolo, indiceGiocatore, fileLog, turno);   // Controllo eventuale eliminazione dei giocatori.
        }

        printf("\n\n+========= FINE TURNO =========+\n\n");
        getchar();
        getchar();

        clearConsole();

        first = elencoGiocatori;

        turno++;        // Incremento turno

        partitaFinita = finePartita(numGiocatori, elencoGiocatori, fileLog, turno);     // Controllo se ci sono le condizioni per le quali un giocatore abbia vinto la partita.

        /** --- FINE TURNO --- **/

    }while(!partitaFinita);


    /** ----- FINE GIOCO ----- **/


    printf(HYEL "\n\n\n\n*---------------------------------*\n"
           "|                                 |\n"
           "|         PARTITA FINITA          |\n"
           "|                                 |\n"
           "*---------------------------------*\n" RESET);

    fprintf(fileLog, "TURNO %d - Fine partita.", turno);

    getchar();
    getchar();



    /** ----- SMANTELLAMENTO ----- **/


    eliminaElencoGiocatori(elencoGiocatori);
    eliminaMazzoCfu(mazzoCfu);
    eliminaMazzoOstacolo(mazzoOstacolo);
    free(listaPersonaggi);
    free(arrayCarteGiocate);
    free(carteCfuPiuAlte);

    return 0;
}
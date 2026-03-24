#include "funzioniUtili.h"

/** La funzione randRange restituisce un numero intero casuale
* compreso tra min e max. Va chiamata la srand() nel main prima
* della sua prima invocazione.
*
* @param min il numero minimo
* @param max il numero massimo
* @return un numero intero casuale compreso tra min e max
**/

int randRangeIntero(int min, int max){
    int random;

    random = min + rand()%(max-min+1);

    return random;
}

/** Funzione che permette di pulire lo schermo dell'output, restituendo una nuova schermata che non mostra le informazioni mostrate precedentemente */
void clearConsole(){
    #ifdef _WIN64 // eseguire codice per Windows
    //    printf("Sono su Windows");
        system("cls");
    #elif __APPLE__ // eseguire codice per piattaforme Apple
    //    printf("Sono su Mac OS");
        system("clear");
    #elif __linux__ // eseguire codice per Linux
    //    printf("Sono su Linux");
        system("clear");
    #endif
}


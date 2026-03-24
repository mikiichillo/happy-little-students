#include "personaggi.h"

/// Pacchetto di funzioni Personaggio

/** Caricamento personaggi da file di testo (numero di personaggi variabile)
 * @param dimensione è un valore che ci servirà successivamente al momento della creazioni dei giocatori, per sapere tra quanti personaggi bisognerà fare la selezione casuale..
 * */
personaggio *caricaPersonaggi(int *dimensione){
    int read;
    personaggio aux;
    personaggio *lista = NULL;
    FILE *fp = NULL;

    fp = fopen(INDIRIZZO_PERSONAGGI, "r");
    if(fp == NULL)
        exit(4);

    lista = (personaggio*) malloc(sizeof(personaggio));
    do{
        read = 0;
        for (int j = 0; j <= ESAME; ++j) {
            read += fscanf(fp, "%d ", &aux.bonus[j]);
        }
        read += fscanf(fp, "%[^\n]s", aux.nome);
        fscanf(fp, "\n");

        if(read==5){
            lista = (personaggio *) realloc(lista, (*dimensione + 1) * sizeof(personaggio));
            lista[*dimensione] = aux;
            (*dimensione)++;
        }
    }while (read == 5);

    fclose(fp);
    return lista;
}

/** Stampa di tutti i personaggi presenti all'interno dell'array personaggi
 *
 * @param listaPersonaggi
 * @param dimensione
 */
void stampaListaPersonaggi(personaggio *listaPersonaggi, int dimensione){
    for (int i = 0; i < dimensione; ++i) {
        stampaPersonaggio(listaPersonaggi[i]);
    }
}

/** Stampa i dati di un singolo personaggio
 *
 * @param personaggio personaggio di cui stampare le informazioni (Nome e bonus/malus)
 */
void stampaPersonaggio(personaggio personaggio){
    printf("Personaggio: %s.\n", personaggio.nome);
    for (int j = 0; j <= ESAME; ++j) {
        switch (j) {
            case STUDIO:
                printf(HGRN "Studio: " RESET);
                break;
            case SOPRAVVIVENZA:
                printf(HBLU "Sopravvivenza: " RESET);
                break;
            case SOCIALE:
                printf(HYEL "Sociale: " RESET);
                break;
            case ESAME:
                printf(HRED "Esame: " RESET);
                break;
            default:
                break;
        }
        printf("[%d];  ", personaggio.bonus[j]);
    }
    printf("\n");
}

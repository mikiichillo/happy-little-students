cmake_minimum_required(VERSION 3.27)
project(Progetto_Finale C)

set(CMAKE_C_STANDARD 99)

add_executable(Progetto_Finale main.c
        librerie/funzioniUtili.c
        librerie/personaggi.c
        librerie/gioco.c
        librerie/gioco.h
        librerie/calcoloPunteggi.c
        librerie/calcoloPunteggi.h
        librerie/salvataggi.c
        librerie/salvataggi.h
        librerie/macro.h
        librerie/carteCfu.c
        librerie/carteCfu.h
        librerie/carteOstacolo.c
        librerie/carteOstacolo.h
        librerie/giocatori.c
        librerie/giocatori.h
        librerie/ANSI-color-codes.h)

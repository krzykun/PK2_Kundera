#include <stdio.h>
#include <stdlib.h>

#include "struktury.h"
#include "komunikacja.h"
#include "obsluga_kolejek.h"

// uzywamy funkcji pobierz_z_kolejki, ktora zmienia zawartosc tab. Ale nas to nie interesuje bo i tak usuwamy zawartosc tab.

//=======================================================================================================
//  zapisywanie_posprzataj()
/*
    Funkcja jest wywolywana gdy nie interesuje nas juz zawartosc przetrzymywana w programie. Dlatego mozemy
    skorzystac z funkcji pobierz_z_kolejki(), pomimo faktu ze wstawi ona nam zmiane do zawartosci.
    Czyscimy najpierw kolejke ponawiania aby nie zepsuc programu.
    Pod koniec funkcji czyscimy tablice zawartosci i zerujemy ilosc przetrzymywanych znakow.
*/
//=======================================================================================================
void zapisywanie_posprzataj(HexByte*** tab, int* ilosc_znakow, Kolejka* kolejka_cofania, Kolejka* kolejka_ponawiania)
{
    Zmiana* do_usuniecia;
    do_usuniecia = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_ponawiania, 2);
    while ( do_usuniecia != 0 )
    {
        free(do_usuniecia);
        do_usuniecia = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_ponawiania, 2);
    }
    do_usuniecia = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_cofania, 2);
    while ( do_usuniecia != 0 )
    {
        free(do_usuniecia);
        do_usuniecia = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_cofania, 2);
    }

    (*tab) = realloc(*tab, 0);
    *ilosc_znakow = 0;
    return;
}

//=======================================================================================================
//  zapisz_plik()
/*
    Wykrywa, czy tablica tab jest pusta. Wywoluje funkcje sprawdzajaca czy mozna otworzyc plik po czym
    otwiera go w tej funkcji i zapisuje do niego zawartosc.
*/
//=======================================================================================================
int zapisz_plik(HexByte** tab, int* ilosc_znakow)
{
    if ( tab == (HexByte**)0 )
    {
        kom_brak_zawartosci();
        return 0;
    }

    char* nazwa_biezacego_pliku = malloc(sizeof(char));
    FILE* biezacy_plik;

    kom_podaj_nazwe_pliku();
    if ( !(otworz_plik(nazwa_biezacego_pliku, biezacy_plik, "w")) )
    {
        kom_nie_mozna_otworzyc_pliku();
        return 0;
    }

    biezacy_plik = fopen(nazwa_biezacego_pliku, "w");
    int i;
    for ( i = 0; i < *ilosc_znakow; i++ )
    {
        fwrite(&(tab[i]->bin_form), sizeof(unsigned char), 1, biezacy_plik);
    }

    fclose(biezacy_plik);
    return 1;
}

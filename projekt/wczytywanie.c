#include <stdio.h>
#include <stdlib.h>

#include "struktury.h"
#include "komunikacja.h"


//=======================================================================================================
//  wczytaj_plik()
/*
    Otwiera plik, odczytuje bajt-po-bajcie plik wejsciowy uzywajac bufora unsigned char.

    !!! REALOKUJE tab PRZY KAZDYM ODCZYCIE
*/
//=======================================================================================================
int wczytaj_plik(HexByte*** tab)
{
    char* nazwa_biezacego_pliku = malloc(sizeof(char));
    FILE* biezacy_plik;

    kom_podaj_nazwe_pliku();
    if ( !(otworz_plik(nazwa_biezacego_pliku, biezacy_plik, "r")) )
    {
        kom_nie_mozna_otworzyc_pliku();
        return 0;
    }

    biezacy_plik = fopen(nazwa_biezacego_pliku, "r");

    //  Deklaracja zmiennej iteracyjnej ilosc_znakow ktora potem powie nam jak duzo wczytalismy
    int ilosc_znakow = 0;

    //  Deklaracja i wyczyszczenie buforu znakowego
    unsigned char* bufor_bitowy = malloc(sizeof(unsigned char));
    memset(bufor_bitowy, 0, sizeof(unsigned char));

    //  wczytaj pierwszy element, robimy to aby miec ladny warunek wykonania nastepnej petli while
    int czy_zostalo_do_wczytania =  fread(bufor_bitowy, sizeof(unsigned char), 1, biezacy_plik);

    //
    //  Petla wczytywania zawartosci pliku do tablicy dynamicznej tab
    //

    while ( czy_zostalo_do_wczytania != 0 )
    {
        ilosc_znakow++;
        (*tab) = (HexByte**)realloc((*tab), ilosc_znakow*sizeof(HexByte*));
        (*tab)[ilosc_znakow-1] = malloc(sizeof(HexByte));
        (*tab)[ilosc_znakow-1]->bin_form = *bufor_bitowy;
        (*tab)[ilosc_znakow-1]->text_form[0] = (char)((*bufor_bitowy) >> 4);
        (*tab)[ilosc_znakow-1]->text_form[1] = (char)((*bufor_bitowy) & 0x0F);

        //  Czyscimy tymczasowy bufor
        memset(bufor_bitowy, 0, sizeof(unsigned char));

        //  Dokonujemy nastepnego odczytania
        czy_zostalo_do_wczytania =  (int) fread(bufor_bitowy, sizeof(unsigned char), 1, biezacy_plik);
    }

    fclose(biezacy_plik);
    return ilosc_znakow;
}

#include "struktury.h"
#include <stdio.h>

//=======================================================================================================
//  wczytaj_plik()
/*

*/
//=======================================================================================================
int wczytaj_plik(HexByte*** tab)
{
    char* nazwa_biezacego_pliku = malloc(sizeof(char));

    //  Komunikacja z uzytkownikiem
    printf("Podaj nazwe pliku do otwarcia:\n>");
    scanf("%s", nazwa_biezacego_pliku);

    FILE* biezacy_plik = fopen(nazwa_biezacego_pliku, "r");
    if (biezacy_plik == NULL)
    {
        printf("Nie udalo sie otworzyc pliku. Sprawdz czy plik istnieje.\n");
        return 0;
    }

    //
    //  Petla wczytywania zawartosci pliku do tablicy dynamicznej tab
    //

    //  Deklaracja zmiennej iteracyjnej ilosc_znakow ktora potem powie nam jak duzo wczytalismy
    int ilosc_znakow = 0;

    //  Deklaracja i wyczyszczenie buforu znakowego
    unsigned char* bufor_bitowy = malloc(sizeof(unsigned char));
    memset(bufor_bitowy, 0, sizeof(unsigned char));

    //  wczytaj pierwszy element, robimy to aby miec ladny warunek wykonania nastepnej petli while
    int czy_zostalo_do_wczytania =  fread(bufor_bitowy, sizeof(unsigned char), 1, biezacy_plik);

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
    return 1;
}

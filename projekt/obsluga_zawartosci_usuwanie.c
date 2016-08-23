#include "struktury.h"
#include "obsluga_zawartosci_szukanie.h"

//=============================================================================================================
//  usun_elementy_z_pozycji
/*
    !!! REALOKACJA tab
*/
//=============================================================================================================
void usun_elementy_z_pozycji(HexByte*** tab, int* dlugosc_tab, int pozycja, int dlugosc_ciagu_usuwanego, Kolejka* kolejka_cofania)
{
    int m = 0;
    int f;
    char* ciag = malloc(sizeof(char));
    *ciag = '\0';
    char* do_dodania = malloc(2*sizeof(char));
    do_dodania[1] = '\0';

    //  Zapisywanie usuwanego ciagu, uzytego potem dla kolejki cofania
    int u;
    for (u = pozycja; u < pozycja + dlugosc_ciagu_usuwanego; u++)
    {
        *do_dodania = (char)(*tab)[u]->bin_form;
        strcat(ciag, do_dodania);
    }

    //  Petle usuwajace znaleziony ciag, aktualizacja dlugosci tablicy.
    while ( m < dlugosc_ciagu_usuwanego )
    {
        f = pozycja;
        free((*tab)[f]);
        for ( f; f < *dlugosc_tab; f++ )
        {
            (*tab)[f] = (*tab)[f+1];
        }
        m++;
        (*dlugosc_tab)--;
        //printf("DEBUG Usunalem element ciagu z miejsca %d.\n", (wystapienie_ciagu + m - 1));
    }

    //  Linia wymagajaca przekazania adresu tablicy tab
    (*tab) = (HexByte**) realloc((*tab), (*dlugosc_tab)*(sizeof(HexByte*)));

    if ( kolejka_cofania )
    {
        dodaj_do_kolejki_cofania(kolejka_cofania, ciag, pozycja, 0);
    }

    return;
}

//=============================================================================================================
//  usun
/*
    Funkcja prostsza od funkcji zamien(). Wywoluje funkcje poszukujaca wystapienia ciagu parametr_1 po czym uzywa funkcji
    usun_ciag() aby ja usunac. Podobnie jak w funkcji znajdz(), wystapienie ciagu jest przekazywane przez indeks i.
    Do funkcji przekazany jest ADRES tab poprzez referencje, tak abysmy mogli miec dostep do tab po realloc().
*/
//=============================================================================================================
void usun(HexByte*** tab, char* parametr_1, int* dlugosc_tab, Kolejka* kolejka_cofania)
{
    HexByte* znalazlem = 0;
    int i = 0;
    int dlugosc_ciagu_usuwanego = strlen(parametr_1);
    int dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_usuwanego;

    znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    while ( znalazlem )
    {
        usun_elementy_z_pozycji(tab, dlugosc_tab, i, dlugosc_ciagu_usuwanego, kolejka_cofania);
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_usuwanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }

    return;
}

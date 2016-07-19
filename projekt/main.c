#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "struktury.h"
#include "komunikacja.h"
#include "wczytywanie.h"
#include "zapisywanie.h"
#include "edytowanie.h"

/*
TRESC ZADANIA
    Napisac szesnastkowy edytor plikow. Program powinien wyswietlac szesnastkowe wartosci kolejnych
    bajtow pliku, umozliwiac ich edycje i zapis, a takze umozliwiac cofanie i ponawianie zmian.
*/

//=============================================================================================================
//  menu_glowne
/*

*/
//=============================================================================================================
int menu_glowne(HexByte*** tab, char* wybor_menu_glowne, char* wybor_menu_poboczne, int* ilosc_znakow)
{
    (*wybor_menu_glowne) = '\0';
    (*wybor_menu_poboczne) = '\0';
    pokaz_menu_glowne();
    wczytaj_opcje(wybor_menu_glowne);
    switch(*wybor_menu_glowne)  // switch obcina nam string do pojhedynczego chara z przodu
    {
        case 'p':
        {
            printf("\nInformacje o programie\n\n");
            return 1;
        }

        case 'w':
        {
            (*ilosc_znakow) = wczytaj_plik(tab);
            return 1;
        }

        case 'e':
        {
            if ( !(*ilosc_znakow) )
            {
                printf("\nDo programu nie wczytano jeszcze zadnej zawartosci.\nAby to zrobic, wybierz opcje 'w' z menu glownego.\n");
                return 1;
            }
            int czy_edytujemy = 1;
            while (czy_edytujemy)
            {
                pokaz_menu_edycji();
                wczytaj_opcje(wybor_menu_poboczne);
                czy_edytujemy = edytuj_zawartosc(tab, ilosc_znakow, wybor_menu_poboczne);
            }
            return 1;
        }

        case 'z':
        {
            zapisz_plik();
            return 1;
        }

        case 'x':
        {
            printf("\nWychodze z programu...\n");
            return 0;
        }

        default:
        {
            printf("\nWprowadzony znak nie posiada przypisanej funkcji.\n");
            return 1;
        }
    }
    return 1;
}

//=============================================================================================================
//  main()
/*

*/
//=============================================================================================================
int main()
{
    //
    //  Inicjalizacja zmiennych oraz glownej petli programu
    //

    int* tryb_pracy = malloc(sizeof(int));
    (*tryb_pracy) = 1;
    char* wybor_menu_glowne = malloc(sizeof(char));
    char* wybor_menu_poboczne = malloc(sizeof(char));
    int* czy_istnieje_zawartosc = malloc(sizeof(int));
    (*czy_istnieje_zawartosc) = 0;
    int* ilosc_znakow = malloc(sizeof(int));
    (*ilosc_znakow) = 0;

    HexByte** tab = (HexByte**) NULL;   // rzutowanie oraz inicjalizacja nie jest konieczna

    while ( *tryb_pracy )
    {
        *tryb_pracy = menu_glowne(&tab, wybor_menu_glowne, wybor_menu_poboczne, ilosc_znakow);
    }
    printf("\nNacisnij dowolny klawisz aby zakonczyc prace z programem...\n");
    getch();

    realloc(tab, 0);
    return 0;
}

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
    Funkcja pokazuje uzytkownikowi opcje i odczytuje wybor ktorego dokona. Potem wywoluje odpowiednie funkcje.
    Jesli podamy symbol ktory nie ma znaczenia w tym menu, menu wyswietli sie ponownie.
    Wystepuje tutaj petla edycyjna dzialajaca podobnie jak glowna petla (zmienna czy_edytujemy)
*/
//=============================================================================================================
int menu_glowne(HexByte*** tab, char* wybor_menu_glowne, char* wybor_menu_poboczne, int* ilosc_znakow, Kolejka* kolejka_cofania, Kolejka* kolejka_ponawiania)
{
    (*wybor_menu_glowne) = '\0';
    (*wybor_menu_poboczne) = '\0';
    wydrukuj_linie(15);
    pokaz_menu_glowne();
    wczytaj_opcje(wybor_menu_glowne);
    switch(*wybor_menu_glowne)  // uzytkownik mogl podac ciag znakow, ale nas interesuje tylko co jest w pierwszym znaku
    {
        case 'p':
        {
            informacje_o_programie();
            poczekaj_na_akcje();
            return 1;
        }

        case 'w':
        {
            if ( ochrona_przed_utrata_zawartosci(tab) )
            {
                (*ilosc_znakow) = wczytaj_plik(tab);
            }
            else
            {
                poczekaj_na_akcje();
            }
            return 1;
        }

        case 'e':
        {
            if ( !(*ilosc_znakow) )
            {
                printf("\nDo programu nie wczytano jeszcze zadnej zawartosci.\nAby to zrobic, wybierz opcje 'w' z menu glownego.\n");
                poczekaj_na_akcje();
                return 1;
            }
            int czy_edytujemy = 1;

            //  Druga petla, edycyjna

            while (czy_edytujemy)
            {
                wydrukuj_linie(9);
                pokaz_menu_edycji();
                wczytaj_opcje(wybor_menu_poboczne);
                czy_edytujemy = edytuj_zawartosc(tab, ilosc_znakow, wybor_menu_poboczne, kolejka_cofania, kolejka_ponawiania);
            }
            return 1;
        }

        case 'z':
        {
            if ( zapisz_plik((*tab), ilosc_znakow) )
            {
                // Dealokacja zawartosci i przetrzymywanych w kolejkach zmian
                zapisywanie_posprzataj(tab, ilosc_znakow, kolejka_cofania, kolejka_ponawiania);
            }
            else
            {
                // Daj uzytkownikowi przeczytac co sie nie udalo
                poczekaj_na_akcje();
            }
            return 1;
        }

        case 'x':
        {
            //return 0;   //skasowac ta linie w release
            if ( ochrona_przed_utrata_zawartosci(tab) )
            {
                printf("\nWychodze z programu...\n");
                return 0;
            }
            else
            {
                return 1;
            }
        }

        default:
        {
            printf("\nWprowadzony znak nie posiada przypisanej funkcji.\n");
            poczekaj_na_akcje();
            return 1;
        }
    }
    return 1;
}

//=============================================================================================================
//  main()
/*
    Funkcja glowna programu. Nastepuje tu alokacja i dealokacja zmiennych ktorych zakres trwania obejmuje
     caly program.
    Funkcja przekazuje kontrole funkcji menu_glowne ktora zwraca wartosc int do zmiennej int* tryb_pracy.
    Tak dlugo, jak wartosc = 1, program dziala. Gdy zostanie ustawiona na 0, program dealokuje zmienne
     i wylacza sie.
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

    Kolejka* kolejka_cofania = malloc(sizeof(Kolejka));
    kolejka_cofania->do_kolejki = (Zmiana*) 0;
    kolejka_cofania->ilosc_zmian = 0;
    Kolejka* kolejka_ponawiania = malloc(sizeof(Kolejka));
    kolejka_ponawiania->do_kolejki = (Zmiana*) 0;
    kolejka_ponawiania->ilosc_zmian = 0;

    HexByte** tab = (HexByte**) NULL;   // rzutowanie nie jest konieczne

    //
    //  Glowna petla programu
    //

    while ( *tryb_pracy )
    {
        *tryb_pracy = menu_glowne(&tab, wybor_menu_glowne, wybor_menu_poboczne, ilosc_znakow, kolejka_cofania, kolejka_ponawiania);
    }
    printf("\nNacisnij dowolny klawisz aby zakonczyc prace z programem...\n");
    getch();

    //
    //  Dealokacja wszystkich zmiennych
    //

    realloc(tryb_pracy, 0);
    realloc(wybor_menu_glowne, 0);
    realloc(wybor_menu_poboczne, 0);
    realloc(czy_istnieje_zawartosc, 0);
    realloc(ilosc_znakow, 0);
    realloc(kolejka_cofania, 0);
    realloc(kolejka_ponawiania, 0);
    return 0;
}

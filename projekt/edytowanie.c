#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "struktury.h"
#include "komunikacja.h"
#include "obsluga_kolejek.h"
#include "obsluga_zawartosci_szukanie.h"
#include "obsluga_zawartosci_usuwanie.h"
#include "obsluga_zawartosci_wstawianie.h"

//=============================================================================================================
//  wyswietl_zawartosc
/*
    Funkcja wyswietla obecna zawartosc przechowywana w tablicy tab.
*/
//=============================================================================================================
void wyswietl_zawartosc(HexByte** tab, int i)
{
    int j;
    printf("\nAktualna zawartosc:\n");
    printf("\n%0X", tab[0]->bin_form);
    for ( j = 1; j < i; j++)
    {
        printf(" %0X", tab[j]->bin_form);
    }
    printf("\n");
    return;
}

//=============================================================================================================
//  edytuj_zawartosc
/*
    Funkcja zajmuje sie edycja zawartosci. Zwraca wartosc 0 gdy uzytkownik chce zaprzestac dalszej edycji.
*/
//=============================================================================================================
int edytuj_zawartosc(HexByte*** tab, int* ilosc_znakow, char* wybor_menu_edycji, Kolejka* kolejka_cofania, Kolejka* kolejka_ponawiania)
{
    int pozycja = 0;
    int ilosc_do_usuniecia = 0;
    char* ciag1 = malloc(sizeof(char));
    char* ciag2 = malloc(sizeof(char));
    switch(*wybor_menu_edycji)
    {
        case '1':
            {
                wyswietl_zawartosc(*tab, *ilosc_znakow);
                poczekaj_na_akcje();
                return 1;
            }
        case '2':
            {
                kom_podaj_poz();
                wczytaj_wartosc(&pozycja);
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                wstaw_ciag_na_pozycje(tab, ilosc_znakow, pozycja, ciag1, strlen(ciag1));
                dodaj_do_kolejki_cofania(kolejka_cofania, ciag1, pozycja, 1);
                poczekaj_na_akcje();
                return 1;
            }
        case '3':
            {
                kom_podaj_poz();
                wczytaj_wartosc(&pozycja);
                kom_podaj_ilosc_do_usun();
                wczytaj_wartosc(&ilosc_do_usuniecia);
                usun_elementy_z_pozycji(tab, ilosc_znakow, pozycja, ilosc_do_usuniecia, kolejka_cofania);
                poczekaj_na_akcje();
                return 1;
            }
        case '4':
            {
                HexByte* znalazlem_na = (HexByte*) 0;
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                znalazlem_na = znajdz_wystapienie(ciag1, *tab, &pozycja, (*ilosc_znakow - strlen(ciag1)), *ilosc_znakow);
                if ( !znalazlem_na )
                {
                    printf("\nPodany ciag nie wystepuje\n");
                    return 1;
                }
                printf("\nPierwsze wystapienie znaleziono na %d pozycji.\n", pozycja);
                poczekaj_na_akcje();
                return 1;
            }
        case '5':
            {
                int ilosc_wystapien = 0;
                HexByte* znalazlem_na = (HexByte*) 0;
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);

                printf("\n");
                znalazlem_na = znajdz_wystapienie(ciag1, *tab, &pozycja, (*ilosc_znakow - strlen(ciag1)), *ilosc_znakow);
                while ( znalazlem_na )
                {
                    ilosc_wystapien++;
                    printf("\nWystapienie %d znaleziono na %d pozycji.\n", ilosc_wystapien, pozycja);
                    pozycja++;
                    znalazlem_na = znajdz_wystapienie(ciag1, *tab, &pozycja, (*ilosc_znakow - strlen(ciag1)), *ilosc_znakow);
                }
                if (ilosc_wystapien == 0)
                {
                    printf("Nie znalazlem wystapien w zawartosci.\n");
                }
                poczekaj_na_akcje();
                return 1;
            }
        case '6':
            {
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                usun(tab, ciag1, ilosc_znakow, kolejka_cofania);
                poczekaj_na_akcje();
                return 1;
            }
        case '7':
            {
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                kom_podaj_ciag();
                wczytaj_opcje(ciag2);
                zamien(tab, ciag1, ciag2, ilosc_znakow, kolejka_cofania);
                poczekaj_na_akcje();
                return 1;
            }
        case '8':
            {
                //  Funkcja pobierz_z_kolejki wprowadza ostatnia zmiane z kolejki do tablicy tab
                Zmiana* wprowadzona_zmiana = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_cofania, 1);

                if ( wprowadzona_zmiana )
                {
                    wprowadzona_zmiana->nast = (struct Zmiana*) kolejka_ponawiania->do_kolejki;
                    kolejka_ponawiania->do_kolejki = wprowadzona_zmiana;
                    kolejka_ponawiania->ilosc_zmian++;
                }
                else
                {
                    brak_zmian_w_kolejce(0);
                    poczekaj_na_akcje();
                }
                return 1;
            }
        case '9':
            {
                //  Funkcja pobierz_z_kolejki wprowadza ostatnia zmiane z kolejki do tablicy tab
                Zmiana* wprowadzona_zmiana = pobierz_z_kolejki(tab, ilosc_znakow, kolejka_ponawiania, 0);
                if ( wprowadzona_zmiana )
                {
                    wprowadzona_zmiana->nast = (struct Zmiana*) kolejka_cofania->do_kolejki;
                    kolejka_cofania->do_kolejki = wprowadzona_zmiana;
                    kolejka_cofania->ilosc_zmian++;
                }
                else
                {
                    brak_zmian_w_kolejce(1);
                    poczekaj_na_akcje();
                }
                return 1;
            }
        case 'x':
            {
                return 0;
            }
        default:
            {
            printf("\nWprowadzony znak nie posiada przypisanej funkcji.\n");
            poczekaj_na_akcje();
            return 0;
            }
    }
}

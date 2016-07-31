#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "struktury.h"

//=============================================================================================================
//  Funkcje komunikacji z uzytkownikiem
/*
    wczytaj_opcje       (char*)
    wczytaj_wartosc     (int*)

    kom_podaj_ciag      ()
    kom_podaj_poz       ()
    kom_podaj_ilosc_do_usun()
    kom_czy_nadpisac_zawartosc()
    kom_podaj_nazwe_pliku()
    kom_brak_zawartosci ()

    informacje_o_programie()
    wydrukuj_linie      (int)
    brak_zmian_w_kolejce(int)
    poczekaj_na_akcje   ()
    pokaz_menu_edycji   ()
    pokaz_menu_glowne   ()
*/
//=============================================================================================================
void wczytaj_opcje(char* wczytaj_do_mnie)
{
    scanf("%s", wczytaj_do_mnie);
    return;
}

//=======================================================================================================s
void wczytaj_wartosc(int* wczytaj_do_mnie)
{
    scanf("%d", wczytaj_do_mnie);
    return;
}

//=======================================================================================================
void kom_podaj_ciag()
{
    printf("Podaj ciag:\n"
           ">");
    return;
}

//=======================================================================================================
void kom_podaj_poz()
{
    printf("Podaj pozycje:\n"
           ">");
    return;
}

//=======================================================================================================
void kom_podaj_ilosc_do_usun()
{
    printf("Podaj ilosc elementow do usuniecia:\n"
           ">");
    return;
}

//=======================================================================================================
void kom_czy_nadpisac_zawartosc()
{
    printf("Program posiada juz wczytana zawartosc. Czy napewno chcesz kontynuowac? (Poprzednia zawartosc zostanie utracona))\n"
           "\tt\ttak\n"
           "\tn\tnie\n"
           ">");
    return;
}

//=======================================================================================================
void kom_podaj_nazwe_pliku()
{
    printf("Podaj nazwe pliku:\n>");
    return;
}

//=======================================================================================================
void kom_brak_zawartosci()
{
    printf("Brak wczytanej zawartosci. Nie moge nic zapisac.\n");
    return;
}

//=======================================================================================================
void kom_nie_mozna_otworzyc_pliku()
{
    printf("Nie udalo sie otworzyc pliku.\n");
    return;
}

//=======================================================================================================
void informacje_o_programie()
{
    printf("\nWitaj w programie edycji plikow szesnastkowych!\n"
           "Program jest zdolny do wczytania wskazanego pojedynczego pliku, edycji jego\n"
           "zawartosci w wartosciach szesnastkowych (np. \"ab0;\" w zapisie szesnastkowym\n"
           " to 61 62 30 3B) oraz zapisu do wybranego pliku.\n"
           "Program daje mozliwosci edycji: wstawiania, usuwania, znajdywania znakow w\n"
           "zawartosci zamiany ciagu oraz cofania i ponawiania zmian.\n"
           "Wszystkie opcje sa opisane.\n"
           );
    return;
}

//=======================================================================================================
void wydrukuj_linie(int tyle)
{
    int i;
    for ( i = 0; i < tyle; i++)
    {
        printf("\n");
    }
    return;
}

//=======================================================================================================
void brak_zmian_w_kolejce(int ktorej)
{
    if ( ktorej )
    {
        printf("Brak zarejestrowanych wycofan zmian.\n");
    }
    else
    {
        printf("Brak zmian do wycofania.\n");
    }
    return;
}

//=======================================================================================================
void poczekaj_na_akcje()
{
    printf("\nNacisnij dowolny klawisz aby kontynuowac...\n");
    getch();
    return;
}

//=======================================================================================================
int otworz_plik(char* nazwa_biezacego_pliku, FILE* biezacy_plik, char* sposob_odczytu)
{
    scanf("%s", nazwa_biezacego_pliku);

    biezacy_plik = fopen(nazwa_biezacego_pliku, sposob_odczytu);
    if (biezacy_plik == NULL)
    {
        printf("Nie udalo sie otworzyc pliku. Sprawdz czy plik istnieje.\n");
        poczekaj_na_akcje();
        return 0;
    }

    return 1;
}

//=======================================================================================================
// ochrona_przed_utrata_zawartosci
/*
    Funkcja zwraca 1 jak uzytkownika nie interesuje poprzednia zawartosc i chce kontynuowac(wczytac
    nowa wartosc albo wylaczyc program)

    !!! FUNKCJA REALOKUJE TABLICE tab JESLI TAKA BEDZIE DECYZJA
*/
//=======================================================================================================
int ochrona_przed_utrata_zawartosci(HexByte*** tab)
{
    if ( *tab )
    {
        char decyzja;
        kom_czy_nadpisac_zawartosc();
        wczytaj_opcje(&decyzja);
        switch(decyzja)
        {
        case 'n':   // Nie chce wczytac nowej wartosci do tablicy tab
            {
                return 0;
            }
        case 't':   //  Spoko, wczytuj nowa wartosc
            {
                *tab = realloc(*tab, 0);
                return 1;
            }
        default:
            {
                printf("\nWprowadzony znak nie posiada przypisanej funkcji.\n");
                poczekaj_na_akcje();
                return 0;
            }
        }
    }
    else return 1;
}

void pokaz_menu_edycji()
{
    printf("\n===EDYCJA PLIKU===\n"
           "\tKomenda\t| Dzialanie\n\n"
           "\t1\tWyswietl obecnie przechowywana zawartosc\n"
           "\t2\tDodaj ciag na pozycje\n"
           "\t3\tUsun ciag od pozycji\n"
           "\t4\tZnajdz pierwsze wystapienie ciagu\n"
           "\t5\tZlicz wszystkie wystapienia ciagu\n"
           "\t6\tZnajdz i usun wszystkie wystapienia ciagu\n"
           "\t7\tZnajdz i zamien wszystkie wystapienia ciagu\n"
           "\t8\tCofnij zmiane w zawartosci\n"
           "\t9\tPonow zmiane w zawartosci\n"
           "\tx\tWyjdz z edycji pliku\n"
           "\n>"
           );
    return;
}

void pokaz_menu_glowne()
{
    printf("\nWitaj w programie szesnastkowej edycji plikow!\nDostepne komendy:\n\n"
           "\tKomenda\t| Dzialanie\n\n"
           "\tp\tWyswietla informacje o programie\n"
           "\tw\tWczytuje plik do programu\n"
           "\te\tModyfikuj zawartosc pliku\n"
           "\tz\tZakoncz prace z biezacym plikiem i zapisz zmiany do pliku\n"
           "\tx\tZakoncz prace z programem\n"
           "\n>"
           );
    return;
}

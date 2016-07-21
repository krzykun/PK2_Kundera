#include <stdio.h>
#include <conio.h>

//=============================================================================================================
//  Funkcje komunikacji z uzytkownikiem
/*
    wczytaj_opcje       (char*)
    wczytaj_wartosc     (int*)
    kom_podaj_ciag      ()
    kom_podaj_poz       ()
    kom_podaj_ilosc_do_usun()
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

void wczytaj_wartosc(int* wczytaj_do_mnie)
{
    scanf("%d", wczytaj_do_mnie);
    return;
}

void kom_podaj_ciag()
{
    printf("Podaj ciag:\n"
           ">");
    return;
}

void kom_podaj_poz()
{
    printf("Podaj pozycje:\n"
           ">");
    return;
}

void kom_podaj_ilosc_do_usun()
{
    printf("Podaj ilosc elementow do usuniecia:\n"
           ">");
    return;
}

void wydrukuj_linie(int tyle)
{
    int i;
    for ( i = 0; i < tyle; i++)
    {
        printf("\n");
    }
    return;
}

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

void poczekaj_na_akcje()
{
    printf("\nNacisnij dowolny klawisz aby kontynuowac...\n");
    getch();
    return;
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

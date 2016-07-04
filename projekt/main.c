#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
TRESC ZADANIA
    Napisac szesnastkowy edytor plikow. Program powinien wyswietlac szesnastkowe wartosci kolejnych
    bajtow pliku, umozliwiac ich edycje i zapis, a takze umozliwiac cofanie i ponawianie zmian.
*/

//=============================================================================================================
//  Uzyta struktura HexByte
/*
    Struktura przeniesiona z laborattorium nr 5.
*/
//=============================================================================================================
typedef struct _HexByte
 {
   char text_form[2]; // reprezntacja bajtu w postaci 16-owej (ascii)
   unsigned char bin_form;// w postaci binarnej
 } HexByte;

//=============================================================================================================
//  Funkcje komunikacji z użytkownikiem
/*
    wczytaj_opcje       (char*)
    pokaz_menu_edycji   ()
    pokaz_menu_glowne   ()
*/
//=============================================================================================================
void wczytaj_opcje(char* wczytaj_do_mnie)
{
    scanf("%s", wczytaj_do_mnie);
    return;
}

void pokaz_menu_edycji()
{
    printf("===EDYCJA PLIKU===\n"
           "\tKomenda\tDzialanie\n"
           "\t1\tWyswietl obecnie przechowywana zawartosc\n"
           "\t2\tUstaw znacznik w ciagu znakow\n"
           "\t3\tDodaj tekst w miejscu znacznika\n"
           "\t4\tUsun ciag znakow od znacznika\n"
           "\t5\tZnajdz wystapienie ciagu\n"
           "\tx\tWyjdz z edycji pliku\n"
           );
    return;
}

void pokaz_menu_glowne()
{
    printf("\nWitaj w programie szesnastkowej edycji plikow!\nDostepne komendy:\n\n"
           "\tKomenda\tDzialanie\n"
           "\tp\tWyswietla informacje o programie\n"
           "\tw\tWczytuje plik do programu\n"
           "\te\tModyfikuj zawartosc pliku\n"
           "\tz\tZakoncz prace z biezacym plikiem i zapisz zmiany do pliku\n"
           "\tx\tZakoncz prace z programem\n>"
           );
    return;
}

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
//=============================================================================================================
//  edytuj_zawartosc
/*
*/
//=============================================================================================================
void edytuj_zawartosc(HexByte*** tab, char* wybor_menu_edycji)
{
    switch(*wybor_menu_edycji)
    {
        case '1':
            {
                return;
                printf("Obecna zawartosc:\n\n");
            }
        default:
            {
            printf("Wprowadzony znak nie posiada przypisanej funkcji.\n"
                    "Nacisnij dowolny przycisk aby kontynuowac...\n");
            getch();
            return;
            }
    }

}
//=============================================================================================================
//  menu_glowne
/*

*/
//=============================================================================================================
int menu_glowne(HexByte*** tab, char* wybor_menu_glowne, char* wybor_menu_poboczne, int* czy_istnieje_zawartosc)
{
    (*wybor_menu_glowne) = '\0';
    (*wybor_menu_poboczne) = '\0';
    pokaz_menu_glowne();
    scanf("%s", wybor_menu_glowne); //%s obcina tekst ktory zostal wprowadzony do pierwszej litery
    switch(*wybor_menu_glowne)
    {
        case 'p':
        {
            printf("\nInformacje o programie\n\n");
            return 1;
        }

        case 'w':
        {
            (*czy_istnieje_zawartosc) = wczytaj_plik(tab);
            return 1;
        }

        case 'e':
        {
            if ( !(*czy_istnieje_zawartosc) )
            {
                printf("\nDo programu nie wczytano jeszcze zadnej zawartosci.\nAby to zrobic, wybierz opcje 'w' z menu glownego.\n");
                return 1;
            }
            pokaz_menu_edycji();
            wczytaj_opcje(wybor_menu_poboczne);
            edytuj_zawartosc(tab, wybor_menu_poboczne);
            return 1;
        }

        case 'z':
        {
            printf("\nZaslepka z\n\n");
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
    int* czy_istnieje_zawartosc = 0;

    HexByte** tab = (HexByte**) NULL;   // rzutowanie oraz inicjalizacja nie jest konieczna

    while ( *tryb_pracy )
    {
        *tryb_pracy = menu_glowne(&tab, wybor_menu_glowne, wybor_menu_poboczne, czy_istnieje_zawartosc);
    }
    printf("\nNacisnij dowolny klawisz aby zakonczyc prace z programem...\n");
    getch();

    realloc(tab, 0);
    return 0;
}

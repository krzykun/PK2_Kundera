#include <stdio.h>
#include <stdlib.h>

/*TRESC ZADANIA
Napisac szesnastkowy edytor plikow. Program powinien wyswietlac szesnastkowe wartosci kolejnych
 bajtow pliku, umozliwiac ich edycje i zapis, a takze umozliwiac cofanie i ponawianie zmian.
*/

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

void wczytaj_plik(FILE* biezacy_plik, char* zawartosc, char* wskaznik_zawartosci)
{
    if (biezacy_plik != NULL)
    {
        printf("Program posiada juz otwarty plik. Zamknij go najpierw, zanim otworzysz nastepny.\n");
        return;
    }
    char* nazwa_biezacego_pliku = malloc(sizeof(char));
    printf("Podaj nazwe pliku do otwarcia:\n>");
    scanf("%s", nazwa_biezacego_pliku);
    biezacy_plik = fopen(nazwa_biezacego_pliku, "r");
    if (biezacy_plik == NULL)
    {
        printf("Nie udalo sie otworzyc pliku. Sprawdz czy plik istnieje.\n");
        return;
    }
    char* bufor_ciagu_znakowego;
    int czy_poprawnie_odczytalem = fscanf(biezacy_plik, "%s", bufor_ciagu_znakowego);
    if (czy_poprawnie_odczytalem != EOF) {
        strcat(zawartosc, bufor_ciagu_znakowego);
        czy_poprawnie_odczytalem = fscanf(biezacy_plik, "%s", bufor_ciagu_znakowego);
    }
    while (czy_poprawnie_odczytalem != EOF) {
        //DEBUG printf("\nPetla %d\ntmp_char:%s\n", ilosc_slow, v);
        strcat(zawartosc, " ");
        strcat(zawartosc, bufor_ciagu_znakowego);
        //memset(bufor_ciagu_znakowego, "", sizeof(bufor_ciagu_znakowego));
        czy_poprawnie_odczytalem = fscanf(biezacy_plik, "%s", bufor_ciagu_znakowego);
    }
    fclose(biezacy_plik);
    wskaznik_zawartosci = zawartosc;
    return;
}

void edytuj_zawartosc(char* zawartosc, char* wybor_menu_edycji)
{
    switch(*wybor_menu_edycji)
    {
        case '1':
            {
                printf("Obecna zawartosc:\n\n%s", zawartosc);
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

int menu_glowne(FILE* biezacy_plik, char* zawartosc, char* wybor_menu_glowne, char* wybor_menu_poboczne, char* wskaznik_zawartosci)
{
    *wybor_menu_glowne = "";
    *wybor_menu_poboczne = "";
    pokaz_menu_glowne();
    scanf("%s", wybor_menu_glowne); //%s obcina tekst ktory zostal wprowadzony do pierwszej litery
    switch(*wybor_menu_glowne)
    {
        case 'p':
        {
            printf("pupa\n");
            return 1;
        }

        case 'w':
        {
            wczytaj_plik(biezacy_plik, zawartosc, wskaznik_zawartosci);
            return 1;
        }

        case 'e':
        {
            pokaz_menu_edycji(zawartosc);
            wczytaj_opcje(wybor_menu_poboczne);
            edytuj_zawartosc(zawartosc, wybor_menu_poboczne);
            return 1;
        }

        case 'z':
        {
            printf("zupa\n");
            return 1;
        }

        case 'x':
        {
            printf("Wychodze z programu...\n");
            return 0;
        }

        default:
        {
            printf("Wprowadzony znak nie posiada przypisanej funkcji.\n"
                    "Nacisnij dowolny przycisk aby kontynuowac...\n");
            getch();
            //system("cls");
            return 1;
        }
    }
    return 1;
}

int main()
{
    int* tryb_pracy = malloc(sizeof(int));
    *tryb_pracy = 1;
    FILE* biezacy_plik = NULL;
    char* zawartosc = malloc(sizeof(char));
    char* wybor_menu_glowne = malloc(sizeof(char));
    char* wybor_menu_poboczne = malloc(sizeof(char));
    char* wskaznik_zawartosci;
    while (*tryb_pracy)
    {
        *tryb_pracy = menu_glowne(biezacy_plik, zawartosc, wybor_menu_glowne, wybor_menu_poboczne, wskaznik_zawartosci);
    }
    printf("Nacisnij dowolny klawisz aby zakonczyc prace z programem...\n");
    getch();
    return 0;
}

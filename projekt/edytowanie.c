#include "struktury.h"
#include "komunikacja.h"

//=============================================================================================================
//  wyswietl_zawartosc
/*
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
//  znajdz_wystapienie
/*
    Funkcja odpowiedzialna za znalezienie ciagu znakow we wczytanym do tablicy tab tekscie.
    Zmienna i jest interpretowana jako offset - wskazuje, od ktorego Hexbyte w tablicy program ma zaczac wyszukiwanie.
    Zwraca null jesli dany ciag nie wstepuje w tekscie.
    Zwraca adres na pierwszy element znalezionego ciagu jesli znajdzie wystapienie w tekscie.
    Jesli znajdzie, to zapisze do parametru i (podanego przez REFERENCJE) miejsce na ktorym znalazla ciag.
    Funkcja NIE zmienia dlugosci tablicy tab ani zmiennej dlugosc_tab.
*/
//=============================================================================================================
HexByte* znajdz_wystapienie(char* parametr_1, HexByte** tab, int* i, int dlugosc_do_szukania, int dlugosc_tab)
{
    //  Warunek brzegowy, nie chcemy sprawdzac smieci spoza zakresu tablicy tab
    if ( dlugosc_do_szukania < *i )
    {
        //printf("DEBUG Wywolanie wykrylo brzeg tablicy.\n");
        return (HexByte*) 0;
    }

    int k = 0;  //  zmienna iterujaca po znalezionym ciagu znakow
    int m = 0;  //  zmienna iterujaca w przod po Hextab

    //  petla wyszukiwania
    while ( ( *i + m ) <= dlugosc_do_szukania )
    {
        while (parametr_1[k] == tab[*i + m + k]->bin_form)
        {
            k++;

            //  warunek wykrywajacy koniec szukanego ciagu znakow - czyli znalezlismy nasz ciag
            if ( parametr_1[k] == '\0' )
            {
                //printf("DEBUG znalazlem wystapienie na %d pozycji.\n", *i+m);

                //  zapisz miejsce na ktorym znalezlismy ciag i zwroc pierwsza wartosc znalezionego ciagu w tab
                *i = *i + m;
                return tab[*i + m];
            }

            //  warunek sprawdzajacy czy tablica nam sie nie skonczyla przy znalezieniu paru pierwszych symboli z par_1
            //  jesli tak, to do widzenia, nie ma w czym juz szukac a ostatniego wystapienia nie ma
            if ( ( *i + m + k ) >= dlugosc_tab)
            {
                //printf("DEBUG Wywolanie wykrylo brzeg tablicy.\n");
                return (HexByte*) 0;
            }
        }

        //  jesli wczesniej znalezlismy pare pierwszych symboli, ale reszta sie nie zgadzala,
        //  to trzeba posprzatac iterator k
        k = 0;

        m++;
    }

    //  Jesli funkcja przejdzie przez cala tablice i nie znajdzie poczatku naszego ciagu to mowi do widzenia
    //  i zwraca NULL
    //printf("DEBUG Nic nie znalazlem.\n");

    return (HexByte*) 0;
}
//=============================================================================================================
//  edytuj_zawartosc
/*
*/
//=============================================================================================================
int edytuj_zawartosc(HexByte*** tab, int* ilosc_znakow, char* wybor_menu_edycji)
{
    int pozycja = 0;
    char* ciag1 = malloc(sizeof(char));
    char* ciag2 = malloc(sizeof(char));
    switch(*wybor_menu_edycji)
    {
        case '1':
            {
                wyswietl_zawartosc(*tab, *ilosc_znakow);
                return 1;
            }
        case '2':
            {
                kom_podaj_poz();
                wczytaj_wartosc(&pozycja);
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
            }
        case '3':
            {
                // Podaj pozycje do usuniecia
                // Podaj ilosc znakow do usuniecia
            }
        case '4':
            {
                HexByte* znalazlem_na;
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                znalazlem_na = znajdz_wystapienie(ciag1, *tab, &pozycja, *ilosc_znakow, *ilosc_znakow);
                if ( !znalazlem_na )
                {
                    printf("\nPodany ciag nie wystepuje\n");
                    return 1;
                }
                printf("\nPierwsze wystapienie znaleziono na %d pozycji.\n", pozycja);
                return 1;
            }
        case '5':
            {
                // zlicz wszystkie wystapienia ciagu
            }
        case '6':
            {
                // znajdz i usun
            }
        case '7':
            {
                // znajdz i zamien
            }
        case 'x':
            {
                return 0;
            }
        default:
            {
            printf("Wprowadzony znak nie posiada przypisanej funkcji.\n"
                    "Nacisnij dowolny przycisk aby kontynuowac...\n");
            getch();
            return 0;
            }
    }

}

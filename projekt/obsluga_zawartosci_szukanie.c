#include "struktury.h"

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

    int k = 0;  //  zmienna iterujaca po znalezionym ciagu znakow, porownanie z parametr_1
    int m = 0;  //  zmienna iterujaca w przod po Hextab, szukanie poczatku ciagu

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

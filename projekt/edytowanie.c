#include "struktury.h"
#include "komunikacja.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

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

void dodaj_do_kolejki_cofania(Kolejka* kolejka_cofania, char* ciag, int pozycja, int typ_akcji)
{
    Zmiana* nowa_zmiana = malloc(sizeof(Zmiana));

    strcpy(nowa_zmiana->ciag, ciag);
    nowa_zmiana->pozycja = pozycja;
    nowa_zmiana->typ_akcji = typ_akcji;
    nowa_zmiana->nast = kolejka_cofania->do_kolejki;

    kolejka_cofania->do_kolejki = nowa_zmiana;
    return;
}

//=============================================================================================================
//  wstaw_ciag_na_pozycje
/*
*/
//=============================================================================================================
void wstaw_ciag_na_pozycje(HexByte*** tab, int* dlugosc_tab, int tutaj, char* ciag, int dlugosc_ciagu_wstawianego)
{
    int m = 0;
    int f;
    while ( dlugosc_ciagu_wstawianego > m )
    {
        //printf("DEBUG Wstawiam nowy element tablicy.\n");

        //  Zwiekszamy zmienna okreslaja rozmiar tablicy (dlugosc ta jest rowna ilosci elementow)
        (*dlugosc_tab)++;

        //  Realokuj tablice tab zwiekszajac jej miejsce o jeden element. Tylko z powodu tej linii przekazujemy
        //  adres tab przez parametry.
        (*tab) = (HexByte**)realloc((*tab), ((*dlugosc_tab)*sizeof(HexByte*)));

        //  Przesun wszystkie elementy o jeden w prawo, aby zrobic miejsce dla nowego, pochodzacego z ciagu wstawianego
        for ( f = ((*dlugosc_tab) - 1); f > tutaj + m; f-- )
        {
            (*tab)[f] = (*tab)[f - 1];
        }

        //  Alokacja nowego elementu
        (*tab)[tutaj + m] = malloc(sizeof(HexByte));
        (*tab)[tutaj + m]->bin_form = (unsigned char)ciag[m];
        (*tab)[tutaj + m]->text_form[0] = (ciag[m] >> 4);
        (*tab)[tutaj + m]->text_form[1] = (ciag[m] & 0x0F);
        m++;
    }
    return;
}

//=============================================================================================================
//  wstaw_ciag
/*
    Funkcja odpowiedzialna za wstawienie ciagu znakow do tablicy tab oraz zmiane jej wielkosci jesli to konieczne.
    Funkcja alokuje pamiec dla nowych elementow tablicy tab, jesli to konieczne.
*/
//=============================================================================================================
void wstaw_ciag(char* ciag_do_wstawienia, int tutaj, HexByte*** tab, int dlugosc_ciagu_szukanego, int dlugosc_ciagu_wstawianego, int* dlugosc_tab)
{
    int zostalo_miejsce = 0;    //zmienna czyszczona gdy nie ma juz co wstawiac, a miejsce jeszcze jest
    int m = 0;  //iterator po ciagu do wstawienia, wskazuje rowniez ile symoli zostalo juz wstawionych
    int f = 0;  //iterator po tab, sluzy do przesuwania elementow tablicy w przypadku tworzenia nowego wewnatrz

    //
    //  Pierwsza petla wstawiania. Wykonywana poprzez bezposrednie podmienienie wartosci ciagu wstawianego i tablicy
    //

    while ( ( dlugosc_ciagu_szukanego > m ) && (!zostalo_miejsce) )
    {
        (*tab)[tutaj + m]->bin_form = (unsigned char)ciag_do_wstawienia[m];
        (*tab)[tutaj + m]->text_form[0] = (ciag_do_wstawienia[m] >> 4);
        (*tab)[tutaj + m]->text_form[1] = (ciag_do_wstawienia[m] & 0x0F);
        m++;
        if ( dlugosc_ciagu_wstawianego == m )
        {
            if ( dlugosc_ciagu_szukanego != m)
            {
                //  Jesli wstawilismy juz caly ciag (pierwszy if) a ciag do ktorego wstawiamy jest wiekszy (drugi if)
                //  to zapisz ten fakt na pozniej
                zostalo_miejsce = 1;
            }
        }
    }

    //
    //  Druga petla wstawiania. Wykonywana, gdy ciag wstawiany okazal sie mniejszy ciagu ktorego szukalismy. W takiej sytuacji,
    //  pozostale miejsca wyszukanego ciagu zeruje
    //

    if ( zostalo_miejsce )
    {
        while ( dlugosc_ciagu_szukanego > m )
        {
            //printf("DEBUG Ciag wstawiany jest krotszy od szukanego.\n");
            (*tab)[tutaj + m]->bin_form = (unsigned char)(0);
            (*tab)[tutaj + m]->text_form[0] = 0;
            (*tab)[tutaj + m]->text_form[1] = 0;
            m++;
        }
    }

    //
    //  Trzecia petla wstawiania. Wykonywana jesli ilosc juz istniejacego miejsca w tablicy okazuje sie
    //  za mala dla wstawianego ciagu,
    //

    while ( dlugosc_ciagu_wstawianego > m )
    {
        //printf("DEBUG Wstawiam nowy element tablicy.\n");

        //  Zwiekszamy zmienna okreslaja rozmiar tablicy (dlugosc ta jest rowna ilosci elementow)
        (*dlugosc_tab)++;

        //  Realokuj tablice tab zwiekszajac jej miejsce o jeden element. Tylko z powodu tej linii przekazujemy
        //  adres tab przez parametry.
        (*tab) = (HexByte**)realloc((*tab), ((*dlugosc_tab)*sizeof(HexByte*)));

        //  Przesun wszystkie elementy o jeden w prawo, aby zrobic miejsce dla nowego, pochodzacego z ciagu wstawianego
        for ( f = ((*dlugosc_tab) - 1); f > tutaj + m; f-- )
        {
            (*tab)[f] = (*tab)[f - 1];
        }

        //  Alokacja nowego elementu
        (*tab)[tutaj + m] = malloc(sizeof(HexByte));
        (*tab)[tutaj + m]->bin_form = (unsigned char)ciag_do_wstawienia[m];
        (*tab)[tutaj + m]->text_form[0] = (ciag_do_wstawienia[m] >> 4);
        (*tab)[tutaj + m]->text_form[1] = (ciag_do_wstawienia[m] & 0x0F);
        m++;
    }
    return;
}

//=============================================================================================================
//  zamien()
/*
    Funkcja wywolujaca funkcje wyszukujaca wystapienie tekstu, oraz wstawiajaca tekst podany jako parametr_1.
    Do funkcji przekazany jest ADRES tab poprzez referencje, tak abysmy mogli miec dostep do tab po realloc().
    Funkcja oblicza i podaje przez parametry dlugosci lancuchow znakowych parametrow, aby kazda funkcja nie musiala
    robic tego sama.
    Wartosc zmiennej znalazlem jest uzyta tylko po to aby wykryc czy funkcja znajdz_wystapienie cos znalazla.
    Mimo to, wartosc tej zmiennej jest wskaznikiem na pierwsze znalezione wystapienie i mozna jej uzyc w ten sposob.
    Nie uzywamy jej tak, bo operujemy na ciagach znakow, nie pojedynczych znakach. Dlatego bardziej przydatny jest
    indeks i miejsca tablicy tab na ktorym znaleziono szukany ciag parametr_1.
*/
//=============================================================================================================
int zamien(HexByte*** tab, char* parametr_1, char* parametr_2, int* dlugosc_tab, Kolejka* kolejka_cofania)
{
    HexByte* znalazlem = 0;
    int i = 0;
    int dlugosc_ciagu_szukanego = strlen(parametr_1);
    int dlugosc_ciagu_wstawianego = strlen(parametr_2);
    int dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_szukanego;

    znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    while ( znalazlem )
    {
        dodaj_do_kolejki_cofania(kolejka_cofania, parametr_1, i, 1);
        wstaw_ciag(parametr_2, i, tab, dlugosc_ciagu_szukanego, dlugosc_ciagu_wstawianego, dlugosc_tab);
        dodaj_do_kolejki_cofania(kolejka_cofania, parametr_2, i, 0);
        //  przesuniecie indeksu i na nastepna pozycje gwarantuje, ze nie znajdziemy tego samego wystapienia dwa razy
        i++;
        //  aktualizuj dlugosc do przeszukiwania, aby nie szukac poza tablica
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_szukanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }
    return 0;
}

//=============================================================================================================
//  usun_elementy_z_pozycji
/*
*/
//=============================================================================================================
void usun_elementy_z_pozycji(HexByte*** tab, int* dlugosc_tab, int pozycja, int dlugosc_ciagu_usuwanego)
{
    int m = 0;
    int f;

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
        usun_elementy_z_pozycji(tab, dlugosc_tab, i, dlugosc_ciagu_usuwanego);
        dodaj_do_kolejki_cofania(kolejka_cofania, parametr_1, i, 1);
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_usuwanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }

    return;
}

int dodaj_do_kolejki_ponawiania()
{

}

//=============================================================================================================
//  edytuj_zawartosc
/*
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
                return 1;
            }
        case '2':
            {
                kom_podaj_poz();
                wczytaj_wartosc(&pozycja);
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                wstaw_ciag_na_pozycje(tab, ilosc_znakow, pozycja, ciag1, strlen(ciag1));
                dodaj_do_kolejki_cofania(kolejka_cofania, ciag1, pozycja, 0);
                return 1;
            }
        case '3':
            {
                kom_podaj_poz();
                wczytaj_wartosc(&pozycja);
                kom_podaj_ilosc_do_usun();
                wczytaj_wartosc(&ilosc_do_usuniecia);
                usun_elementy_z_pozycji(tab, ilosc_znakow, pozycja, ilosc_do_usuniecia);
                dodaj_do_kolejki_cofania(kolejka_cofania, , pozycja, 1)
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
                return 1;
            }
        case '6':
            {
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                usun(tab, ciag1, ilosc_znakow, kolejka_cofania);
                return 1;
            }
        case '7':
            {
                kom_podaj_ciag();
                wczytaj_opcje(ciag1);
                kom_podaj_ciag();
                wczytaj_opcje(ciag2);
                zamien(tab, ciag1, ciag2, ilosc_znakow, kolejka_cofania);
                return 1;
            }
        case '8':
            {
                //dodaj najnowsza operacje z kolejki cofania
                //dodaj ta akcje do kolejki ponawiania
            }
        case '9':
            {
                //dodaj najnowsza operacje z kolejki ponawiania
                //dodaj ta akcje do kolejki cofania
            }
        case 'x':
            {
                return 0;
            }
        default:
            {
            printf("\nWprowadzony znak nie posiada przypisanej funkcji.\n");
            return 0;
            }
    }

}

#include "struktury.h"
#include "obsluga_zawartosci_szukanie.h"

//=============================================================================================================
//  wstaw_ciag_na_pozycje
/*
    Wstawia ciag na podana pozycje. Kod skopiowany z funkcji wstaw ciag.

    !!! REALOKACJA tab
*/
//=============================================================================================================
void wstaw_ciag_na_pozycje(HexByte*** tab, int* dlugosc_tab, int tutaj, char* ciag, int dlugosc_ciagu_wstawianego)
{
    int m = 0;
    int f;
    while ( dlugosc_ciagu_wstawianego > m )
    {
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
//  zamien_ciagi
/*
    Funkcja odpowiedzialna za wstawienie ciagu znakow do tablicy tab oraz zmiane jej wielkosci jesli to konieczne.
    Funkcja alokuje pamiec dla nowych elementow tablicy tab, jesli to konieczne.

    !!! REALOKACJA tab
*/
//=============================================================================================================
void zamien_ciagi(char* ciag_do_wstawienia, int tutaj, HexByte*** tab, int dlugosc_ciagu_szukanego, int dlugosc_ciagu_wstawianego, int* dlugosc_tab)
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
        dodaj_do_kolejki_cofania(kolejka_cofania, parametr_1, i, 0);
        zamien_ciagi(parametr_2, i, tab, dlugosc_ciagu_szukanego, dlugosc_ciagu_wstawianego, dlugosc_tab);
        dodaj_do_kolejki_cofania(kolejka_cofania, parametr_2, i, 1);
        //  przesuniecie indeksu i na nastepna pozycje gwarantuje, ze nie znajdziemy tego samego wystapienia dwa razy
        i++;
        //  aktualizuj dlugosc do przeszukiwania, aby nie szukac poza tablica
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_szukanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }
    return 0;
}

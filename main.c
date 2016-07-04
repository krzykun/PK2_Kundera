#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Zadanie 1 (unie)
Dana jest unia:
typedef union Rejestr_
{
unsigned char bajty[4];
unsigned short int slowa[2]; //16 bitowe
unsigned int rejestr;//32 bitowy rejestr

} Rejestr
Napisac funkcje implementujace:
1) Przesuniecie cykliczne w lewo o 16 bitow;
2) Przesuniecie cykliczne w lewo o 8 bitow;
3) Przesuniecie cykliczne w prawo o 8 bitow;
4) Przesuniecie w prawo o 1 bit;
5) Przesuniecie w lewo o 1 bit;
6) Wyswietla stan rejestru jako liczbe hex;
7) Wyswietla najmlodzy bajt rejestru jako liczbe hex;
8) Wyswietla starsze slowo rejestru jako liczbe hex;
Wszystkie te funkcje przyjmuja jako parametr wskaznik do Rejestr i nic nie zwracaja
Przetestowac wszystkie funkcje dla liczb szesnastkowych;

Zadanie 2 (struktury)
Dana jest struktura;
typedef Slownik_;
{
int ls; //liczba wszystkich unikalnych slow
int h; // wysokosc drzewa - liczba poziomow
Slowo *korzen;
Slowo *mf; // wskaznik na slowo najczesciej wystepujace
Slowo **t; // realokowalna tablica wskazników do wszystkich nie powtarzajacych sie slów w drzewie (realokacja co K=1000 elementow)
// pomocna przy sortowaniu (przestawiamy wskazniki) - funkcja nr 2 (dalsza czesc zadania)

} Slownik;
typedef Slowo;
{
int lw; //liczba wystapien slowa
char *slowo;// slowo
Slowo *prawy; //prawy i lewy potomek
Slowo *lewy; //
} Slownik;
1) Napisac funkcje ktora tworzy slownik na podstwie pliku tekstowego.
(rozmiar minimum 300KB, np. ksiazka z projektu Gutenberg) -
funkcja powinna korzystac z pomocniczej funkcji:
Slowo * wstaw(Slownik *s,char *slowo);
Funkcja zwraca adres nowo wstawionego slowa do drzewa, badz NULL gdy slowo jest juz w drzewie.
Adres rózny od NULL wstawiamy do tablicy t.
2) Napisac funkcje która zapisuje do pliku wszystkie slowa od najczesciej do najrzadziej wystepujacych
(skorzystac z tablicy t).
3) Napisac funkcje która zwraca iloraz wysokosci drzewa oraz wysokosci tego drzewa
w przypadku gdyby bylo ono idealnie zrownowazone.
*/

//zadanie 1

//==============================================================================================
//  Uzyta struktura
/*
*/
//==============================================================================================
typedef union Rejestr_
{
    unsigned char bajty[4];         //4 bajty (4*8 = 32 bity)
    unsigned short int slowa[2];    //16 bitowe
    unsigned int rejestr;           //32 bitowy rejestr
} Rejestr;

//==============================================================================================
//  Operacje na rejestrze
/*
    Deklaracja kazdej z funkcji: void nazwa(Rejestr* obj_reg);
*/
//==============================================================================================
//1) Przesuniecie cykliczne w lewo o 16 bitow;
void przesun_cyklicznie_w_lewo_16bit(Rejestr* obj_reg)
{
    unsigned int temp = obj_reg->slowa[1];
    obj_reg->slowa[1] = obj_reg->slowa[0];
    obj_reg->slowa[0] = temp;
    return;
}
//==============================================================================================
//2) Przesuniecie cykliczne w lewo o 8 bitow;
void przesun_cyklicznie_w_lewo_8bit(Rejestr* obj_reg)
{
    unsigned char temp = obj_reg->bajty[3];
    obj_reg->rejestr = obj_reg->rejestr << 8;
    obj_reg->bajty[0] = temp;
    return;
}
//==============================================================================================
//3) Przesuniecie cykliczne w prawo o 8 bitow;
void przesun_cyklicznie_w_prawo_8bit(Rejestr* obj_reg)
{
    unsigned char temp = obj_reg->bajty[0];
    obj_reg->rejestr = obj_reg->rejestr >> 8;
    obj_reg->bajty[3] = temp;
    return;
}
//==============================================================================================
//4) Przesuniecie w prawo o 1 bit;
void przesun_w_prawo_1bit(Rejestr* obj_reg)
{
    obj_reg->rejestr = obj_reg->rejestr >> 1;
    obj_reg->bajty[3] = obj_reg->bajty[3] & 0x7F;
    return;
}
//==============================================================================================
//5) Przesuniecie w lewo o 1 bit;
void przesun_w_lewo_1bit(Rejestr* obj_reg)
{
    obj_reg->rejestr = obj_reg->rejestr << 1;
    obj_reg->bajty[0] = obj_reg->bajty[0] & 0xFE;
    return;
}
//==============================================================================================
//6) Wyswietla stan rejestru jako liczbe hex;
void wyswietl_rejestr(Rejestr* obj_reg)
{
    printf("\nStan rejestru:\t\t0x %08X\n", obj_reg->rejestr);
    return;
}
//==============================================================================================
//7) Wyswietla najmlodzy bajt rejestru jako liczbe hex;
void wyswietl_najmlodszy_bajt(Rejestr* obj_reg)
{
    printf("\nNajmlodszy bajt:\t0x %02X\n", obj_reg->bajty[0]);
    return;
}
//==============================================================================================
//8) Wyswietla starsze slowo rejestru jako liczbe hex;
void wyswietl_starsze_slowo(Rejestr* obj_reg)
{
    printf("\nStarsze slowo:\t\t0x %04X\n", obj_reg->slowa[1]);
    return;
}


//Zadanie 2 (struktury)


//==============================================================================================
//  Uzyte struktury Slowo i Slownik
/*
*/
//==============================================================================================
typedef struct Slowo_
{
    int lw;         //liczba wystapien slowa
    char *slowo;    // slowo
    struct Slowo *prawy;   //prawy i lewy potomek
    struct Slowo *lewy;
} Slowo;

typedef struct Slownik_
{
    int ls;         //liczba wszystkich unikaTOWYCH slow
    int h;          //wysokosc drzewa - liczba poziomow
    Slowo *korzen;
    Slowo *mf;      //wskaznik na slowo najczesciej wystepujace
    Slowo **t;      //realokowalna tablica wskaznikow do wszystkich nie powtarzajacych
                    // sie slow w drzewie (realokacja co K=1000 elementow) pomocna przy
                    // sortowaniu(przestawiamy wskazniki) - funkcja nr 2 (dalsza czesc zadania)
} Slownik;

//==============================================================================================
//  wstaw()
/*
    1) Napisac funkcje ktora tworzy slownik na podstawie pliku tekstowego.
    (rozmiar minimum 300KB, np. ksiazka z projektu Gutenberg) -
    funkcja powinna korzystac z pomocniczej funkcji:
*/
//==============================================================================================
Slowo* wstaw(Slownik* s, char* slowo)
{
    //
    //  Sprawdzenie, czy slowo juz istnieje w drzewie, szukajac po liscie t
    //

    int i = 0;
    while (i < s->ls)
    {
        if ( strcmp(slowo, s->t[i]->slowo ) == 0 )
        {
            //  Rejestrujemy kolejne wystapienie tego slowa
            s->t[i]->lw += 1;
            //printf("DEBUG Wystapienie dodane: %s\nCzestosc: %d\n", s->t[i]->slowo, s->t[i]->lw);
            //  Poniewaz element juz istnieje, to zwracamy nulla
            return 0;
        }
        i++;
    }
    //  pelne zakonczenie powyzszej petli oznacza ze slowo nie istnieje jeszcze w drzewie

    //  Inkrementujemy wiec licznik slow w drzewie
    s->ls++;
    //  Tworzymy nowe slowo i rezerwujemy pamiec
    Slowo* nowe_slowo = malloc(sizeof(slowo));
    nowe_slowo->slowo = malloc(50*sizeof(char));
    //  Na wskazniki, wstawiamy ciag znakow
    strcpy(nowe_slowo->slowo, slowo);
    nowe_slowo->lw = 1;
    nowe_slowo->lewy = NULL;
    nowe_slowo->prawy = NULL;

    if (s->korzen->prawy != NULL)
    {
        if (s->korzen->lewy != NULL)
        {
            //  Jesli oba liscie obecnego wierzcholka sa puste, tworzymy nowy wierzcholek
            if ( strcmp(slowo, s->korzen->slowo) < 0 )
            {
                //  Jesli poprzedni korzen ma wartosc wieksza w porzadku leksykograficznym,
                //  to bedzie prawym lisciem nowego elementu
                nowe_slowo->prawy = s->korzen;
                s->korzen = nowe_slowo; //ustanawiamy nowy korzen
            }
            else
            {
                //  Jezeli wartosc korzenia jest mniejsza leksykograficznie, bedzie lewym lisciem
                nowe_slowo->lewy = s->korzen;
                s->korzen = nowe_slowo; //ustanawiamy nowy korzen
            }
            s->h++;
        }
        //  Jesli lewy lisc poprzedniego korzenia jest wolny, to tam wstawiamy nowe slowo
        else s->korzen->lewy = nowe_slowo;
    }
    //  Ale wczesniej sprawdzamy, czy prawy jest wolny i tam wstawiamy jesli mozna
    else s->korzen->prawy = nowe_slowo;
    //printf("DEBUG Tekst dodany do drzewa: %s\nCzestosc: %d\n", nowe_slowo->slowo, nowe_slowo->lw);

    //  Zwracamy wskaznik na nowo stworzony element
    return nowe_slowo;
}

//==============================================================================================
//  Zaladuj_tekst
/*
    Odpowiada za zarzadzanie ladowaniem tekstu. Zmienna odczytany tekst przechowuje CALY odczytany z pliku
    tekst. Doklejamy do niej nowe slowo czytane przez bufor tekstu char* poprzez funkcje strcat().
    Zmienna relatywna_dlugosc_tekstu sluzy do testowania, czy potrzebna jest realokacja zmiennej odczytany_tekst.
    Funkcja wstaw() zwraca adres nowo wstawionego slowa do drzewa, badz NULL gdy slowo jest juz w drzewie.
    Adres rozny od NULL wstawiamy do tablicy t.
    Wolne_miejsce_tab_t wskazuje na nastepne wolne miejsce w tablicy t.
*/
//==============================================================================================
void Zaladuj_tekst(Slownik* s, int* wolne_miejsce_tab_t)
{
    //
    //  Pierwsza czesc, odczytanie tekstu z pliku
    //

    //  alokacja pamieci dla nazwy pliku
    char* nazwa_pliku = malloc(100*sizeof(char));

    printf("Podaj nazwe pliku do odczytu:\n");
    scanf("%s", nazwa_pliku);

    //
    //  otworz plik, sprawdz czy sie otwarl
    //

    FILE* otwarty_plik;
    otwarty_plik = fopen(nazwa_pliku, "r");
    if (!otwarty_plik)
    {
        printf("Nie udalo sie otworzyc pliku.\n");
        return;
    }

    //
    //  wczytaj tekst z pliku do odczytany_tekst ktory potem bedziemy rozbijac przez strtok()
    //

    // Alokacja pamieci dla bufora oraz odczytanego tekstu
    char* bufor_tekstu = malloc(100*sizeof(char));
    char* odczytany_tekst = malloc(1000000*sizeof(char));

    //  Wyczysc zmienne trzymajace tekst przed uzyciem
    memset(odczytany_tekst, "", 1000000*sizeof(char));
    strcpy(odczytany_tekst, "");

    int ilosc_slow;
    int czy_poprawnie_odczytalem;
    long int relatywna_dlugosc_tekstu = 0;

    //  pierwszy odczyt z pliku
    czy_poprawnie_odczytalem = fscanf(otwarty_plik, "%s", bufor_tekstu);

    //  petla czytania z pliku
    while ( czy_poprawnie_odczytalem != EOF )
    {
        if ( relatywna_dlugosc_tekstu > 900000 )
        {
            //  realokacja wykorzystywanej pamieci odczytanego tekstu jesli ma przekroczyc pewna wartosc
            odczytany_tekst = realloc(odczytany_tekst, strlen(odczytany_tekst) + 1000000);
            relatywna_dlugosc_tekstu = 0;
        }

        //  mozna dodac odczytany tekst z bufora
        strcat(odczytany_tekst, bufor_tekstu);
        strcat(odczytany_tekst, " ");
        ilosc_slow++;
        relatywna_dlugosc_tekstu += 1 + strlen(bufor_tekstu);

        //  czyszczenie bufora
        strcpy(bufor_tekstu, "");
        //  odczytanie nastepnego slowa
        czy_poprawnie_odczytalem = fscanf(otwarty_plik, "%s", bufor_tekstu);
    }
    fclose(otwarty_plik);

    //printf("DEBUG odczytany tekst: %s\n", odczytany_tekst);

    //
    //  Druga czesc, tworzenie slownika
    //

    //  Stworz pierwsza strukture Slowo, zawiera ona jedno slowo i nulle, alokacja pamieci
    Slowo* nowe_slowo = malloc(sizeof(Slowo));
    nowe_slowo->slowo = malloc(50*sizeof(char));
    nowe_slowo->prawy = NULL;
    nowe_slowo->lewy = NULL;
    nowe_slowo->lw = 1;

    //  Odczytujemy pierwsze slowo z odczytanego tekstu
    char* tresc_nowego_slowa = strtok(odczytany_tekst, " .,-:;()#[]/!?\"\\");
    //  I wsadzamy je do pierwszego Slowa
    strcpy(nowe_slowo->slowo, tresc_nowego_slowa);

    //printf("DEBUG Tekst dodany do drzewa: %s\nCzestosc: %d\n", nowe_slowo->slowo, nowe_slowo->lw);
    printf("\nDEBUG %d\n\n", ilosc_slow);

    //  Ustawiam poczatkowe wartosci slownika
    int licznik_realokacji = 1;
    s->t = malloc(1000*sizeof(Slowo*));
    s->korzen = nowe_slowo;
    s->t[0] = nowe_slowo;
    s->mf = nowe_slowo;
    s->h = 1;
    s->ls = 1;

    //  Petla wczytywania slow do struktury slownika
    Slowo* czy_stworzono_nowe;
    int i = 1;
    while ( i < ilosc_slow)
    {
        //  Odczytujemy nastepne Slowo
        tresc_nowego_slowa = strtok(NULL, " .,-:;()#[]/!?\"\\");
        czy_stworzono_nowe = wstaw(s, tresc_nowego_slowa);
        if ( czy_stworzono_nowe )
        {
            s->t[*wolne_miejsce_tab_t] = czy_stworzono_nowe;
            (*wolne_miejsce_tab_t)++;
            licznik_realokacji++;

            //  Realokacja pamieci jesli przekroczylismy pewna wartosc tablicy t
            if (licznik_realokacji == 1000)
            {
                s->t = realloc(s->t, ((1000 + (*wolne_miejsce_tab_t))*sizeof(Slowo*)));
                licznik_realokacji = 0;
            }
        }
        i++;
    }
    return;
}

//==============================================================================================
//  porownaj_czestosci()
/*
    Funkcja pomocnicza uzyta do porownania dla funkcji qsort().
    Uzywajac tej funkcji qsort sortowac bedzie nierosnaco.
*/
//==============================================================================================
int porownaj_czestosci(const void* param1, const void* param2)
{
    Slowo* _param1 = *(Slowo**)param1;
    Slowo* _param2 = *(Slowo**)param2;
    if ( _param1->lw > _param2->lw ) return -1;
    if ( _param1->lw < _param2->lw ) return 1;
    if ( _param1->lw = _param2->lw ) return 0;
}

//==============================================================================================
//  zapisz()
/*
    2) Napisac funkcje która zapisuje do pliku wszystkie slowa od najczesciej do najrzadziej wystepujacych
    (skorzystac z tablicy t).

    Funkcja odpowiada za sortowanie poprzez funkcje qsort z biblioteki standardowej.
    Gdy Slownik jest posortowany, program pyta uzytkownika o nazwe pliku do jakiego ma zapisac
    informacje o czestosci wystapien. Funkcja wykrywa bledne otwarcie pliku, po czym zapisuje informacje
    o czestosci wystapien do pliku.
*/
//==============================================================================================
void zapisz(Slownik* s, int wolne_miejsce_od_tab_t)
{
    //
    //  Sortuje slownik
    //

    if ( wolne_miejsce_od_tab_t == 0 )
    {
        printf("Nie ma co sortowac.\n");
        return;
    }
    qsort(s->t, wolne_miejsce_od_tab_t, sizeof(Slowo*), porownaj_czestosci);

    //
    //  Do jakiego pliku mam zapisac?
    //

    char* nazwa_pliku = malloc(20*sizeof(char));
    printf("\nPodaj nazwe pliku do zapisu czestosci wystapien slow:\n");
    scanf("%s", nazwa_pliku);

    //
    //  Operacje na pliku
    //

    FILE * otwarty_plik;
    otwarty_plik = fopen(nazwa_pliku, "w");
    if ( otwarty_plik == 0 )
    {
        printf("Bledna nazwa pliku,\n");
        return;
    }
    fprintf(otwarty_plik, "Posortowane wyrazy Slownika\n\n");
    int i;
    for ( i = 0; i < wolne_miejsce_od_tab_t; i++ ) // <= w warunku poniewaz wczesniej odjelismy 1
    {
        fprintf(otwarty_plik, "Slowo: %s\tCzestosc: %d\n", s->t[i]->slowo, s->t[i]->lw);
    }
    return;
}

//==============================================================================================
//  oblicz()
/*
    3) Napisac funkcje ktora zwraca iloraz wysokosci drzewa oraz wysokosci tego drzewa
    w przypadku gdyby bylo ono idealnie zrownowazone.
*/
//==============================================================================================
double oblicz(Slownik* s)
{
    //drzewo BST idealnie zrownowazone ma log2(n) poziomow
    // licze dla drzewa BST poniewaz utworzone przez program drzewo jest drzewem binarnym
    // (ale nie bst poniewaz nie ma wszystkich wlasciwosci drzewa bst)
    double wysokosc_idealnego = 0.0;
    wysokosc_idealnego = log2(s->ls);
    double wynik = ( s->h / wysokosc_idealnego);
    return wynik;
}

//==============================================================================================
//
//  main()
//
//==============================================================================================
int main()
{
    //
    //  Zadanie 1
    //

    Rejestr* ten_rejestr = malloc(sizeof(Rejestr));
    printf("Podaj wartosc ktora program ma wpisac do rejestru:\n");
    unsigned int bufor_rejestru = 0;
    scanf("%x", &bufor_rejestru);
    ten_rejestr->rejestr = bufor_rejestru;

    wyswietl_rejestr(ten_rejestr);
    wyswietl_starsze_slowo(ten_rejestr);
    wyswietl_najmlodszy_bajt(ten_rejestr);
    printf("\nWykonuje przesuniecie cykliczne w lewo o 16 bitow");
    przesun_cyklicznie_w_lewo_16bit(ten_rejestr);
    wyswietl_rejestr(ten_rejestr);
    printf("\nWykonuje przesuniecie cykliczne w lewo o 8 bitow");
    przesun_cyklicznie_w_lewo_8bit(ten_rejestr);
    wyswietl_rejestr(ten_rejestr);
    printf("\nWykonuje przesuniecie cykliczne w prawo o 8 bitow");
    przesun_cyklicznie_w_prawo_8bit(ten_rejestr);
    wyswietl_rejestr(ten_rejestr);
    printf("\nWykonuje przesuniecie w prawo o 1 bit");
    przesun_w_prawo_1bit(ten_rejestr);
    wyswietl_rejestr(ten_rejestr);
    printf("\nWykonuje przesuniecie w lewo o 1 bit");
    przesun_w_lewo_1bit(ten_rejestr);
    wyswietl_rejestr(ten_rejestr);
    printf("\nNacisnij dowolny przycisk aby kontynuowac prace programu...\n");
    getch();

    //
    //  Zadanie 2
    //

    //  Alokacja pamieci Slownika
    Slownik* s = malloc(sizeof(Slownik));
    s->mf = malloc(sizeof(Slowo));
    s->korzen = malloc(sizeof(Slowo));

    int wolne_miejsce_tab_t = 1;

    //  Funkcja wczytujaca tekst do Slownika
    Zaladuj_tekst(s, &wolne_miejsce_tab_t);
    //printf("DEBUG Zaladowalem %d wyrazow.\n", wolne_miejsce_tab_t);

    //  Funkcja sortuje i zapisuje do pliku
    zapisz(s, wolne_miejsce_tab_t);

    //  Obliczam i wyswietlam stosunek wysokosci drzew
    printf("Stosunek wysokosci drzewa do wysokosci drzewa idealnego: %f\nIm wieksza wartosc "
           "tego stosunku, tym bardziej stworzone przez program drzewo jest niezrownowaozne "
           "w stosunku do idealnego.\n", oblicz(s));

    printf("\nNacisnij dowolny przycisk aby kontynuowac prace programu...\n");
    getch();
    return 0;
}

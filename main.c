#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*
TRESC ZADANIA
5 Napisac program ktory wczytuje pliki binarne  do tablicy dynamicznej tab:
Hex * tab;
typedef struct HexByte
 {
   char [2]; // reprezntacja bajtu w postaci 16-owej (ascii)
   unsigned char;// w postaci binarnej
 } HexByte

Program wywoluje sie z lini komend (cmd) z nastepujacymi opcjami:
hex.exe plik.in -ot plik.txt // zapisuje wczytany plik w postaci tekstowej
hex.exe plik.in -ob plik.bin  // zapisuje wczytany plik w postaci binarnej
hex.exe plik.in  -d FA36  // usuwa z pliku ciag bajtow
hex.exe plik.in  -r  DB A908 FFAA33 // zastepuje jeden ciag bajtów innym ciagiem
Opcje -d i -r moga wystepowac z opcja -ot albo -ob
*/


//=============================================================================================================
//  Komunikacja z uzytkownikiem
//=============================================================================================================
void tekst_pomocy()
{
    printf("\t\tTEKST POMOCY PROGRAMU\nSchemat wywolania funkcji: <0> <!> <2> <3> <4> <5> <6>\n"
        "\t0 - nazwa pliku, nieuzywane\n"
        "\t1 - nazwa pliku wejsciowego. Plik wejsciowy musi istniec zanim wywolamy ten program\n"
        "\t2 - format, w jakim dane maja byc zapisane w pliku wyjsciowym\n"
        "\t3 - nazwa pliku wyjsciowego. Nie musi sitniec przed wywolaniem programu, ale moze\n"
        "\t4 - typ akcji do wykonania na tekscie\n"
        "\t5 - parametr akcji. Uzywany, jesli to potrzebne\n"
        "\t6 - parametr akcji. Uzywany, jesli to potrzebne\n"
        "Przykladowe wywolanie funkcji:\n"
        "\thex.exe tekst.txt -ob bity.bin -r F A\n"
        "\tznaczenie: wczytaj tekst z pliku, zamien wszystkie wystapienia\n"
        "\tF na A oraz zapisz do pliku binarnego\n");
    return;
}

void zle_parametry()
{
    printf("Podano nieprawidlowa ilosc lub zly format parametrow.\n");
    return;
}

//=============================================================================================================
//  Uzyta struktura HexByte
//=============================================================================================================
typedef struct _HexByte
 {
   char text_form[2]; // reprezntacja bajtu w postaci 16-owej (ascii)
   unsigned char bin_form;// w postaci binarnej
 } HexByte;

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
        return NULL;
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
                return NULL;
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

    return NULL;
}
//=============================================================================================================
//  usun_ciag
/*
    Funkcja jest odpowiedzialna za usuniecie ciagu, czyli posprzatanie wartosci usuwanego elementu oraz
    aktualizacje rozmiaru tablicy dlugosc_tab.
    Element do usuniecia jest uwalniany, a na jego miejsce zostaje przypisany kolega z prawej strony.
    Zmienna f jest wskaznikiem na element ktory chcemy usunac (kolejny element znalezionego ciagu).
    Zmienna m zlicza nam ilosc usunietych znakow w danym przebiegu funkcji.

    !!! FUNKCJA UWALNIA HexByte PRZY USUWANIU CIAGU ZNAKOW
*/
//=============================================================================================================
void usun_ciag(int dlugosc_ciagu_usuwanego, int wystapienie_ciagu, HexByte*** tab, int* dlugosc_tab)
{
    int f;
    int m = 0;

    //  Petle usuwajace znaleziony ciag, aktualizacja dlugosci tablicy.
    while ( m < dlugosc_ciagu_usuwanego )
    {
        f = wystapienie_ciagu;
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
//  usun
/*
    Funkcja prostsza od funkcji zamien(). Wywoluje funkcje poszukujaca wystapienia ciagu parametr_1 po czym uzywa funkcji
    usun_ciag() aby ja usunac. Podobnie jak w funkcji znajdz(), wystapienie ciagu jest przekazywane przez indeks i.
    Do funkcji przekazany jest ADRES tab poprzez referencje, tak abysmy mogli miec dostep do tab po realloc().
*/
//=============================================================================================================
int usun(HexByte*** tab, char* parametr_1, int* dlugosc_tab)
{
    HexByte* znalazlem = 0;
    int i = 0;
    int dlugosc_ciagu_usuwanego = strlen(parametr_1);
    int dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_usuwanego;

    znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    while ( znalazlem )
    {
        usun_ciag(dlugosc_ciagu_usuwanego, i, tab, dlugosc_tab);
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_usuwanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }

    return 0;
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
int zamien(HexByte*** tab, char* parametr_1, char* parametr_2, int* dlugosc_tab)
{
    HexByte* znalazlem = 0;
    int i = 0;
    int dlugosc_ciagu_szukanego = strlen(parametr_1);
    int dlugosc_ciagu_wstawianego = strlen(parametr_2);
    int dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_szukanego;

    znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    while ( znalazlem )
    {
        wstaw_ciag(parametr_2, i, tab, dlugosc_ciagu_szukanego, dlugosc_ciagu_wstawianego, dlugosc_tab);
        //  przesuniecie indeksu i na nastepna pozycje gwarantuje, ze nie znajdziemy tego samego wystapienia dwa razy
        i++;
        //  aktualizuj dlugosc do przeszukiwania, aby nie szukac poza tablica
        dlugosc_do_szukania = *dlugosc_tab - dlugosc_ciagu_szukanego;
        znalazlem = znajdz_wystapienie(parametr_1, *tab, &i, dlugosc_do_szukania, *dlugosc_tab);
    }
    return 0;
}

//=============================================================================================================
//  zapisz_do_pliku()
/*
    Funkcja posiada dwie galezie: zapis binarny lub tekstowy.
    Kazda z galezi otwiera plik na swoj sposob (binarny lub nie) oraz testuje czy udalo sie otworzyc plik.
    Jesli pliku nie udalo sie otworzyc, wychodzimy z funkcji.

    !!! FUNKCJA UWALNIA ZASOBY HexByte ** tab ZARAZ PO ZAPISIE WARTOSCI KOMORKI tab DO PLIKU
*/
//=============================================================================================================
int zapisz_do_pliku(HexByte** tab, char* typ_zapisu, char* output_file, int dlugosc_tab)
{
    FILE* otwarty_plik;
    int m = 0;

    //
    //  Zapis binarny
    //

    if ( typ_zapisu[2] == 'b' )
    {
        otwarty_plik = fopen(output_file, "wb");
        if ( otwarty_plik == 0 )
        {
            return 0;
        }
        for ( m = 0; m < dlugosc_tab; m++ )
        {
            fwrite(&(tab[m]->bin_form), sizeof(unsigned char), 1, otwarty_plik);
            free(tab[m]);
        }
    }

    //
    //  Zapis tekstowy
    //

    if ( typ_zapisu[2] == 't' )
    {
        otwarty_plik = fopen(output_file, "w");
        if ( otwarty_plik == 0 )
        {
            return 0;
        }
        for ( m = 0; m < dlugosc_tab; m++ )
        {
            fprintf(otwarty_plik, "%c%c", tab[m]->text_form[0], tab[m]->text_form[1]);
            free(tab[m]);
        }
    }
    fclose(otwarty_plik);
    return 1;
}

//=============================================================================================================
//  main()
/*
    Schemat wywolania funkcji:
    <0> <!> <2> <3> <4> <5> <6>
    0 - nazwa pliku, nieuzywane
    1 - nazwa pliku wejsciowego. Plik wejsciowy musi istniec zanim wywolamy ten program
    2 - format, w jakim dane maja byc zapisane w pliku wyjsciowym
    3 - nazwa pliku wyjsciowego. Nie musi istniec przed wywolaniem programu, ale moze
    4 - typ akcji do wykonania na tekscie
    5 - parametr akcji. Uzywany, jesli to potrzebne
    6 - parametr akcji. Uzywany, jesli to potrzebne
    Przykladowe wywolanie funkcji:

        hex.exe tekst.txt -ob bity.bin -r F A
        znaczenie: wczytaj tekst z pliku, zamien wszystkie wystapienia
        F na A oraz zapisz do pliku binarnego
    Aby zobaczyc tekst pomocy:
        hex.exe -h

*/
//=============================================================================================================
int main(int argc, char* argv[])
{
    //
    //  Deklaracje zmiennych do wypelnienia parametrami z linii komend
    //

    int i;
    char* input_file;
    char* typ_zapisu;
    char* output_file;
    char* pozadana_akcja = 0;
    char* parametr_1 = 0;
    char* parametr_2 = 0;

    //
    //  Petla ladowania parametrow do programu
    //  Za kazdym przebiegiem petli aktywuje sie inna galaz wyrazenia switch
    //

    for ( i = 0;  i < argc; i++ )
    {
        switch(i)
        {
        case 1 :
            {
            //  wyswietl tekst pomocy i wyjdz z programu
            if ( !(abs(strcmp(argv[i], "-h"))) )
                {
                    tekst_pomocy();
                    return 0;
                }
            //  nie chce tekstu pomocy, to jedziemy dalej
            input_file = argv[i];
            break;
            }
        case 2:
            {
                typ_zapisu = argv[i];
                break;
            }
        case 3:
            {
                output_file = argv[i];
                break;
            }
        case 4:
            {
                pozadana_akcja = argv[i];
                break;
            }
        case 5:
            {
                parametr_1 = argv[i];
                break;
            }
        case 6:
            {
                parametr_2 = argv[i];
                break;
            }
        }
    }

    //
    //  Filtrujemy przypadki gdzie uzytkownik podal zle parametry i wychodzimy z funkcji jesli tak zrobil
    //

    if ( strcmp(pozadana_akcja, "-r") )
    {
        if ( strcmp(pozadana_akcja, "-d") )
        {
            if ( strcmp(pozadana_akcja, "-s") )
            {
                zle_parametry();
                return 1;
            }
        }

    }
    if ( !(abs(strcmp(pozadana_akcja, "-r"))) && ((parametr_1 == 0) || (parametr_2 == 0)))
    {
        zle_parametry();
        return 1;
    }
    if ( !(abs(strcmp(pozadana_akcja, "-d"))) && ((parametr_1 == 0) || (parametr_2 != 0)))
    {
        zle_parametry();
        return 1;
    }

    //
    //  Przygotowanie do czytania
    //  Sprawdzenie czy mozna utworzyc plik
    //  Deklaracja HexByte** tab i przypisanie nulla
    //

    FILE* otwarty_plik;
    HexByte** tab = (HexByte**) NULL;
    otwarty_plik = fopen(input_file, "rb");
    if (! (otwarty_plik) )
    {
        printf("Nie udalo sie otworzyc pliku.\n");
    }
    printf("Parametry poprawne\n");

    //  Deklaracja zmiennej iteracyjnej i ktora potem powie nam jak duzo wczytalismy
    i = 0;

    //  Deklaracja i wyczyszczenie buforu znakowego
    unsigned char* bufor_bitowy = malloc(sizeof(unsigned char));
    memset(bufor_bitowy, 0, sizeof(unsigned char));

    //  wczytaj pierwszy element, robimy to aby miec ladny warunek wykonania nastepnej petli while
    int czy_zostalo_do_wczytania =  fread(bufor_bitowy, sizeof(unsigned char), 1, otwarty_plik);

    //
    //  Petla wczytywania zawartosci pliku do tablicy dynamicznej tab
    //  Przypisanie dlugosci tablicy (wazne)
    //

    while ( czy_zostalo_do_wczytania != 0 )
    {
        i++;
        tab = (HexByte**)realloc(tab, i*sizeof(HexByte*));
        tab[i-1] = malloc(sizeof(HexByte));
        tab[i-1]->bin_form = *bufor_bitowy;
        tab[i-1]->text_form[0] = (char)((*bufor_bitowy) >> 4);
        tab[i-1]->text_form[1] = (char)((*bufor_bitowy) & 0x0F);
        //  czyscimy tymczasowy bufor
        memset(bufor_bitowy, 0, sizeof(unsigned char));
        czy_zostalo_do_wczytania =  fread(bufor_bitowy, sizeof(unsigned char), 1, otwarty_plik);
    }
    int dlugosc_tab = i;

    //
    //  Wypisanie na ekran odczytanych przez program wartosci z pliku
    //
    /*

    printf("\nOdczytana wartosc: \n");
    int j;
    printf("%0X%0X", tab[0]->text_form[0], tab[0]->text_form[1]);
    for ( j = 1; j < i; j++)
    {
        printf(" %0X%0X", tab[j]->text_form[0], tab[j]->text_form[1]);
    }
    printf("\n\n%0X", tab[0]->bin_form);
    for ( j = 1; j < i; j++)
    {
        printf(" %0X", tab[j]->bin_form);
    }

    */
    //
    //  Wywolanie podfunkcji ktora zajmie sie usunieciem lub zamiana ciagu znakow
    //

    if ( !(abs(strcmp(pozadana_akcja, "-r"))) )
    {
        printf("\nWywoluje procedure zamiany.\n");
        zamien(&tab, parametr_1, parametr_2, &dlugosc_tab);
    }
    else
    {
        if ( !(abs(strcmp(pozadana_akcja, "-d"))) )
        {
            printf("\nWywoluje procedure usuniecia.\n");
            usun(&tab, parametr_1, &dlugosc_tab);
        }
    }

    //
    //  Wypisanie na ekran wartosci ktore uzyskal program
    //
    /*

    printf("\nDEBUG Koncowa wartosc: \n");
    printf("%0X%0X", tab[0]->text_form[0], tab[0]->text_form[1]);
    for ( j = 1; j < dlugosc_tab; j++)
    {
        printf(" %0X%0X", tab[j]->text_form[0], tab[j]->text_form[1]);
    }
    printf("\n\n%0X", tab[0]->bin_form);
    for ( j = 1; j < dlugosc_tab; j++)
    {
        printf(" %0X", tab[j]->bin_form);
    }

    */
    //
    //  zapisz do pliku
    //

    int zapis_wykonano = 0;
    zapis_wykonano = zapisz_do_pliku(tab, typ_zapisu, output_file, dlugosc_tab);
    if ( !zapis_wykonano )
    {
        printf("Zapis do pliku nie powiodl sie.\n");
    }

    //
    //  Uwolnienie zasobow tablicy HexByte ** tab przez realloc() o rozmiarze 0
    //

    realloc(tab, 0);
    return 0;
}

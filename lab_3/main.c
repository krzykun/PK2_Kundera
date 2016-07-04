#include <stdio.h>
#include <stdlib.h>
/*
Dana jest struktura:
typedef struct Tekst_
{
 char *t; //wskaznik na tekst
 int r;// rozmiar tekstu w bajtach
 int lw; // liczba wyrazow w tekscie

} Tekst;

Napisac funkcje ktora wczytuje dowolny plik tekstowy do struktury powyzej ustawia pola s lw.
Naglowek funkcji:
void czytaj(Tekst *mt,char * nazwa_pliku)
W oparciu o funkcje biblioteczna strstr() napisac funkcje ktora znajduje wzorzec w tekscie i zwraca do niego
wskaznik.
Naglowek tej  funkcji:   char *  znajdz(Tekst *mt,char * wzorzec, int offset)
W oparciu o ta funkcje znalezc wszystkie wystapienia danego tekstu. Parametr offset wykorzystywany jest
przy kontynuacji wyszukiwania (offset=0 przy pierwszym wywolaniu funkcji znajdz)
W oparciu o funkcje biblioteczne strcmp i qsort napisac funkcje ktora sortuje wszystkie wyraazy w tekscie.
Naglowek funkcji:
 Tekst * sortuj(Tekst *mt); //zwraca tekst posortowany.
*/

typedef struct Tekst_
{
 char *t; //wskaznik na tekst
 int r;// rozmiar tekstu w bajtach
 int lw; // liczba wyrazów w tekscie

} Tekst;
//=====================================================================================================
// czytaj()
/*
    Zadaniem funkcji jest odczytac zawartosc pliku i zapisac ja do zmiennej.
    Zlicza ilosc wyrazow oraz rozmiar w bajtach.
*/
//=====================================================================================================
void czytaj(Tekst *mt, char * nazwa_pliku) {   //ustawia r i lw
    FILE* otwarty_plik;
    char* tmp_char;
    int czy_poprawnie_odczytalem = 0;
    int ilosc_slow = 0;

    //
    //  Czytam wyrazy z pliku
    //

    //DEBUG printf("\n\nRozpoczynam prace z plikiem...\n");
    otwarty_plik = fopen(nazwa_pliku, "r");
    if (otwarty_plik != NULL) {
        czy_poprawnie_odczytalem = fscanf(otwarty_plik, "%s", tmp_char);
        if (czy_poprawnie_odczytalem != EOF) {
            //DEBUG printf("Petla %d\ntmp_char:%s\n", ilosc_slow, tmp_char);
            strcat(mt->t, tmp_char);
            ilosc_slow++;
            czy_poprawnie_odczytalem = fscanf(otwarty_plik, "%s", tmp_char);
        }
        while (czy_poprawnie_odczytalem != EOF) {
            //DEBUG printf("\nPetla %d\ntmp_char:%s\n", ilosc_slow, tmp_char);
            strcat(mt->t, " ");
            strcat(mt->t, tmp_char);
            ilosc_slow++;
            czy_poprawnie_odczytalem = fscanf(otwarty_plik, "%s", tmp_char);
        }
        fclose(otwarty_plik);

        // przypisanie ilosci wyrazow

        mt->r = strlen(mt->t) + 1;  //przypisanie r
        mt->lw = ilosc_slow;    //przypisanie lw
        printf("\nOdczytalem %d wyrazow. Rozmiar odczytanego tekstu"
               " to %d bajtow.\n\n"
               //DEBUG "Dokladna wartosc:\n%s\n"
               , mt->lw, mt->r/*, mt->t*/);
    }
    else printf("\nNie udalo sie otworzyc pliku.\n");
    //DEBUG printf("Zakonczono prace z plikiem.\n\n");
    return;
}

void oblicz_rozmiar(Tekst* mt) {
    printf("\nRozmiar tekstu to: %d+1 (+1 to symbol konczacy ciag znakow)\n",strlen(mt->t));
}

//=====================================================================================================
//  znajdz
/*
    W oparciu o funkcje biblioteczna strstr() napisac funkcje ktora znajduje wzorzec
    w tekscie i zwraca do niego wskaznik. W oparciu o ta funkcje znalezc wszystkie
    wystapienia danego tekstu. Parametr offset wykorzystywany jest przy kontynuacji
    wyszukiwania (offset=0 przy pierwszym wywolaniu funkcji znajdz)
*/
//=====================================================================================================
char* znajdz(Tekst *mt, char* wzorzec, int offset)  {
    if (offset != 0) {
        char* temp_tekst = mt->t + offset;
        //strcpy(temp_tekst, &(mt->t[offset]));  //usun pierwszych offset znakow w podanym stringu
        char* temp2 = strstr(temp_tekst, wzorzec);
        return temp2;
    }
    char* temp = strstr(mt->t, wzorzec);
    return temp;
}  //offset - gdzie skonczyl poprzednio szukac, strstr

/*
W oparciu o funkcje biblioteczne strcmp i qsort napisac funkcje ktora sortuje wszystkie wyraazy w tekscie.
Naglowek funkcji:
 Tekst * sortuj(Tekst *mt); //zwraca tekst posortowany.
} //zwraca tekst posortowany, strcmp i qsort //w srodku musi byc malloc i ustawione pola
*/

//=====================================================================================================
// moje_porownaj
/*
    Uzywane przez qsort(). Funkcja porownuje dwa ciagi znakow i zwraca mlodszy w porzadku leksykograficznym.
*/
//=====================================================================================================
int moje_porownaj(const void * a, const void * b)
{
    char* _a = *(char**)a;
    char* _b = *(char**)b;
    return strcmp(_a, _b);
}
//=====================================================================================================
// sortuj
/*
    Funkcja dzieli ciag znakow mt->t na wyrazy oraz sortuje je leksykograficznie. Wynik zwracany jest w mt->t.
*/
//=====================================================================================================
Tekst * sortuj(Tekst *mt)
{
    //printf("\nDEBUG\n\nmt->lw: %d", mt->lw);
    char* tablica_ze_slowami[mt->lw];

    //  Pierwsza fragmentacja mt->t
    tablica_ze_slowami[0] = strtok(mt->t, " ");
    //DEBUG printf("PRZED SORTOWANIEM\n");
    //DEBUG printf("Tablica_Ze_slowami[0] = %s\n", tablica_ze_slowami[0]);
    int i = 1;

    //  Fragmentacja mt->t
    while ( ( i < mt->lw ) && (tablica_ze_slowami[i-1] != NULL) )
    {
        //printf("\n\nDEBUG\ti\t%d\tlw\t%d\n", i, mt->lw);
        tablica_ze_slowami[i] = strtok(NULL, " ");
        //DEBUG printf("Tablica_Ze_slowami[%d] = %s\n", i, tablica_ze_slowami[i]);
        i++;
    }
    printf("Sortuje...\n");

    //  Wywolanie sortowania
    qsort(tablica_ze_slowami, mt->lw, sizeof(char*), moje_porownaj);
    //DEBUG printf("Zakonczylem sortowanie.\n\nPO SORTOWANIU\n");
    /*
    for ( i = 0; i < mt->lw; i++)
    {
        //DEBUG printf("Tablica_Ze_slowami[%d] = %s\n", i, tablica_ze_slowami[i]);
    }
    */

    //
    //  Sklejamy spowrotem tekst, tym razem juz posortowany
    //

    mt->t = malloc(mt->r);
    mt->t[0] = '\0';
    strcat(mt->t, tablica_ze_slowami[0]);
    //DEBUG printf("\nSkleilem slowo: %s\nWartosc mt->t: %s\n\n", tablica_ze_slowami[0], mt->t);
    i = 1;

    while ( i < mt->lw )
    {
        strcat(mt->t, " ");
        strcat(mt->t, tablica_ze_slowami[i]);
        //DEBUG printf("Skleilem slowo: %s\nWartosc mt->t: %s\n\n", tablica_ze_slowami[i], mt->t);
        i++;
    }
    return mt;
}

//======================================================================================
//  main()
/*
    Na zapytanie "Gotowy na sortowanie?" nalezy odpowiedziec 0 lub 1, wprowadzenie innej
    wartosci prawdopodobnie spowoduje sortowanie.
*/
//======================================================================================
int main()
{
    //  Wczytanie nazwy pliku
    int rozmiar_tekstu;
    char* chce_otworzyc = malloc(20*sizeof(char));
    printf("Podaj nazwe pliku (wraz z rozszerzeniem): ");
    scanf("%s", chce_otworzyc);

    //  Wykrywanie rozmiaru wczytywanego pliku
    FILE* otwarty_plik;
    otwarty_plik = fopen(chce_otworzyc, "r");
    fseek(otwarty_plik, 0, SEEK_END);
    rozmiar_tekstu = ftell(otwarty_plik) + 1;
    rewind(otwarty_plik);
    fclose(otwarty_plik);

    //  Utworzenie struktury Tekst przetrzymujace informacje, alokacja pamieci mt
    if (rozmiar_tekstu == 0)
    {
        printf("\nPlik jest pusty lub nie istnieje. Nie ma czego szukac ani czego sortowac.\n");
        printf("Nacisnij dowolny klawisz aby zakonczyc program...\n");
        getch();
        return 0;
    }
    Tekst* mt = malloc(sizeof(Tekst));
    mt->t = malloc(rozmiar_tekstu*sizeof(char));
    memset(mt->t, "", rozmiar_tekstu*sizeof(char));
    //  Wstawienie znaku konczacego string aby program wiedzial, ze mt jest pusty
    mt->t[0] = '\0';

    //
    //  Wczytanie zawartosci pliku
    //

    //  Wywolanie funkcji potrzebnych do zrealizowania zadania
    czytaj(mt, chce_otworzyc);
    //DEBUG oblicz_rozmiar(mt);

    //
    //  Poszukiwanie wzorca w tekscie
    //

    char* pozycja_wzorca;
    int czy_szukac = 1;
    int offset_tekstu = 0;
    char* wzorzec = malloc(20*sizeof(char));
    while (czy_szukac) {
        if (czy_szukac == 2) {
            pozycja_wzorca = znajdz(mt, wzorzec, offset_tekstu);
        }
        else {
            printf("Podaj wzorzec do wyszukania: ");
            scanf("%s", wzorzec);
            offset_tekstu = 0;
            pozycja_wzorca = znajdz(mt, wzorzec, offset_tekstu);

        }
        if (pozycja_wzorca) {
            offset_tekstu = pozycja_wzorca - mt->t + 1;
            printf("\nZnaleziony wzorzec znajduje sie na %d pozycji.\n", offset_tekstu);
        }
        else {
            printf("Nie udalo sie znalezc tego wzorca w tekscie.\n");
        }
        printf("Szukac jeszcze raz?\n\n"
               "2\tTak, poszukaj tego samego\n"
               "1\tTak, ale poszukaj czegos innego\n"
               "0\tNie, chce isc dalej.\n\n");
        scanf("%d", &czy_szukac);
    }

    //
    //  Sortowanie
    //

    int czy_sortowac = 0;
    while (!czy_sortowac)
    {
        printf("\nGotowy na sortowanie?\n\n1 tak\n0 nie\n");
        scanf("%d", &czy_sortowac);
        if (czy_sortowac)
        {
            //  Wywolanie sortowania
            sortuj(mt);

            printf("Nacisnij dowolny klawisz aby zobaczyc posortowany tekst...\n");
            getch();
            printf("Posortowany tekst:\n\n%s\n", mt->t);
        }
    }
    printf("Nacisnij dowolny klawisz aby zakonczyc program...\n");
    getch();
    return 0;
}

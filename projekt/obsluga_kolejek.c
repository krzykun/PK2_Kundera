#include "struktury.h"

//=============================================================================================================
//  dodaj_do_kolejki_cofania
/*
    Funkcja dodaje zmiane do kolejki cofania. Wywolywana jest przy dodaniu/usunieciu elementu ciagu.
    Funkcja dodajaca do kolejki ponawiania nie istnieje, poniewaz dodajemy do niej tylko gdy cofamy zmiane.

    !!! FUNKCJA ALOKUJE PAMIEC DLA NOWEJ ZMIANY
*/
//=============================================================================================================
void dodaj_do_kolejki_cofania(Kolejka* kolejka_cofania, char* ciag, int pozycja, int typ_akcji)
{
    Zmiana* nowa_zmiana = malloc(sizeof(Zmiana));
    nowa_zmiana->ciag = malloc(sizeof(char));
    strcpy(nowa_zmiana->ciag, ciag);
    nowa_zmiana->pozycja = pozycja;
    nowa_zmiana->typ_akcji = typ_akcji;
    //  Ciekawe rzutowanie ktore nie jest niezbedne, ale likwiduje warning
    nowa_zmiana->nast = (struct Zmiana*) kolejka_cofania->do_kolejki;

    kolejka_cofania->do_kolejki = nowa_zmiana;
    return;
}

//=============================================================================================================
//  Pobierz_z_kolejki
/*
    Funkcja ma za zadanie pobrac pierwszy element Zmiana* z kolejki podanej w parametrze oraz zwrocic strukture
    zmiana ktora zostanie pobrana.
    Funkcja rowniez przesuwa kolejke z ktorej pobrala strukture Zmiana.
    Funkcja wprowadza zmiany w tresci przechowywanej w programie.

    Funkcja rozpoznaje przez parametr czy_odwrocic_wstaw_us czy obslugujemy kolejke cofania, czy ponawiania.
    Jesli kolejke cofania:
        Musimy odwrocic znaczenie typ_akcji struktury Zmiana. Oznacza to tyle, ze jesli uzytkownik cos dodal
        to chcemy to usunac, a jesli cos usunal, to chcemy to dodac.
    Jesli kolejke ponawiania:
        Musimy posluchac zmiennej typ_akcji, ktory rozdzieli nam odpowiednio akcje.
        Dlaczego? Bo jesli uzytkownik cos DODAL, potem to usunal (do kolejki cofania) a teraz jednak chce to
        spowrotem, to bedziemy to ponownie DODAWAC
        0 oznacza usuwanie, 1 dodawanie.

    Jesli kolejka jest pusta, funkcja zwraca NULL.
*/
//=============================================================================================================
Zmiana* pobierz_z_kolejki(HexByte*** tab, int* ilosc_znakow, Kolejka* kolejka, int czy_odwrocic_wstaw_us)
{
    if ( kolejka->do_kolejki )
    {
        if ( czy_odwrocic_wstaw_us )
        {
            if ( !(kolejka->do_kolejki->typ_akcji) )
            {
                //  !(typ_akcji) == 1 oznacza ze przy dodawaniu do kolejki zawartosc usuwalismy
                //  wiec teraz chcemy ja dodac

                wstaw_ciag_na_pozycje(tab, ilosc_znakow, kolejka->do_kolejki->pozycja, kolejka->do_kolejki->ciag, strlen(kolejka->do_kolejki->ciag));
            }
            else
            {
                usun_elementy_z_pozycji(tab, ilosc_znakow, kolejka->do_kolejki->pozycja, strlen(kolejka->do_kolejki->ciag), (Kolejka*) 0 );
            }
        }
        else
        {
            if ( kolejka->do_kolejki->typ_akcji )
            {
                // Tutaj, zawartosc ktora pierwotnie dodawalismy chcemy znowu dodac, wszystko jest w porzadku

                wstaw_ciag_na_pozycje(tab, ilosc_znakow, kolejka->do_kolejki->pozycja, kolejka->do_kolejki->ciag, strlen(kolejka->do_kolejki->ciag));
            }
            else
            {
                usun_elementy_z_pozycji(tab, ilosc_znakow, kolejka->do_kolejki->pozycja, strlen(kolejka->do_kolejki->ciag), (Kolejka*) 0 );
            }
        }

        //  Usun ta akcje z kolejki
        Zmiana* pobrano = kolejka->do_kolejki;
        kolejka->do_kolejki = (Zmiana*) kolejka->do_kolejki->nast;

        //  Dekrementuj ilosc zmian w kolejce
        kolejka->ilosc_zmian--;

        //zwracamy akcje aby dodac ja do drugiej kolejki
        return pobrano;
    }
    else
    {
        //  Jesli kolejka jest pusta, to zwroc nula i niech funkcja glowna sie tym zajmie
        return (Zmiana*) 0;
    }
}

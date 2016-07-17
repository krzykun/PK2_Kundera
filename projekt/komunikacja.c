//=============================================================================================================
//  Funkcje komunikacji z uzytkownikiem
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

#pragma once

#include "struktury.h"

#define KOMUNIKACJA_H_INCLUDED

void wczytaj_opcje(char*);
void wczytaj_wartosc(int*);

void kom_podaj_ciag();
void kom_podaj_poz();
void kom_podaj_ilosc_do_usun();
void kom_czy_nadpisac_zawartosc();
void kom_podaj_nazwe_pliku();
void kom_brak_zawartosci();
void kom_nie_mozna_otworzyc_pliku();

void informacje_o_programie();
void wydrukuj_linie(int);
void brak_zmian_w_kolejce(int);
void poczekaj_na_akcje();
int otworz_plik(char*, FILE*, char*);

int ochrona_przed_utrata_zawartosci(HexByte***);
void pokaz_menu_edycji();
void pokaz_menu_glowne();

// KOMUNIKACJA_H_INCLUDED

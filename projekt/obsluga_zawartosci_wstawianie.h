#pragma once

#include "struktury.h"

#define OBSLUGA_ZAWARTOSCI_WSTAWIANIE_H_INCLUDED

void wstaw_ciag_na_pozycje(HexByte***, int*, int, char*, int);
void zamien_ciagi(char*, int, HexByte***, int, int, int*);
int zamien(HexByte***, char*, char*, int*, Kolejka*);

// OBSLUGA_ZAWARTOSCI_WSTAWIANIE_H_INCLUDED

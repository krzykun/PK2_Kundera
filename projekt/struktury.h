#pragma once
#define STRUKTURY_H_INCLUDED

//=============================================================================================================
//  Uzyta struktura HexByte
/*
    Struktura przeniesiona z laboratorium nr 5.
*/
//=============================================================================================================
typedef struct _HexByte
 {
   char text_form[2]; // reprezentacja bajtu w postaci 16-owej (ascii)
   unsigned char bin_form;// w postaci binarnej
 } HexByte;

typedef struct _Zmiana
{
    char* ciag;
    int pozycja;
    int typ_akcji; //dod = 1 us = 0
    struct Zmiana* nast;
} Zmiana;

typedef struct _Kolejka
{
    Zmiana* do_kolejki;
    int ilosc_zmian;
} Kolejka;
// STRUKTURY_H_INCLUDED

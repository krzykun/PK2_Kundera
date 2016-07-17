#include "struktury.h"

//=============================================================================================================
//  edytuj_zawartosc
/*
*/
//=============================================================================================================
void edytuj_zawartosc(HexByte*** tab, char* wybor_menu_edycji)
{
    switch(*wybor_menu_edycji)
    {
        case '1':
            {
                return;
                printf("Obecna zawartosc:\n\n");
            }
        default:
            {
            printf("Wprowadzony znak nie posiada przypisanej funkcji.\n"
                    "Nacisnij dowolny przycisk aby kontynuowac...\n");
            getch();
            return;
            }
    }

}

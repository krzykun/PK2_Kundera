Ponieważ funkcja oczekuje otrzymania parametrów, dodaję schemat wywolania funkcji:

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

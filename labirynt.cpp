#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string.h>

using namespace std;

enum Kierunek
{
    Gora = 0,
    Prawo,
    Dol,
    Lewo
};

enum TypPola
{
    Korytarz = 0,
    Sciana,
    Wejscie
};

struct Pole
{
    TypPola typ;
    char widok;
    bool odwiedzone;
};

char widokGracza(const Kierunek kierunek)
{
    switch(kierunek)
    {
        case Gora: return '^';
        case Prawo: return '>';
        case Dol: return 'v';
        case Lewo: return '<';
    }
}

char widokMapy(const TypPola typ)
{
    switch(typ)
    {
        case Korytarz: return '_';
        case Sciana: return '#';
        case Wejscie: return 'W';
    }
}

void naprzodXY(int &x, int &y, const Kierunek kierunek)
{
    switch(kierunek)
    {
        case Gora: y--; break;
        case Prawo: x++; break;
        case Dol: y++; break;
        case Lewo: x--; break;
    }
}

void obrot(Kierunek &kierunek, Kierunek strona)
{
    int roznica;
    if(strona == Prawo)
        roznica = 1;
    else
        roznica = 3;//Obrot w lewo = 3 obroty w prawo
    kierunek = (Kierunek)(((int)kierunek+roznica)%4);
}

void program(int E)
{
    srand(time(NULL));

    const int kosztEnergiiL = 10;
    const int kosztEnergiiP = 10;
    const int kosztEnergiiN = 15;
    const int kosztEnergiiT = 30;

    int punkty = 0;
    int graczY = 4;
    int graczX = 10;
    Kierunek kierunek = Lewo;

    string labirynt[8];
    labirynt[0] = "11111111111";
    labirynt[1] = "11110000011";
    labirynt[2] = "11100111011";
    labirynt[3] = "11101000011";
    labirynt[4] = "11101011020";
    labirynt[5] = "11000010011";
    labirynt[6] = "10011000111";
    labirynt[7] = "11111111111";

    const int mapaWy = 8;
    const int mapaSz = 11;
    Pole **mapa;
    mapa = new Pole*[mapaWy];
    for(int y = 0; y < mapaWy; y++)
    {
        mapa[y] = new Pole[mapaSz];
        for(int x = 0; x < mapaSz; x++)
        {
            mapa[y][x].typ = (TypPola)(labirynt[y][x]-48);//Zamiana char > int > TypPola
            mapa[y][x].widok = '?';
            mapa[y][x].odwiedzone = false;
        }
    }
    mapa[graczY][graczX-1].typ = Wejscie;
    mapa[graczY][graczX-1].odwiedzone = true;
    mapa[graczY][graczX].typ = Korytarz;
    mapa[graczY][graczX].odwiedzone = true;
    for(int y = 0; y < mapaWy; y++)
    {
        mapa[y][mapaSz-2].widok = widokMapy(mapa[y][mapaSz-2].typ);
        mapa[y][mapaSz-1].widok = widokMapy(mapa[y][mapaSz-1].typ);
    }
    mapa[graczY][graczX].widok = widokGracza(kierunek);

    cout<< "Eksplorator labiryntu" << endl
        << "Celem gry jest odwiedzenie jak najwiekszej liczby pol labiryntu i wyjscie na zewnatrz" << endl
        << "Wyczerpanie energii skutkuje przegrana" << endl
        << "Mozliwe ruchy gracza:" << endl
        << "L - lewo - Koszt: 10 pkt energii" << endl
        << "P - prawo - Koszt: 10 pkt energii" << endl
        << "N - naprzod - Koszt: 15 pkt energii" << endl
        << "T - teleportacja - Koszt: 30 pkt energii" << endl;

    bool stanGry = true;
    while(stanGry)
    {
        cout << endl;
        for(int y = 0; y < mapaWy; y++)
        {
            for(int x = 0; x < mapaSz; x++)
            {
                cout << mapa[y][x].widok;
            }
            cout << endl;
        }
        {
            cout << endl << "Masz " << E << " pkt. energii, przed Toba ";
            int y = graczY;
            int x = graczX;
            naprzodXY(x, y, kierunek);
            if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
            {
                switch(mapa[y][x].typ)
                {
                    case Korytarz: cout << "korytarz"; break;
                    case Sciana: cout << "sciana"; break;
                    case Wejscie: cout << "wejscie do labiryntu"; break;
                }
            }
            else
                cout << "koniec mapy";
            cout << " co robisz (L/P/N/T)?: ";
        }

        char wybor;
        cin >> wybor;
        cin.clear();
        cin.ignore(1000, '\n');

        switch(wybor)
        {
            case 'L': case 'l':
            {
                obrot(kierunek, Lewo);
                int y = graczY;
                int x = graczX;

                naprzodXY(x, y, kierunek);
                if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
                    mapa[y][x].widok = widokMapy(mapa[y][x].typ);
                mapa[graczY][graczX].widok = widokGracza(kierunek);
                E -= kosztEnergiiL;
                break;
            }

            case 'P': case 'p':
            {
                obrot(kierunek, Prawo);
                int y = graczY;
                int x = graczX;

                naprzodXY(x, y, kierunek);
                if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
                    mapa[y][x].widok = widokMapy(mapa[y][x].typ);
                mapa[graczY][graczX].widok = widokGracza(kierunek);
                E -= kosztEnergiiP;
                break;
            }

            case 'N': case 'n':
            {
                int y = graczY;
                int x = graczX;
                naprzodXY(x, y, kierunek);
                if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
                {
                    if(mapa[y][x].typ != Sciana)
                    {
                        if(mapa[y][x].typ == Wejscie && punkty > 0)
                        {
                            stanGry = false;
                            break;
                        }
                        if(!mapa[y][x].odwiedzone)
                        {
                            mapa[y][x].odwiedzone = true;
                            punkty++;
                        }
                        mapa[graczY][graczX].widok = widokMapy(mapa[graczY][graczX].typ);
                        graczY = y;
                        graczX = x;

                        naprzodXY(x, y, kierunek);
                        if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
                            mapa[y][x].widok = widokMapy(mapa[y][x].typ);
                        mapa[graczY][graczX].widok = widokGracza(kierunek);
                        E -= kosztEnergiiN;
                    }
                    else
                        cout << "Nie mozna isc naprzod - Sciana" << endl;
                }
                else
                    cout << "Nie mozna isc naprzod - Koniec planszy" << endl;
                break;
            }

            case 'T': case 't':
            {
                bool znaleziono = false;
                for(int y = 0; y < mapaWy && !znaleziono; y++)
                {
                    for(int x = 0; x < mapaSz && !znaleziono; x++)
                    {
                        znaleziono = (mapa[y][x].typ == Korytarz && !mapa[y][x].odwiedzone);
                    }
                }

                if(znaleziono)
                {
                    int y;
                    int x;
                    do
                    {
                        y = rand()%mapaWy;
                        x = rand()%mapaSz;
                    }while(mapa[y][x].typ != Korytarz || mapa[y][x].odwiedzone);

                    mapa[graczY][graczX].widok = widokMapy(mapa[graczY][graczX].typ);
                    graczY = y;
                    graczX = x;
                    mapa[y][x].odwiedzone = true;
                    punkty++;

                    kierunek = (Kierunek)(rand()%4);

                    naprzodXY(x, y, kierunek);
                    if(x >= 0 && x < mapaSz && y >= 0 && y < mapaWy)
                        mapa[y][x].widok = widokMapy(mapa[y][x].typ);
                    mapa[graczY][graczX].widok = widokGracza(kierunek);

                    E -= kosztEnergiiT;
                }
                else
                    cout << "Wszystkie pola zostaly juz odwiedzone" << endl;
                break;
            }
        }

        if(E < 0)
        {
            cout << "Energia wyczerpana - PRZEGRANA!" << endl;
            stanGry = false;
        }
    }
    if(E >= 0)
        cout << "Koniec gry. Wynik: " << punkty << endl;
}

int main()
{
    int E = 500;
    program(E);

    return 0;
}

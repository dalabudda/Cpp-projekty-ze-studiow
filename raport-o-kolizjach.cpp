#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


const int dlugoscDaty = 10;


struct Data
{
    int dzien;
    int miesiac;
    int rok;
};


bool czyZgodne(const char *str1, const char *str2)//Jezeli jeden napis zawiera w sobie drugi zwraca prawde
{
    int i = 0;
    while(str1[i] == str2[i])
    {
        if((str1[i] == '\0' || str2[i] == '\0') && i > 0)
            return true;
        i++;
    }
    if((str1[i] == '\0' || str2[i] == '\0') && i > 0)
        return true;
    else
        return false;
}


bool czyPoprawnaData(const char data[dlugoscDaty])
{
    for(int i = 0; i < dlugoscDaty; i++)
    {
        if((i == 2 || i == 5) && data[i] != '-')
            return false;
        else if(data[i] < 48 || data[i] > 57)
            return false;
        else
            return true;
    }
}


bool czyOdDo(const Data data, const Data dataOd, const Data dataDo)
{
    return
    (
        (
            dataOd.rok == 0 ||//Od poczatku
            data.rok > dataOd.rok ||
            (
                data.rok == dataOd.rok &&
                (
                    data.miesiac > dataOd.miesiac ||
                    (
                        data.miesiac == dataOd.miesiac &&
                        data.dzien >= dataOd.dzien
                    )
                )
            )
        ) &&
        (
            dataDo.rok == 0 ||//Do teraz
            data.rok < dataDo.rok ||
            (
                data.rok == dataDo.rok &&
                (
                    data.miesiac < dataDo.miesiac ||
                    (
                        data.miesiac == dataDo.miesiac &&
                        data.dzien <= dataDo.dzien
                    )
                )
            )
         )
    );
}


char *dataDoChar(const Data data)
{
    char *wynik;
    wynik = new char[dlugoscDaty+1];

    wynik[0] = data.dzien/10 +48;
    wynik[1] = data.dzien%10 +48;
    wynik[2] = '-';
    wynik[3] = data.miesiac/10 +48;
    wynik[4] = data.miesiac%10 +48;
    wynik[5] = '-';
    wynik[6] = data.rok/1000 +48;
    wynik[7] = (data.rok/100)%10 +48;
    wynik[8] = (data.rok/10)%10 +48;
    wynik[9] = data.rok%10 +48;
    wynik[10] = '\0';

    return wynik;
}


Data charDoData(const char data[dlugoscDaty])
{
    Data wynik;

    wynik.dzien = (data[0]-48)*10 + (data[1]-48);
    wynik.miesiac = (data[3]-48)*10 + (data[4]-48);
    wynik.rok = (data[6]-48)*1000 + (data[7]-48)*100 + (data[8]-48)*10 + (data[9]-48);

    return wynik;
}


int liczKody(const char *linia, const int start)
{
    int i = start;
    int liczbaKodow = 0;
    while(linia[i] != '\0')
    {
        if(linia[i] != ' ')
            if(linia[i-1] == ' ')
                liczbaKodow++;
        i++;
    }

    return liczbaKodow;
}


int liczDlugoscKodu(const char *linia, const int start)
{
    int i = start;
    while(linia[i] != ' ' && linia[i] != '\0')
        i++;

    return i - start;
}


char **wczytajKody(const char *region)
{
    const int maksDlugoscLini = 300;

    char **kody = 0;
    ifstream plik("kody.txt");

    char linia[maksDlugoscLini];
    while(plik.getline(linia, maksDlugoscLini))
    {
        if(czyZgodne(linia, region))
        {
            int i = 0;
            while(linia[i] != ' ')//przesuniecie o nazwe regionu
                i++;
            i++;//przesuniecie na litere za spacja

            int liczbaKodow = liczKody(linia, i);
            kody = new char*[liczbaKodow+1];//+1 dla zerowego wskaznika

            for(int lK = 0; lK < liczbaKodow; lK++)
            {
                int dlugoscKodu = liczDlugoscKodu(linia, i);
                kody[lK] = new char[dlugoscKodu+1];//+1 dla '\0'

                for(int dK = 0; dK < dlugoscKodu; dK++)
                {
                    kody[lK][dK] = linia[i];
                    i++;
                }
                kody[lK][dlugoscKodu] = '\0';//koniec napisu

                i++;
            }
            kody[liczbaKodow] = 0;//koniec tablicy napisow

            break;
        }
    }

    plik.close();

    return kody;
}


void rysujNaglowekRaportu(const int typRaportu)
{
    cout << endl << endl;

    switch(typRaportu)
    {
        case 1:
        {
            cout<< "Raport o pojazdach:" << endl << endl
                << setw(11) << "Rejestracja"
                << setw(10) << "Przebieg"
                << setw(11) << "Rok prod."
                << setw(8) << "Paliwo"
                << setw(9) << "Wartosc"
                << setw(15) << "Polisa"
                << setw(9) << "Kolizje";
            break;
        }

        case 2:
        {
            cout<< "Raport o kolizjach - dlugi:" << endl << endl
                << setw(10) << "Data"
                << setw(15) << "Polisa"
                << setw(13) << "Rejestracja"
                << setw(10) << "Przebieg"
                << setw(11) << "Rok prod."
                << setw(8) << "Paliwo"
                << setw(9) << "Wartosc";
            break;
        }

        case 3:
        {
            cout<< "Raport o kolizjach - krotki:" << endl << endl
                << setw(10) << "Data"
                << setw(15) << "Polisa";
            break;
        }
    }

    cout << endl << endl;
}


void raport(char const* const* kody, const Data dataOd, const Data dataDo, const int typRaportu)
{
    rysujNaglowekRaportu(typRaportu);

    ifstream pojazdy, kolizje;
    char rejestracja[10], paliwo, polisa[20], polisa2[20], data[11];
    int przebieg, rokProd, wartosc;

    pojazdy.open("pojazdy.txt");
    while(pojazdy >> rejestracja >> przebieg >> rokProd >> paliwo >> wartosc >> polisa)
    {
        bool zgodne;//zgodnosc rejestracji z regionem
        if(kody == 0)//Dla zerowego wskaznika wypisuje wszystkie dane
            zgodne = true;
        else
        {
            int i = 0;
            while(kody[i] != 0)
            {
                if(zgodne = czyZgodne(rejestracja, kody[i]))
                    break;
                i++;
            }
        }

        if(zgodne)
        {
            int liczbaKolizji = 0;

            kolizje.open("kolizje.txt");
            while(kolizje >> polisa2 >> data)
            {
                if(czyZgodne(polisa2, polisa))//kolizja powiazana z pojazdem
                {
                    if(czyOdDo(charDoData(data), dataOd, dataDo))
                    {
                        liczbaKolizji++;

                        if(typRaportu == 2)
                        {
                            cout<< setw(10) << data
                                << setw(15) << polisa2
                                << setw(13) << rejestracja
                                << setw(10) << przebieg
                                << setw(11) << rokProd
                                << setw(8) << paliwo
                                << setw(9) << wartosc << endl;
                        }
                        if(typRaportu == 3)
                        {
                            cout<< setw(10) << data
                                << setw(15) << polisa2 << endl;
                        }
                    }
                }
            }
            kolizje.close();

            if(typRaportu == 1)
            {
                cout<< setw(11) << rejestracja
                    << setw(10) << przebieg
                    << setw(11) << rokProd
                    << setw(8) << paliwo
                    << setw(9) << wartosc
                    << setw(15) << polisa
                    << setw(9) << liczbaKolizji << endl;
            }

        }
    }
    pojazdy.close();
}


void wyborRegionu(char **&kody)
{
    const int liczbaRegionow = 17;
    const char *regiony[liczbaRegionow];
    regiony[0] = "Polska";
    regiony[1] = "dolnoslaskie";
    regiony[2] = "kujawsko-pomorskie";
    regiony[3] = "lubelskie";
    regiony[4] = "lubuskie";
    regiony[5] = "lodzkie";
    regiony[6] = "malopolskie";
    regiony[7] = "mazowieckie";
    regiony[8] = "opolskie";
    regiony[9] = "podkarpackie";
    regiony[10] = "podlaskie";
    regiony[11] = "pomorskie";
    regiony[12] = "slaskie";
    regiony[13] = "swietokrzyskie";
    regiony[14] = "warminsko-mazurskie";
    regiony[15] = "wielkopolskie";
    regiony[16] = "zachodniopomorskie";

    cout << "Regiony: " << endl;
    for(int i = 0; i < liczbaRegionow; i++)
        cout << i << ". " << regiony[i] << endl;

    int wybor;
    do{
        cout << "Wybierz region [0-16]: ";
        cin >> wybor;
        cin.clear();
        cin.ignore(1000, '\n');
    }while(wybor < 0 || wybor > 16);
    cout << endl;

    if(wybor == 0)
        kody = 0;
    else
        kody = wczytajKody(regiony[wybor]);
}


void wyborDaty(Data dataOd, Data dataDo)
{
    int wybor;
    do{
        cout << "Zakres dat: " << endl;
        cout << "1. Od: ";
        if(dataOd.rok == 0)
            cout << "Poczatek";
        else
            cout << dataDoChar(dataOd);
        cout << endl;
        cout << "2. Do: ";
        if(dataDo.rok == 0)
            cout << "Teraz";
        else
            cout << dataDoChar(dataDo);
        cout << endl;
        cout << "3. Zatwierdz" << endl;

        do{
            cout << "Wybierz [1-3]: ";
            cin >> wybor;
            cin.clear();
            cin.ignore(1000, '\n');
        }while(wybor < 1 || wybor > 3);
        cout << endl;

        if(wybor != 3)
        {
            char data[dlugoscDaty];
            do{
                cout << "Wpisz date ";
                if(wybor == 1)
                    cout << "od";
                else
                    cout << "do";
                cout << " [DD-MM-RRRR]: ";
                cin >> data;
                cin.clear();
                cin.ignore(1000, '\n');
            }while(!czyPoprawnaData(data));
            cout << endl;

            if(wybor == 1)
                dataOd = charDoData(data);
            else
                dataDo = charDoData(data);
        }
    }while(wybor != 3);
}


int wyborRaportu()
{
    cout << "1. Raport o pojazdach" << endl;
    cout << "2. Raport o kolizjach dlugi" << endl;
    cout << "3. Raport o kolizjach krotki" << endl;
    int wybor;
    do{
        cout << "Wybierz raport [1-3]: ";
        cin >> wybor;
        cin.clear();
        cin.ignore(1000, '\n');
    }while(wybor < 1 || wybor > 3);
    cout << endl;

    return wybor;
}


int main()
{
    char **kody;

    wyborRegionu(kody);

    Data dataOd, dataDo;
    dataOd.rok = dataDo.rok = 0;

    wyborDaty(dataOd, dataDo);

    raport(kody, dataOd, dataDo, wyborRaportu());

    return 0;
}

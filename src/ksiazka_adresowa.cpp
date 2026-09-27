#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ksiazka_adresowa.hpp"

using namespace std;

KsiazkaAdresowa::KsiazkaAdresowa(){
    current_id = 0;
    odczyt_z_pliku();
}

KsiazkaAdresowa::~KsiazkaAdresowa(){
    if (plik.is_open()){
        plik.close();
    }
}

void KsiazkaAdresowa::zapisz_czlonka(){
    struktura adresat;

    cout<<"Podaj imie czlonka: ";
    cin>>adresat.imie;
    cout<<"Podaj nazwisko: ";
    cin>>adresat.nazwisko;
    cout<<"Podaj numer telefonu: ";
    cin.ignore();
    getline(cin, adresat.numer_telefonu);
    cin>>adresat.numer_telefonu;
    cout<<"Podaj email: ";
    cin>>adresat.email;
    cout<<"Podaj pelny adres: ";
    cin.ignore();
    getline(cin, adresat.adres);
    cout<<endl;

    adresat.id = current_id;
    zapis_do_pliku(adresat);
}


void KsiazkaAdresowa::wyszukaj_czlonka(){
    vector<struktura> znalezieni_adresaci;
    unsigned short int typ_wyszukiwania{1};
    string dane_wpisywane_przez_uzytkownika;

    cout<<"W jaki sposob chcesz wyszukac czlonka ksiazki adresowej? Po imieniu (wpisz 1), po nazwisku (wpisz 2) : ";
    cin>>typ_wyszukiwania;

    switch(typ_wyszukiwania){
        case 1:
            
            cout<<"Wybrano opcje wyszukiwania po imieniu, podaj imie adresata do znalezienia : ";
            cin>>dane_wpisywane_przez_uzytkownika;

            for(size_t i{0}; i<ksiazka_adresowa.size(); ++i){
                if(ksiazka_adresowa[i].imie.compare(dane_wpisywane_przez_uzytkownika) == 0){
                    znalezieni_adresaci.push_back(ksiazka_adresowa[i]);
                }
            }
            break;
        
        case 2:
            
            cout<<"Wybrano opcje wyszukiwania po nazwisku, podaj nazwisko adresata do znalezienia : ";
            cin>>dane_wpisywane_przez_uzytkownika;

            for(size_t i{0}; i<ksiazka_adresowa.size(); ++i){
                if(ksiazka_adresowa[i].nazwisko.compare(dane_wpisywane_przez_uzytkownika) == 0){
                    znalezieni_adresaci.push_back(ksiazka_adresowa[i]);
                }
            }
            break;

        default:
            cerr<<"Wybrano niepoprawna liczbe, sprawdz dopuszczalne liczby do wpisania i sprobuj ponownie"<<endl;
            return;
    }

    if (znalezieni_adresaci.size() > 1)
    {
        cout<<"Znaleziono " << znalezieni_adresaci.size() << " adresatow o podanych atrybutach :"<<endl;
        
        for(size_t i{0}; i<znalezieni_adresaci.size() ;++i){
            cout<<" "<<i+1<<". "<<znalezieni_adresaci[i].imie<<" "<<znalezieni_adresaci[i].nazwisko<<endl;
        }
        cout<<endl;
    }
    else if( znalezieni_adresaci.size() == 1){
        cout<<"Znaleziono 1 adresata o podanych atrybutach : 1. "<<znalezieni_adresaci[0].imie <<" "<<znalezieni_adresaci[0].nazwisko<<endl;
    }
    else{
        cout<<"Nieodnaleziono zadnych adresatow o zadanych atrybutach, sprawdz poprawnosc wpisywanych nazw, pamietaj o nieuzywawniu polskich znakow!"<<endl;
    }

    if(znalezieni_adresaci.size() > 0){
        typ_wyszukiwania = 1;

        while(typ_wyszukiwania!=0){
            cout<<"Czy chcesz szczegolowe dane ktoregos ze znalezionych adresatow? (wpisz numer z powyzszych adresatow, aby wyswietlic jego dokladne dane, w przeciwnym razie wpisz 0) : ";
            cin>>typ_wyszukiwania;

            if (typ_wyszukiwania != 0 && typ_wyszukiwania <= znalezieni_adresaci.size()){
                cout<<endl<<"Oto informacje o adresacie : "<<znalezieni_adresaci[typ_wyszukiwania-1].imie<<" "<<znalezieni_adresaci[typ_wyszukiwania-1].nazwisko<<endl;
                cout<<" id:          "<<znalezieni_adresaci[typ_wyszukiwania-1].id<<endl;
                cout<<" email:       "<<znalezieni_adresaci[typ_wyszukiwania-1].email<<endl;
                cout<<" telefon:     "<<znalezieni_adresaci[typ_wyszukiwania-1].numer_telefonu<<endl;
                cout<<" pelny adres: "<<znalezieni_adresaci[typ_wyszukiwania-1].adres<<endl<<endl;
            }
            else if (typ_wyszukiwania > znalezieni_adresaci.size()){
                cerr<<"Podano niepoprawny numer, sprawdz numery przy imionach znalezionych osob i wpisz jeden z nich!"<<endl;
            }
        }
    }
}

void KsiazkaAdresowa::wyswietl_wszystkich(){
    cout<<"Wszystkie dane ksiazki adresowej: "<<endl<<endl;

    for(size_t i{0}; i<ksiazka_adresowa.size(); ++i){
        cout<<" id:          "<<ksiazka_adresowa[i].id<<endl;
        cout<<" imie:        "<<ksiazka_adresowa[i].imie<<endl;
        cout<<" nazwisko:    "<<ksiazka_adresowa[i].nazwisko<<endl;
        cout<<" email:       "<<ksiazka_adresowa[i].email<<endl;
        cout<<" telefon:     "<<ksiazka_adresowa[i].numer_telefonu<<endl;
        cout<<" pelny adres: "<<ksiazka_adresowa[i].adres<<endl;
        cout<<"////////////////////////////////////////////////////////////////"<<endl<<endl;
    }
}

bool KsiazkaAdresowa::odczyt_z_pliku(){
    plik.open(NAZWA_PLIKU, ios::in);
    string linia;
    unsigned int numer_linii {1};
    struktura adresat;

    if(!plik.good()){
        cerr<<"Problem z otwarciem pliku, sprawdz prawa dostepu badz istnienie samego pliku o nazwie: "<<NAZWA_PLIKU<<endl;
        cout<<"Zignoruj ten blad jesli uruchamiasz aplikacje pierwszy raz."<<endl<<endl;
        return false;
    }
    else{
        cout<<"Odczyt danych z pliku "<<NAZWA_PLIKU<<" w trakcie..."<<endl;

        while(getline(plik,linia)){
            switch(numer_linii){
                case 1: adresat.id = stoi(linia);   break;
                case 2: adresat.imie = linia;   break;
                case 3: adresat.nazwisko = linia;   break;
                case 4: adresat.numer_telefonu = linia; break;
                case 5: adresat.email = linia; break;
                case 6: 
                    adresat.adres = linia; 
                    ksiazka_adresowa.push_back(adresat);
                    current_id++;
                    numer_linii = 0;
                    break;
            }
            numer_linii++;
        }

        cout<<"Zakonczono odczyt danych! Odczytano "<<ksiazka_adresowa.size()<<" adresatow."<<endl;
    }
    plik.close();
    return true;
}

bool KsiazkaAdresowa::zapis_do_pliku(const struktura &adresat){
    plik.open(NAZWA_PLIKU, ios::app);

    if(!plik.good()){
        cerr<<"Plik nie moze byc otwarty badz stworzony. Sprawdz prawa do modyfikacji pliku!"<<endl;
        return false;
    }
    else{
        cout<<"Rozpoczynam zapis do pliku "<<NAZWA_PLIKU<<" nowego adresata "<<adresat.imie<<" "<<adresat.nazwisko<<"."<<endl;

        plik<<adresat.id<<endl;
        plik<<adresat.imie<<endl;
        plik<<adresat.nazwisko<<endl;
        plik<<adresat.numer_telefonu<<endl;
        plik<<adresat.email<<endl;
        plik<<adresat.adres<<endl;

        cout<<"Zakonczono zapis!"<<endl; 

        ksiazka_adresowa.push_back(adresat);
        current_id++;
    }
    plik.close();
    return true;
}
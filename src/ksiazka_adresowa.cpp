#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ksiazka_adresowa.hpp"
#include <sstream>

using namespace std;

KsiazkaAdresowa::KsiazkaAdresowa(unsigned short int nadawca_id){
    current_id = 0;
    this->nadawca_id = nadawca_id;
    odczyt_z_pliku();
}

KsiazkaAdresowa::~KsiazkaAdresowa(){
    if (plik.is_open()){
        plik.close();
    }

    if (plik_temp.is_open()){
        plik_temp.close();
    }
}

void KsiazkaAdresowa::zapisz_czlonka(){
    struktura_adresaci adresat;

    cout<<"Podaj imie czlonka: ";
    cin>>adresat.imie;
    cout<<"Podaj nazwisko: ";
    cin>>adresat.nazwisko;
    cout<<"Podaj numer telefonu: ";
    cin.ignore();
    getline(cin, adresat.numer_telefonu);
    cout<<"Podaj email: ";
    cin>>adresat.email;
    cout<<"Podaj pelny adres: ";
    cin.ignore();
    getline(cin, adresat.adres);
    cout<<endl;

    adresat.id = current_id;
    adresat.nadawca_id = nadawca_id;
    zapis_do_pliku(adresat);
}


void KsiazkaAdresowa::wyszukaj_czlonka(){
    vector<struktura_adresaci> znalezieni_adresaci;
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

void KsiazkaAdresowa::odczytywanie_do_struktury( struktura_adresaci& adresat, unsigned int typ_zapisu, struktura_edycja_adresata_w_ksiazce edytowany_adresat){
    string linia;
    unsigned int numer_linii {1};
    std::stringstream ss(linia);
    std::string atrybut;

    while(getline(plik,linia)){
        ss.str(linia);
        while(getline(ss, atrybut,'|')){
            switch(numer_linii){
                case 1: adresat.id = stoi(atrybut);   break;
                case 2: adresat.nadawca_id = stoi(atrybut);   break;
                case 3: adresat.imie = atrybut;   break;
                case 4: adresat.nazwisko = atrybut;   break;
                case 5: adresat.numer_telefonu = atrybut; break;
                case 6: adresat.email = atrybut; break;
                case 7: 
                    adresat.adres = atrybut; 

                    switch (typ_zapisu)
                    {
                    case 0:
                        if(adresat.nadawca_id == nadawca_id){
                            ksiazka_adresowa.push_back(adresat);
                        }
                        current_id = adresat.id + 1;
                        break;
                    case 1:
                        if (adresat.id != edytowany_adresat.id){
                            zapisywanie_adresata_do_pliku(adresat, plik_temp);
                        }
                        else{
                            zapisywanie_adresata_do_pliku(ksiazka_adresowa[edytowany_adresat.iteracja], plik_temp);
                        }
                        break;                    
                    case 2:
                        if (adresat.id != edytowany_adresat.id){
                            zapisywanie_adresata_do_pliku(adresat, plik_temp);
                        }
                        else{
                            cout<<"Usunieto adresata o podanych atrybutach."<<endl;
                        }
                        break;
                    }
                    numer_linii = 0;
                    break;
            }
            numer_linii++;
        }
        ss.clear();
    }
}

void KsiazkaAdresowa::zapisywanie_adresata_do_pliku(const struktura_adresaci& adresat, std::fstream& plik){
    plik<<adresat.id<<'|';
    plik<<adresat.nadawca_id<<'|';
    plik<<adresat.imie<<'|';
    plik<<adresat.nazwisko<<'|';
    plik<<adresat.numer_telefonu<<'|';
    plik<<adresat.email<<'|';
    plik<<adresat.adres<<endl;
}

bool KsiazkaAdresowa::odczyt_z_pliku(){
    plik.open(NAZWA_PLIKU, ios::in);

    struktura_adresaci adresat;

    if(!plik.good()){
        cerr<<"Problem z otwarciem pliku, sprawdz prawa dostepu badz istnienie samego pliku o nazwie: "<<NAZWA_PLIKU<<endl;
        cout<<"Zignoruj ten blad jesli uruchamiasz aplikacje pierwszy raz."<<endl<<endl;
        return false;
    }

    cout<<"Odczyt danych z pliku "<<NAZWA_PLIKU<<" w trakcie..."<<endl;
    odczytywanie_do_struktury(adresat);
    cout<<"Zakonczono odczyt danych! Odczytano "<<ksiazka_adresowa.size()<<" adresatow."<<endl;
    
    plik.close();
    return true;
}


bool KsiazkaAdresowa::zapis_do_pliku(const struktura_adresaci &adresat){
    plik.open(NAZWA_PLIKU, ios::app);

    if(!plik.good()){
        cerr<<"Plik nie moze byc otwarty badz stworzony. Sprawdz prawa do modyfikacji pliku!"<<endl;
        return false;
    }

    cout<<"Rozpoczynam zapis do pliku "<<NAZWA_PLIKU<<" nowego adresata "<<adresat.imie<<" "<<adresat.nazwisko<<"."<<endl;
    zapisywanie_adresata_do_pliku(adresat, plik);
    cout<<"Zakonczono zapis!"<<endl; 

    ksiazka_adresowa.push_back(adresat);
    current_id++;
    
    plik.close();
    return true;
}

struktura_edycja_adresata_w_ksiazce KsiazkaAdresowa::edytuj_adresata_w_ksiazce(const std::string& imie, const std::string& nazwisko){
    struktura_edycja_adresata_w_ksiazce edytowany_adresat;
    for(size_t i = 0; i < ksiazka_adresowa.size(); ++i){
        if(ksiazka_adresowa[i].imie.compare(imie) == 0 && ksiazka_adresowa[i].nazwisko.compare(nazwisko) == 0){
            edytowany_adresat.znaleziono_adresata = true;
            edytowany_adresat.id = ksiazka_adresowa[i].id;
            edytowany_adresat.iteracja = i;

            cout<<"Znaleziono adresata o podanych atrybutach."<<endl;
            cout<<"Edytuj dane adresata : "<<ksiazka_adresowa[i].imie<<" "<<ksiazka_adresowa[i].nazwisko<<endl;
            cout<<"Podaj nowe imie : ";
            cin>>ksiazka_adresowa[i].imie;
            cout<<"Podaj nowe nazwisko : ";
            cin>>ksiazka_adresowa[i].nazwisko;
            cout<<"Podaj nowy numer telefonu : ";
            cin.ignore();
            getline(cin, ksiazka_adresowa[i].numer_telefonu);
            cout<<"Podaj nowy email : ";
            cin>>ksiazka_adresowa[i].email;
            cout<<"Podaj nowy pelny adres : ";
            cin.ignore();
            getline(cin, ksiazka_adresowa[i].adres);

            cout<<"Zakonczono edycje danych adresata, zapisuje zmiany do pliku..."<<endl;
            break;
        }
    }
    return edytowany_adresat;
}

void KsiazkaAdresowa::edytuj_czlonka(){
    std::string imie, nazwisko;
    cout<<"Podaj imie i nazwisko adresata, ktorego chcesz edytowac : ";
    cin>>imie>>nazwisko;

    struktura_adresaci temp_adresat;
    struktura_edycja_adresata_w_ksiazce edytowany_adresat = edytuj_adresata_w_ksiazce(imie, nazwisko);
    
    if (!edytowany_adresat.znaleziono_adresata){
        cout<<"Nie znaleziono adresata o podanych atrybutach, sprawdz poprawnosc wpisywanych danych i sprobuj ponownie!"<<endl;
        return;
    }
    
    plik_temp.open(NAZWA_PLIKU_TEMP, ios::out);
    plik.open(NAZWA_PLIKU, ios::in);

    if(!plik.good() || !plik_temp.good()){
        cerr<<"Pliki nie moga byc otwarte badz stworzone. Sprawdz prawa do modyfikacji plikow!"<<endl;
        return;
    }

    odczytywanie_do_struktury(temp_adresat, 1, edytowany_adresat);

    plik.close();
    plik_temp.close();

    remove(NAZWA_PLIKU.c_str());
    rename(NAZWA_PLIKU_TEMP.c_str(), NAZWA_PLIKU.c_str());
}

void KsiazkaAdresowa::usun_czlonka(){
    std::string imie, nazwisko;
    cout<<"Podaj imie i nazwisko adresata, ktorego chcesz usunac : ";
    cin>>imie>>nazwisko;

    struktura_adresaci temp_adresat;
    struktura_edycja_adresata_w_ksiazce edytowany_adresat;

    for(size_t i = 0; i < ksiazka_adresowa.size(); ++i){
        if(ksiazka_adresowa[i].imie.compare(imie) == 0 && ksiazka_adresowa[i].nazwisko.compare(nazwisko) == 0){
            cout<<"Adresat o podanych atrybutach zostal znaleziony."<<endl;
            edytowany_adresat.znaleziono_adresata = true;
            edytowany_adresat.id = ksiazka_adresowa[i].id;
            break;
        }
    }

    if (!edytowany_adresat.znaleziono_adresata){
        cout<<"Nie znaleziono adresata o podanych atrybutach, sprawdz poprawnosc wpisywanych danych i sprobuj ponownie!"<<endl;
        return;
    }

    plik_temp.open(NAZWA_PLIKU_TEMP, ios::out);
    plik.open(NAZWA_PLIKU, ios::in);

    if(!plik.good() || !plik_temp.good()){
        cerr<<"Pliki nie moga byc otwarte badz stworzone. Sprawdz prawa do modyfikacji plikow!"<<endl;
        return;
    }

    odczytywanie_do_struktury(temp_adresat, 2, edytowany_adresat);

    plik.close();
    plik_temp.close();

    remove(NAZWA_PLIKU.c_str());
    rename(NAZWA_PLIKU_TEMP.c_str(), NAZWA_PLIKU.c_str());
}
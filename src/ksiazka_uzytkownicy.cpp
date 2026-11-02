#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ksiazka_uzytkownicy.hpp"
#include <sstream>


KsiazkaUzytkownicy::KsiazkaUzytkownicy(){
    current_id = 1;
    id_zalogowanego_uzytkownika = 0;
    odczyt_z_pliku();
}

KsiazkaUzytkownicy::~KsiazkaUzytkownicy(){
    if (plik.is_open()){
        plik.close();
    }
}

bool KsiazkaUzytkownicy::odczyt_z_pliku(){
    std::string linia, atrybut;
    unsigned short int numer_linii{1};
    struktura_uzytkownicy uzytkownik;
    std::stringstream ss;

    plik.open(NAZWA_PLIKU_UZYTKOWNICY, std::ios::in);

    if(!plik.good()){
        std::cerr<<"Plik nie moze byc otwarty badz stworzony. Sprawdz prawa do modyfikacji pliku!"<<std::endl;
        return false;
    }
    else{
        std::cout<<"Odczyt danych z pliku "<<NAZWA_PLIKU_UZYTKOWNICY<<" w trakcie..."<<std::endl;

        while(getline(plik,linia)){
            ss.str(linia);
            while(getline(ss, atrybut, '|')){
                 switch(numer_linii){
                    case 1: uzytkownik.id_uzytkownika = stoi(atrybut);   break;
                    case 2: uzytkownik.nazwa = atrybut;   break;
                    case 3: 
                        uzytkownik.haslo = atrybut; 
                        ksiazka_uzytkownicy.push_back(uzytkownik);
                        numer_linii = 0;
                        break;
                }
                numer_linii++;
            }
            ss.clear();
        }

        current_id = uzytkownik.id_uzytkownika + 1;
        std::cout<<"Zakonczono odczyt danych! Odczytano "<<ksiazka_uzytkownicy.size()<<" uzytkownikow."<<std::endl;
    }
    plik.close();
    return true;
}


bool KsiazkaUzytkownicy::zapis_do_pliku(const struktura_uzytkownicy& uzytkownik){
    plik.open(NAZWA_PLIKU_UZYTKOWNICY, std::ios::app);

    if(!plik.good()){
        std::cerr<<"Plik nie moze byc otwarty badz stworzony. Sprawdz prawa do modyfikacji pliku!"<<std::endl;
        return false;
    }
    else{
        std::cout<<"Rozpoczynam zapis do pliku "<<NAZWA_PLIKU_UZYTKOWNICY<<" nowego uzytkownika "<<uzytkownik.nazwa<<"."<<std::endl;

        plik<<uzytkownik.id_uzytkownika<<"|";
        plik<<uzytkownik.nazwa<<"|";
        plik<<uzytkownik.haslo<<std::endl;

        std::cout<<"Zakonczono zapis!"<<std::endl; 

        ksiazka_uzytkownicy.push_back(uzytkownik);
        current_id++;
    }
    plik.close();
    return true;
}


void KsiazkaUzytkownicy::zarejestruj_czlonka(){
    struktura_uzytkownicy uzytkownik;
    std::string haslo_powtorzone;

    std::cout<<"Podaj nazwe uzytkownika: ";
    std::cin>>uzytkownik.nazwa;
    std::cout<<"Podaj haslo: ";
    std::cin>>uzytkownik.haslo;
    std::cout<<"Podaj ponownie haslo: ";
    std::cin>>haslo_powtorzone;

    if (uzytkownik.haslo != haslo_powtorzone){
        std::cout<<"Podane hasla nie sa identyczne, sprobuj ponownie!"<<std::endl;
        return;
    }

    std::cout<<std::endl;

    uzytkownik.id_uzytkownika = current_id;
    zapis_do_pliku(uzytkownik);
}


bool KsiazkaUzytkownicy::wyszukaj_login(const std::string& login){
    for(size_t i{0}; i<ksiazka_uzytkownicy.size(); ++i){
        if(ksiazka_uzytkownicy[i].nazwa.compare(login) == 0){
            id_zalogowanego_uzytkownika = ksiazka_uzytkownicy[i].id_uzytkownika;
            return true;
        }
    }
    return false;
}

bool KsiazkaUzytkownicy::sprawdz_haslo(const std::string& haslo){
    return (ksiazka_uzytkownicy[id_zalogowanego_uzytkownika-1].haslo.compare(haslo) == 0) ? true : false;
}

unsigned int KsiazkaUzytkownicy::zaloguj_czlonka(){
    std::string login, haslo;

    std::cout<<"Podaj login: ";
    std::cin>>login;
    std::cout<<"Podaj haslo: ";
    std::cin>>haslo;

    if(wyszukaj_login(login) && sprawdz_haslo(haslo)){
        std::cout<<"Zalogowano pomyslnie!"<<std::endl;
        return id_zalogowanego_uzytkownika;
    }
    else{
        std::cout<<"Niepoprawny login lub haslo, sprobuj ponownie!"<<std::endl;
        return 0;
    }
}

void KsiazkaUzytkownicy::wyloguj_czlonka(){
    id_zalogowanego_uzytkownika = 0;
}

void KsiazkaUzytkownicy::zmien_haslo_czlonka(){
    std::string nowe_haslo, nowe_haslo_powtorzone;

    if (id_zalogowanego_uzytkownika == 0){
        std::cout<<"Nie jestes zalogowany, nie mozesz zmienic hasla!"<<std::endl;
        return;
    }

    std::cout<<"Podaj nowe haslo: ";
    std::cin>>nowe_haslo;
    std::cout<<"Podaj ponownie nowe haslo: ";
    std::cin>>nowe_haslo_powtorzone;

    if (nowe_haslo != nowe_haslo_powtorzone){
        std::cout<<"Podane hasla nie sa identyczne, sprobuj ponownie!"<<std::endl;
        return;
    }

    ksiazka_uzytkownicy[id_zalogowanego_uzytkownika-1].haslo = nowe_haslo;

    // Update the password in the file
    plik.open(NAZWA_PLIKU_UZYTKOWNICY_TEMP, std::ios::out);

    if(!plik.good()){
        std::cerr<<"Plik Uzytkownicy_temp.txt nie moze byc otwarty badz stworzony. Sprawdz prawa do modyfikacji pliku!"<<std::endl;
        return;
    }

    for (const auto& uzytkownik : ksiazka_uzytkownicy) {
        plik << uzytkownik.id_uzytkownika << std::endl;
        plik << uzytkownik.nazwa << std::endl;
        plik << uzytkownik.haslo << std::endl;
    }
    plik.close();

    // Replace the old file with the new one
    std::remove(NAZWA_PLIKU_UZYTKOWNICY.c_str());
    std::rename(NAZWA_PLIKU_UZYTKOWNICY_TEMP.c_str(), NAZWA_PLIKU_UZYTKOWNICY.c_str());

    std::cout<<"Haslo zostalo pomyslnie zmienione!"<<std::endl;
}

void KsiazkaUzytkownicy::wyswietl_wszystkich(){
    std::cout<<"Lista wszystkich uzytkownikow:"<<std::endl;
    for(size_t i{0}; i<ksiazka_uzytkownicy.size(); ++i){
        std::cout<<"ID: "<<ksiazka_uzytkownicy[i].id_uzytkownika<<", Nazwa: "<<ksiazka_uzytkownicy[i].nazwa<<std::endl;
    }
}
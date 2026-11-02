#ifndef _ksiazka_uzytkownicy_HPP

#define _ksiazka_uzytkownicy_HPP

#include "struktura_uzytkownicy.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class KsiazkaUzytkownicy{

public:
    KsiazkaUzytkownicy();
    ~KsiazkaUzytkownicy();

    void zarejestruj_czlonka();
    unsigned int zaloguj_czlonka();
    
    void zmien_haslo_czlonka();
    void wyloguj_czlonka();

    void wyswietl_wszystkich();
    
private:

    bool zapis_do_pliku(const struktura_uzytkownicy& uzytkownik);
    bool odczyt_z_pliku();

    bool wyszukaj_login(const std::string& login);
    bool sprawdz_haslo(const std::string& haslo);

    unsigned int current_id, id_zalogowanego_uzytkownika;
    std::vector<struktura_uzytkownicy> ksiazka_uzytkownicy;
    
    std::fstream plik;
    const std::string NAZWA_PLIKU_UZYTKOWNICY{"Uzytkownicy.txt"};
    const std::string NAZWA_PLIKU_UZYTKOWNICY_TEMP{"Uzytkownicy_temp.txt"};
};


#endif
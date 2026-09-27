#ifndef _ksiazka_adresowa_HPP

#define _ksiazka_adresowa_HPP

#include "struktura.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class KsiazkaAdresowa{

public:
    KsiazkaAdresowa();
    ~KsiazkaAdresowa();

    void zapisz_czlonka();
    void wyszukaj_czlonka();
    void wyswietl_wszystkich();
    
private:

    bool zapis_do_pliku(const struktura& adresat);
    bool odczyt_z_pliku();

    unsigned int current_id;
    std::vector<struktura> ksiazka_adresowa;

    std::fstream plik;
    const std::string NAZWA_PLIKU{"ksiazka_adresowa.txt"};
};

#endif
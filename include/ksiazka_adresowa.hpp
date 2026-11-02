#ifndef _ksiazka_adresowa_HPP

#define _ksiazka_adresowa_HPP

#include "struktura_adresaci.hpp"
#include "struktura_edycja_adresata_w_ksiazce.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class KsiazkaAdresowa{

public:
    KsiazkaAdresowa(unsigned short int nadawca_id);
    ~KsiazkaAdresowa();

    void zapisz_czlonka();
    void wyszukaj_czlonka();
    void wyswietl_wszystkich();
    void edytuj_czlonka();
    void usun_czlonka();
    
private:

    bool zapis_do_pliku(const struktura_adresaci& adresat);
    bool odczyt_z_pliku();

    /**
    * @brief Funkcja upraszcza proces odczytu danych z pliku i przypisuje je do odpowiednich pól struktury. 
    * 
    * Funkcja przetwarza ciąg znaków linia po linii, a następnie dzieli każdy 
    * wiersz na pojedyncze elementy za pomocą podanego znaku podziału.
    * 
    * @param adresat Struktura, do której mają zostać przypisane wyodrębnione fragmenty.
    * @param typ_zapisu Określa tryb zapisu danych: 0 - zapis do wektora struktury przy wczytaniu ksiązki dla danego id, 1 - zapis do pliku z edycją, 2 - zapis do pliku usuwającego adresata.
    * @param edytowany_adresat Struktura zawierająca informacje o adresacie, który jest aktualnie edytowany lub usuwany.
    * 
    * @warning Nie ma sensu przekazywać edytowany_adresat w trybie 0.
    */
    void odczytywanie_do_struktury(struktura_adresaci& adresat, unsigned int typ_zapisu = 0, struktura_edycja_adresata_w_ksiazce edytowany_adresat = {0, false});

    void zapisywanie_adresata_do_pliku(const struktura_adresaci& adresat, std::fstream& plik);
    struktura_edycja_adresata_w_ksiazce edytuj_adresata_w_ksiazce(const std::string& imie, const std::string& nazwisko);

    unsigned int current_id, nadawca_id;
    
    std::vector<struktura_adresaci> ksiazka_adresowa;

    std::fstream plik, plik_temp;
    const std::string NAZWA_PLIKU{"ksiazka_adresowa.txt"};
    const std::string NAZWA_PLIKU_TEMP{"ksiazka_adresowa_temp.txt"};
};

#endif
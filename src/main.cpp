#include <iostream>
#include "ksiazka_adresowa.hpp"
#include "ksiazka_uzytkownicy.hpp"
#include "struktura_menu_wstepne.hpp"

using namespace std;

struktura_menu_wstepne menu_przed_zalogowaniem(KsiazkaUzytkownicy& Ksiazka_Uzytkownicy){
    struktura_menu_wstepne menu;
    cout<<endl;
    cout<<"Wybierz odpowiedni numer z menu aby aktywowac odpowiednia funkcje :"<<endl;
    cout<<"     0. Wyjdz z programu."<<endl;
    cout<<"     1. Zarejestruj nowego uzytkownika"<<endl;
    cout<<"     2. Zaloguj sie"<<endl<<endl;

    unsigned short int wybor;
    cin>>wybor;
    menu.wybor = wybor;

    switch(wybor){
            case 0: break;
            case 1: Ksiazka_Uzytkownicy.zarejestruj_czlonka(); break;
            case 2: menu.id_zalogowanego_uzytkownika = Ksiazka_Uzytkownicy.zaloguj_czlonka(); break;
            default:
                cout<<"Podano niepoprawna liczbe, sprobuj ponownie! "<<endl;
    }
    cout<<endl;

    return menu;
}

unsigned short int menu_po_zalogowaniu( KsiazkaUzytkownicy& Ksiazka_Uzytkownicy, unsigned short int id_zalogowanego_uzytkownika){
    KsiazkaAdresowa Ksiazka_Adresowa(id_zalogowanego_uzytkownika);

    cout<<endl;
    cout<<"Wybierz odpowiedni numer z menu aby aktywowac odpowiednia funkcje :"<<endl;
    cout<<"     1. Zapisz nowego adresata"<<endl;
    cout<<"     2. Wyszukaj istniejacego adresata"<<endl;
    cout<<"     3. Wyswietl wszystkich adresatow"<<endl;
    cout<<"     4. Edytuj istniejacego adresata"<<endl;
    cout<<"     5. Usun istniejacego adresata"<<endl;
    cout<<"////////////////////////////////////////////////////////////////"<<endl;
    cout<<"     6. Zmien haslo"<<endl;
    cout<<"     0. Wyloguj sie"<<endl<<endl;
    
    unsigned short int wybor;
    cin>>wybor;
    cout<<endl;

    switch(wybor){
        case 0: Ksiazka_Uzytkownicy.wyloguj_czlonka(); break;
        case 1: Ksiazka_Adresowa.zapisz_czlonka(); break;
        case 2: Ksiazka_Adresowa.wyszukaj_czlonka(); break;
        case 3: Ksiazka_Adresowa.wyswietl_wszystkich(); break;
        case 4: Ksiazka_Adresowa.edytuj_czlonka(); break;
        case 5: Ksiazka_Adresowa.usun_czlonka(); break;
        case 6: Ksiazka_Uzytkownicy.zmien_haslo_czlonka(); break;
        default:
            cout<<"Podano niepoprawna liczbe, sprobuj ponownie! "<<endl;
    }
    cout<<endl;

    return wybor;
}



int main(){

    cout<<"Witaj w programie Ksiazka adresowa."<<endl<<endl;

    KsiazkaUzytkownicy Ksiazka_Uzytkownicy;
    struktura_menu_wstepne menu;

    while(menu.wybor != 0){
       menu = menu_przed_zalogowaniem(Ksiazka_Uzytkownicy);
       if (menu.id_zalogowanego_uzytkownika != 0){
            unsigned short int wybor{1};

            while(wybor != 0){
                wybor = menu_po_zalogowaniu(Ksiazka_Uzytkownicy, menu.id_zalogowanego_uzytkownika);
            }
        }
    }
    return 0;
}